#include "main.h"

extern OS_Q msg_q; //声明消息队列

void RR1_task(void *p_arg)
{
    OS_ERR  err;
    p_arg = p_arg;
    while(1)
    {
        /* 发布消息到消息队列 queue */
        OSQPost ((OS_Q *)&msg_q,                    //消息变量指针
                (void *)"msg_q test",               //要发送的数据的指针，将内存块首地址通过队列“发送出去”
                (OS_MSG_SIZE )sizeof ("msg_q test"),//数据字节大小
                (OS_OPT )OS_OPT_POST_FIFO | OS_OPT_POST_ALL, //先进先出和发布给全部任务的形式
                (OS_ERR *)&err);                         //返回错误类型
        OSTimeDly (1000, OS_OPT_TIME_DLY, &err );   //相对性延时1000个时钟节拍（1s）
    }
}

void RR2_task(void *p_arg)
{
    OS_ERR  err;
    CPU_SR_ALLOC();
    p_arg = p_arg;
    OS_MSG_SIZE msg_size;
    //定义一个局部变量，用于保存关中断前的 CPU 状态寄存器
    char * pMsg;
    while(1)
    {
        /* 请求消息队列 queue 的消息 */
        pMsg = OSQPend ((OS_Q *)&msg_q, //消息变量指针
                        (OS_TICK )100, //等待时长
                        (OS_OPT )OS_OPT_PEND_NON_BLOCKING,//如果没有获取到信号量就等待
                        (OS_MSG_SIZE *)&msg_size, //获取消息的字节大小
                        (CPU_TS *)0, //获取任务发送时的时间戳
                        (OS_ERR *)&err); //返回错误
        
        if ( err == OS_ERR_NONE ) //如果接收成功
        {
            OS_CRITICAL_ENTER(); //进入临界段
            printf ( "RR2_task：%d 字节，内容：%s\r\n", msg_size, pMsg );
            OS_CRITICAL_EXIT();
        }
        OSTimeDly (1, OS_OPT_TIME_DLY, &err );
    }
}

