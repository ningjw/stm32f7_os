#include "main.h"

CPU_TS             ts_start;       //时间戳变量
CPU_TS             ts_end; 

void TmrCallback (OS_TMR *p_tmr, void *p_arg) //软件定时器MyTmr的回调函数
{
    CPU_INT32U       cpu_clk_freq;
    
    printf ( "%s", ( char * ) p_arg );
    
    cpu_clk_freq = HAL_RCC_GetSysClockFreq();  //获取CPU时钟，时间戳是以该时钟计数
    ts_end = OS_TS_GET() - ts_start;              //获取定时后的时间戳（以CPU时钟进行计数的一个计数值）
    
    printf ( "\r\n定时1s，通过时间戳测得定时%04d ms。\r\n", ts_end / (cpu_clk_freq/1000) );//将定时时间折算成 ms
    ts_start = OS_TS_GET();                            //获取定时前时间戳
}


void  tmr_task( void * p_arg )
{
    OS_ERR      err;
    OS_TMR      my_tmr;   //声明软件定时器对象
    (void)p_arg;

    /* 创建软件定时器 */
    OSTmrCreate ((OS_TMR              *)&my_tmr,             //软件定时器对象
                 (CPU_CHAR            *)"MySoftTimer",       //命名软件定时器
                 (OS_TICK              )10,                  //定时器初始值，依10Hz时基计算，即为1s
                 (OS_TICK              )10,                  //定时器周期重载值，依10Hz时基计算，即为1s
                 (OS_OPT               )OS_OPT_TMR_PERIODIC, //周期性定时
                 (OS_TMR_CALLBACK_PTR  )TmrCallback,         //回调函数
                 (void                *)"Timer Over!",       //传递实参给回调函数
                 (OS_ERR              *)err);                //返回错误类型

    /* 启动软件定时器 */
    OSTmrStart(&my_tmr, &err);
    ts_start = OS_TS_GET();                       //获取定时前时间戳
                 
    while (DEF_TRUE) {                            //任务体，通常写成一个死循环
        OSTimeDly ( 1000, OS_OPT_TIME_DLY, &err ); //不断阻塞该任务
    }
}




