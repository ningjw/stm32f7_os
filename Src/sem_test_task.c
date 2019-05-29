#include "main.h"

extern OS_MUTEX TEST_MUTEX;  //定义一个互斥信号量
extern OS_SEM SYNC_SEM;
extern OS_TCB SEM1TaskTCB;  

uint8_t share_res[30]; //共享资源区

void sem0_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    while(1)
    {
        OSTaskSemPost(&SEM1TaskTCB, OS_OPT_POST_NONE,&err); // 使用系统内建信号量向任务发送信号量
        OSTimeDly (1000, OS_OPT_TIME_DLY, &err );//延时
    }
}

void sem1_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    while(1)
    {
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err); // 请求任务内建的信号量
        printf("成功请求到内置信号量\r\n");
        OSSched();  
    }
}

#if 0
void sem0_task(void *p_arg)
{
    OS_ERR err;
    uint8_t task0_str[]="sem0 Running!";
    p_arg = p_arg;
    while(1)
    {
        OSMutexPend (&TEST_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);// 请求互斥信号量
        memcpy(share_res,task0_str,sizeof(task0_str));
        printf("%s",share_res);
        OSMutexPost(&TEST_MUTEX,OS_OPT_POST_NONE,&err);// 释放互斥信号量
        OSTimeDly (100, OS_OPT_TIME_DLY, &err );//延时
    }
}


void sem1_task(void *p_arg)
{
    OS_ERR err;
    uint8_t task1_str[]="sem1 Running!";
    p_arg = p_arg;
    while(1)
    {
        OSMutexPend (&TEST_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);// 请求互斥信号量
        memcpy(share_res,task1_str,sizeof(task1_str));
        printf("%s",share_res);
        OSMutexPost(&TEST_MUTEX,OS_OPT_POST_NONE,&err);// 释放互斥信号量
        OSTimeDly (100, OS_OPT_TIME_DLY, &err );//延时
    }
}
#endif

#if 0
void sem0_task(void *p_arg)
{
    OS_ERR err;
    uint8_t task0_str[]="sem0 Running!";
    p_arg = p_arg;
    while(1)
    {
        printf("sem0_task:\r\n");
        OSSemPend(&MY_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); // 请求信号量
        memcpy(share_res,task0_str,sizeof(task0_str));    // 向共享资源区拷贝数据
        OSTimeDly (100, OS_OPT_TIME_DLY, &err );          //相对性延时1000个时钟节拍（1s）
        OSSemPost (&MY_SEM,OS_OPT_POST_1,&err);           // 释放信号量
        OSTimeDly (100, OS_OPT_TIME_DLY, &err );          //相对性延时1000个时钟节拍（1s）
    }
}

void sem1_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    uint8_t task1_str[]="sem1 Running!";
    while(1)
    {
        printf("sem1_task:\r\n");
        OSSemPend(&MY_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); // 请求信号量
        memcpy(share_res,task1_str,sizeof(task1_str));    // 向共享资源区拷贝数据
        OSTimeDly (100, OS_OPT_TIME_DLY, &err );          //相对性延时1000个时钟节拍（1s）
        OSSemPost (&MY_SEM,OS_OPT_POST_1,&err);           // 释放信号量
        OSTimeDly (100, OS_OPT_TIME_DLY, &err );          //相对性延时1000个时钟节拍（1s）
    }
}
#endif

#if 0
void sem0_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    while(1)
    {
        OSSemPost(&SYNC_SEM,OS_OPT_POST_1,&err);//发送信号量
        OSTimeDly (100, OS_OPT_TIME_DLY, &err );//延时
    }
}

void sem1_task(void *p_arg)
{
    OS_ERR err;
    uint16_t num = 0;
    p_arg = p_arg;
    while(1)
    {
        OSSemPend(&SYNC_SEM,0,OS_OPT_PEND_BLOCKING,0,&err);// 请求信号量
        num++;
        printf("请求信号量次数： %d\r\n",num);
        OSTimeDly (1000, OS_OPT_TIME_DLY, &err );          //延时
    }
}
#endif



