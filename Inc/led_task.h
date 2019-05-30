#ifndef __LED_TASK
#define __LED_TASK

void led0_task(void* arg);
void led1_task(void* arg);
void SoftTmr_Callback(TimerHandle_t xTimer);

#endif
