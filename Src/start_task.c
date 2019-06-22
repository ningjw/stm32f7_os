#include "main.h"

static TaskHandle_t AppTaskCreate_Handle; /* 创建任务句柄 */
static TaskHandle_t LED1_Task_Handle = NULL;/* LED1  任务句柄 */
static TaskHandle_t LED2_Task_Handle = NULL;/* LED2  任务句柄 */

extern SemaphoreHandle_t  BinarySem;
extern SemaphoreHandle_t  CountSem;
extern SemaphoreHandle_t  MuxSem;
extern EventGroupHandle_t Event;
extern QueueHandle_t      Queue;
extern TimerHandle_t      SoftTmr;


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
    taskENTER_CRITICAL(); //进入临界区
    
    SDRAM_Initialization_Sequence(&hsdram1);
    
    LTDC_Clear(0xffff);
    
    GT9147_Init();//电容触摸屏控制器初始化
    
    /*  创建 LED_Task  任务 */
    xTaskCreate((TaskFunction_t )led1_task, /*  任务入口函数 */
                (const char* )"LED2_Task",/*  任务名字 */
                (uint16_t )128, /*  任务栈大小 */
                (void* )NULL, /*  任务入口函数参数 */
                (UBaseType_t )5,  /*  任务的优先级 */
                (TaskHandle_t* )&LED2_Task_Handle);/*  任务控制块指针 */
    
    /*  创建 LED_Task  任务 */
    xTaskCreate((TaskFunction_t )led0_task, /*  任务入口函数 */
                (const char* )"LED1_Task",/*  任务名字 */
                (uint16_t )128, /*  任务栈大小 */
                (void* )NULL, /*  任务入口函数参数 */
                (UBaseType_t )6,  /*  任务的优先级 */
                (TaskHandle_t* )&LED1_Task_Handle);/*  任务控制块指针 */




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
    
    vTaskDelete(AppTaskCreate_Handle); //删除 AppTaskCreate 任务

    taskEXIT_CRITICAL(); //退出临界区
}

void os_init(void)
{
    taskENTER_CRITICAL();           //进入临界区

    /*  创建 LED_Task  任务 */
    xTaskCreate((TaskFunction_t )AppTaskCreate, /*  任务入口函数 */
                (const char* )"StartTask",/*  任务名字 */
                (uint16_t )128,   /*  任务栈大小 */
                (void* )NULL,     /*  任务入口函数参数 */
                (UBaseType_t )5,  /*  任务的优先级 */
                (TaskHandle_t* )&AppTaskCreate_Handle);/*  任务控制块指针 */
                
    vTaskStartScheduler();   /* 启动任务，开启调度 */

    taskEXIT_CRITICAL();            //退出临界区
}




