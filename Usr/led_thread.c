#include "main.h"

extern rt_sem_t sem;/* 指向信号量的指针 */

extern rt_mutex_t mutex;/* 创建互斥锁 */

extern rt_event_t event;/* 指向事件的地址指针 */

void led0_thread(void* arg)
{
    arg = arg;
    while(1)
    {
        rt_sem_take(sem, RT_WAITING_FOREVER);/* 试图持有一个信号量，如果不成功则一直等待知道成功 */
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        rt_sem_release(sem); /* 释放一次信号量 */
        
        rt_thread_delay(500); /* 延时 500 个 tick */
    }
}

void led1_thread(void* arg)
{
    arg = arg;
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        rt_thread_delay(500); /* 延时 500 个 tick */
    }
}





