#include "main.h"


static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];/*  空闲任务任务堆栈 */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];/*  定时器任务堆栈 */
static StackType_t AppTaskCreate_Stack[128];/* AppTaskCreate任务任务堆栈 */

static StaticTask_t Idle_Task_TCB;/*  空闲任务控制块 */
static StaticTask_t Timer_Task_TCB;/*  定时器任务控制块 */
static StaticTask_t AppTaskCreate_TCB;/* AppTaskCreate 任务控制块 */



static TaskHandle_t AppTaskCreate_Handle; /* 创建任务句柄 */
static TaskHandle_t LED1_Task_Handle = NULL;/* LED1  任务句柄 */
static TaskHandle_t LED2_Task_Handle = NULL;/* LED2  任务句柄 */
/**
*******************************************************************
* @brief 获取空闲任务的任务堆栈和任务控制块内存
* ppxTimerTaskTCBBuffer  :  任务控制块内存
* ppxTimerTaskStackBuffer  :  任务堆栈内存
* pulTimerTaskStackSize  :  任务堆栈大小
**********************************************************************
*/
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &Idle_Task_TCB; /*  任务控制块内存 */
    *ppxIdleTaskStackBuffer = Idle_Task_Stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE; /*  任务堆栈大小 */
}


/**
*********************************************************************
* @brief 获取定时器任务的任务堆栈和任务控制块内存
* ppxTimerTaskTCBBuffer  :  任务控制块内存
* ppxTimerTaskStackBuffer  :  任务堆栈内存
* pulTimerTaskStackSize  :  任务堆栈大小
**********************************************************************
*/
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &Timer_Task_TCB; /*  任务控制块内存 */
    *ppxTimerTaskStackBuffer = Timer_Task_Stack; /*  任务堆栈内存 */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH; /*  任务堆栈大小 */
}

/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
    taskENTER_CRITICAL(); //进入临界区

    /*  创建 LED_Task  任务 */
    xTaskCreate((TaskFunction_t )led0_task, /*  任务入口函数 */
                (const char* )"LED1_Task",/*  任务名字 */
                (uint16_t )128, /*  任务栈大小 */
                (void* )NULL, /*  任务入口函数参数 */
                (UBaseType_t )4,  /*  任务的优先级 */
                (TaskHandle_t* )&LED1_Task_Handle);/*  任务控制块指针 */


    /*  创建 LED_Task  任务 */
    xTaskCreate((TaskFunction_t )led1_task, /*  任务入口函数 */
                (const char* )"LED2_Task",/*  任务名字 */
                (uint16_t )128, /*  任务栈大小 */
                (void* )NULL, /*  任务入口函数参数 */
                (UBaseType_t )5,  /*  任务的优先级 */
                (TaskHandle_t* )&LED2_Task_Handle);/*  任务控制块指针 */

    vTaskDelete(AppTaskCreate_Handle); //删除 AppTaskCreate 任务

    taskEXIT_CRITICAL(); //退出临界区
}

void os_init(void)
{
    taskENTER_CRITICAL();           //进入临界区

    /* 创建 AppTaskCreate 任务 */
    AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t)AppTaskCreate,	//任务函数
                           (const char*  )"AppTaskCreate",//任务名称
                           (uint32_t 	   )128,	        //任务堆栈大小
                           (void* 	   )NULL,	        //传递给任务函数的参数
                           (UBaseType_t  )3,           	//任务优先级
                           (StackType_t* )AppTaskCreate_Stack,//任务堆栈
                           (StaticTask_t*)&AppTaskCreate_TCB);//任务控制块

    if(NULL != AppTaskCreate_Handle)/* 创建成功 */
        vTaskStartScheduler();   /* 启动任务，开启调度 */

    taskEXIT_CRITICAL();            //退出临界区
    vTaskStartScheduler();   /* 启动任务，开启调度 */
}




