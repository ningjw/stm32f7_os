#include "main.h"

/*
******************************************************************************
                                      任务优先级
******************************************************************************
*/
#define LED0_PRIO 5
#define LED1_PRIO 5

/*
******************************************************************************
                                      任务栈大小
******************************************************************************
*/
#define LED0_STACK_SIZE 128
#define LED1_STACK_SIZE 128

/*
******************************************************************************
                                      任务时间片
******************************************************************************
*/
#define LED0_TIMESLICE 5
#define LED1_TIMESLICE 5
 

/* 定时器的控制块 */
static rt_timer_t timer1;

rt_sem_t sem = RT_NULL;/* 指向信号量的指针 */

rt_mutex_t mutex = RT_NULL;/* 创建互斥锁 */

rt_event_t event = RT_NULL;/* 指向事件的地址指针 */



/* 定时器1超时函数 */
void timeout1(void* parameter)
{
    static uint32_t count = 0;
    count++;
    printf("count = %d\r\n",count);
}


/*
******************************************************************************
                                  指向线程控制块的指针
******************************************************************************
*/
rt_thread_t led0 = RT_NULL;
rt_thread_t led1 = RT_NULL;


/* 用户应用入口 */
int rtt_init(void)
{
    /* 创建定时器1 */
    timer1 = rt_timer_create("timer1", /* 定时器名字是 timer1 */
            timeout1, 				   /* 超时时回调的处理函数 */
            RT_NULL, 				   /* 超时函数的入口参数 */
            1000, 					   /* 定时长度，以OS Tick为单位，即10个OS Tick */
            RT_TIMER_FLAG_PERIODIC);   /* 周期性定时器 */

    /* 启动定时器 */
    if (timer1 != RT_NULL) rt_timer_start(timer1);

    /* 创建一个信号量，初始值是1 */
    sem = rt_sem_create("sem", 1, RT_IPC_FLAG_FIFO);

    /* 创建一个互斥锁 */
    mutex = rt_mutex_create("mutex", RT_IPC_FLAG_FIFO);
    
    /* 创建事件 */
    event = rt_event_create( "event", RT_IPC_FLAG_FIFO);
    
    /* 创建线程1 */
    led0 = rt_thread_create("t1",led0_thread, (void*)1,
                            LED0_STACK_SIZE, LED0_PRIO, LED0_TIMESLICE);
    if (led0 != RT_NULL)
        rt_thread_startup(led0);
    else
        return -1;
    
    /* 创建线程2 */
    led1 = rt_thread_create("t2",led1_thread, (void*)2,
                            LED1_STACK_SIZE, LED1_PRIO, LED1_TIMESLICE);
    if (led1 != RT_NULL)
        rt_thread_startup(led1);
    else
        return -1;
    return 0;
}
