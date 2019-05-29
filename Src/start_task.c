#include "main.h"

osThreadId_t STaskId;

void start_task(void *argument) 
{
    osThreadNew(led0_task, NULL, NULL);    // Create  thread
    
    osThreadNew(led1_task, NULL, NULL);    // Create thread
    
    osThreadExit();
}

void os_init(void)
{
     osThreadAttr_t attr;
    
    osKernelInitialize(); // Initialize CMSIS-RTOS
    
    attr.name = "start_task";         //name of the thread
    attr.attr_bits = osThreadJoinable;//create thread in joinable mode
    attr.cb_mem = NULL;               //use Automatic Dynamic Allocation for the thread control block
    attr.cb_size = 0;                 //no memory provided with cb_mem.
    attr.stack_mem =  NULL;           //allocate stack from a fixed-size memory pool using Thread Stack Management.
    attr.stack_size = 0;              //The size (in bytes) of the stack specified by stack_mem.
    attr.priority = 3;
    attr.tz_module = 0;               //not thread context specified
    attr.reserved = 0;                //must be 0
    STaskId = osThreadNew(start_task, NULL, &attr);    // Create application main thread
    
    if (osKernelGetState() == osKernelReady) {
       osKernelStart();                    // Start thread execution
    }
}






