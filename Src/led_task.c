#include "main.h"

osSemaphoreId_t sem_id;
osMutexId_t mutex_id;
osMessageQueueId_t msg_q_id;
osEventFlagsId_t event_id;


typedef struct
{
    uint8_t id;
    uint8_t  msg[10];
}Message;

void led0_task(void *arg)
{
    Message tx_msg;
    
    sem_id = osSemaphoreNew(10, 1, NULL);
    
    mutex_id = osMutexNew(NULL);
    
    msg_q_id = osMessageQueueNew(10, sizeof(Message), NULL);
    
    event_id = osEventFlagsNew(NULL);
    
    while(1)
    {
        osSemaphoreRelease(sem_id);//释放信号量
        
        osMutexRelease(mutex_id);//释放互斥量
        
        tx_msg.id = 0x1;
        osMessageQueuePut(msg_q_id, &tx_msg, 0, NULL);
        
        osEventFlagsSet(event_id, 0x1);
        
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
        osDelay(500);
    }
}


void led1_task(void *arg)
{
    Message rx_msg;
    while(1)
    {
        osSemaphoreAcquire(sem_id, osWaitForever);//请求信号量
        
        osMutexAcquire(mutex_id, osWaitForever);//请求互斥量
        
        osMessageQueueGet(msg_q_id, &rx_msg, 0, osWaitForever);
        
        osEventFlagsWait(event_id, 0x1, osFlagsWaitAll, osWaitForever);
        
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1);
        osDelay(500);
    }
}









