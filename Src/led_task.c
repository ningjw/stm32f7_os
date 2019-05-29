#include "main.h"

SemaphoreHandle_t  BinarySem = NULL;
SemaphoreHandle_t  CountSem = NULL;
SemaphoreHandle_t  MuxSem = NULL;
EventGroupHandle_t Event = NULL;
QueueHandle_t      Queue = NULL;

TimerHandle_t SoftTmr = NULL; /*  软件定时器句柄 */

TimerHandle_t SoftTmrStatic = NULL;//静态软件定时器
StaticTimer_t xTimerBuffers;

void SoftTmr_Callback(TimerHandle_t xTimer)
{
    
}

void led0_task(void* arg)
{
    BinarySem = xSemaphoreCreateBinary();    /*  创建 二值 信号量 */
    CountSem = xSemaphoreCreateCounting(5, 5); /*  创建 计数信号量 */
    MuxSem = xSemaphoreCreateMutex();        /*  创建 互斥信号量 */
    Event = xEventGroupCreate();/*  创建 事件组 */
    Queue = xQueueCreate((UBaseType_t ) 4,/*  消息队列的长度 */
                         (UBaseType_t ) 4);/*  消息的大小 */

    SoftTmr = xTimerCreate((const char*)"AutoReloadTimer",
                           (TickType_t)1000,   /* 定时器周期 1000(tick) */
                           (UBaseType_t)pdTRUE,/*  周期模式 */
                           (void*)1,           /* 为每个计时器分配一个索引的唯一 ID */
                            SoftTmr_Callback);
    if (SoftTmr != NULL) {
        xTimerStart(SoftTmr, 0); // 开启周期定时器
    }
    
    SoftTmrStatic = xTimerCreateStatic("Timer",
                                       1000,
                                       pdTRUE,
                                       ( void * )2,
                                       SoftTmr_Callback,
                                       &xTimerBuffers);

    arg = arg;
    uint32_t send_data1 = 1;
    while(1)
    {
        xSemaphoreGive( BinarySem );
        xEventGroupSetBits(Event, 0x1); /*  触发一个事件 1 */
        xQueueSend( Queue,      /*  消息队列的句柄 */
                    &send_data1,/*  发送的消息内容 */
                    0 );        /*  等待时间 0 */
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        vTaskDelay(500); /* 延时 500 个 tick */
    }
}

void led1_task(void* arg)
{
    EventBits_t r_event; /*  定义一个事件接收变量 */
    uint32_t r_queue; /*  定义一个接收消息的变量 */
    arg = arg;
    while(1)
    {
        xSemaphoreTake(BinarySem, portMAX_DELAY);
        r_event = xEventGroupWaitBits(Event, /*  事件对象句柄 */
                                      0x01,/*  接收 任务 感兴趣的事件 */
                                      pdTRUE, /*  退出时清除事件位 */
                                      pdTRUE, /*  满足感兴趣的所有事件 */
                                      portMAX_DELAY);/*  指定超时事件, 一直等 */
        xQueueReceive( Queue, /*  消息队列的句柄 */
                       &r_queue, /*  发送的消息内容 */
                       portMAX_DELAY); /*  等待时间  一直等 */
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        vTaskDelay(500); /* 延时 500 个 tick */
    }
}





