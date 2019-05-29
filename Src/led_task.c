#include "main.h"

extern OS_TCB DS1TaskTCB; 
extern OS_MEM mem;

void led0_task(void *p_arg)
{
    OS_ERR  err;
    p_arg = p_arg;
    char * p_mem_blk;
    
    while(DEF_TRUE){
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        
        /*  向 mem  获取内存块 */
        p_mem_blk = OSMemGet ((OS_MEM *)&mem,(OS_ERR *)&err);
        strcpy(p_mem_blk,"mem test");
        // 发送消息到任务内置队列
        OSTaskQPost((OS_TCB* )&DS1TaskTCB, // 向任务 DS1TaskTCB  发送消息
                    (void*  )p_mem_blk,
                    (OS_MSG_SIZE )strlen(p_mem_blk),
                    (OS_OPT  )OS_OPT_POST_FIFO,
                    (OS_ERR*  )&err);

        OSTimeDly (1000, OS_OPT_TIME_DLY, &err );   //相对性延时1000个时钟节拍（1s）
    }
}

void led1_task(void *p_arg)
{
    OS_ERR  err;
    p_arg = p_arg;
    OS_MSG_SIZE size;
    char *p_msg;
    while(1){
        
        // 请求消息
        p_msg = OSTaskQPend((OS_TICK  )10,                    //等待消息的超时时间
                            (OS_OPT )OS_OPT_PEND_BLOCKING,//是否使用阻塞模式
                            (OS_MSG_SIZE*  )&size,            //指向存放消息大小的变量
                            (CPU_TS* )0,                      //指向一个时间戳，表明什么时候接收到消息。
                            (OS_ERR* )&err );
        if(err == OS_ERR_NONE){
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
            printf ("led1_task：%d 字节，内容：%s\r\n", size, (char *)p_msg );
        }
        
        /*  退还内存块 */
        OSMemPut((OS_MEM *)&mem, // 指向内存管理对象
                (void *)p_msg, // 内存块的首地址
                (OS_ERR *)&err); //
        OSTimeDly (500, OS_OPT_TIME_DLY, &err );   //相对性延时1000个时钟节拍（1s）
    }
}


