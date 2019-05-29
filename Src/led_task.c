#include "main.h"

UINT16 TimerId;/*  定义定时器句柄（ID ） */
UINT32 BinarySemId;/*  定义二值信号量的句柄 */
UINT32 CountSemId;/*  定义计数信号量的句柄 */
UINT32 MutexId;/*  定义 互斥锁 的句柄 */
EVENT_CB_S EventGroup_CB;/*  定义事件标志组的控制块 */
UINT32 QueueId;/*  定义消息队列的句柄 */

static void Timer_Callback(UINT32 arg)
{
    UINT32 tick_num1;

    tick_num1 = (UINT32)LOS_TickCountGet();  /*  获取滴答定时器的计数值 */
}

void led0_task(void)
{
    UINT32 send_data1 = 1;
    UINT32 *p_mem = NULL;
    
    LOS_SwtmrCreate(1000,  /*  软件定时器的定时时间（ms ）*/
                    LOS_SWTMR_MODE_PERIOD,/*  软件定时器模式  周期模式 */
                    (SWTMR_PROC_FUNC)Timer_Callback,/*  软件定时器的回调函数 */
                    &TimerId,  /*  软件定时器的 id */
                    0);
    
    LOS_SwtmrStart(TimerId);
    
    LOS_BinarySemCreate(1,&BinarySemId);/*  创建一个二值信号量*/
    
    LOS_SemCreate (5,&CountSemId);/*  创建一个计数信号量，初始化计数值为 5*/
    
    LOS_MuxCreate(&MutexId);/*  创建一个 互斥锁*/
    
    LOS_EventInit(&EventGroup_CB);/*  创建一个事件标志组*/

    /*  创建一个测试队列*/
    LOS_QueueCreate("Test_Queue",  /*  队列的名称 */
                    16,  /*  队列的长度 */
                    &QueueId, /*  队列的 ID( 句柄) */
                    0,  /*  队列模式，官方暂时未使用 */
                    16); /*  节点大小，单位为字节 */
    
    while(1)
    {
        LOS_SemPost( BinarySemId ); // 给出二值信号量
        
        LOS_SemPost(CountSemId);/* 释放一个计数信号量，LiteOS  的计数信号量允许一直释放*/
        
        LOS_MuxPost( MutexId ); // 给出 互斥锁
        
        LOS_EventWrite(&EventGroup_CB,0x01);/*  触发一个事件 1 */
        
        /* 将数据写入（发送）到队列中，等待时间为 0 */
        LOS_QueueWrite(QueueId,  /* 写入（发送）队列的 ID(句柄)  */
                       &send_data1, /* 写入（发送）的数据 */
                       sizeof(send_data1), /* 数据的长度 */
                       0); 
        
        p_mem = (UINT32*)LOS_MemAlloc(m_aucSysMem0,20);
                       
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
        LOS_TaskDelay(500);
    }
}


void led1_task(void)
{
    UINT32 *r_queue;
    while(1)
    {
        LOS_SemPend( BinarySemId, LOS_WAIT_FOREVER ); // 获取二值信号量 xSemaphore, 没获取到则一直等待
        
        LOS_SemPend ( CountSemId,0);/*  获取一个计数信号量，等待时间 0 */
        
        LOS_MuxPend( MutexId, LOS_WAIT_FOREVER );// 获取 互斥锁, 没获取到则一直等待
        
        /*  等待事件标志组 */
        LOS_EventRead(&EventGroup_CB,   // 事件标志组对象
                      0x01,             // 等待任务感兴趣的事件
                      LOS_WAITMODE_AND, // 等待两位均被置位
                      LOS_WAIT_FOREVER );// 无期限等待
        LOS_EventClear(&EventGroup_CB,~0x01); // 清除事件标志
        
        LOS_QueueRead(QueueId,           /*  读取（接收）队列的 ID( 句柄) */
                      &r_queue,          /*  读取（接收）的数据保存位置 */
                      16,                /*  读取（接收）的数据的长度*/
                      LOS_WAIT_FOREVER); /*  等待时间：一直等 */
        
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1);
        
        LOS_TaskDelay(500);
    }
}





