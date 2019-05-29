#include "main.h"

OS_SEM SYNC_SEM;    //定义一个信号量
OS_MUTEX TEST_MUTEX;//定义一个互斥信号量
OS_FLAG_GRP flag_grp;
OS_Q msg_q;
OS_MEM mem;             //声明内存管理对象
uint8_t ucArray[3][20]; //声明内存分区大小
/*
*********************************************************************************************************
*                                         任务优先级
*********************************************************************************************************
*/
#define START_TASK_PRIO    3  //任务优先级
#define DS0_TASK_PRIO      4  //任务优先级
#define DS1_TASK_PRIO      5  //任务优先级
#define SEM0_TASK_PRIO     6  //任务优先级
#define SEM1_TASK_PRIO     7  //任务优先级
#define FLAG_GRP0_PRIO     8  //任务优先级
#define FLAG_GRP1_PRIO     9  //任务优先级
#define ROUND_ROBIN_TASK1  10  //时间片轮转调度任务1
#define ROUND_ROBIN_TASK2  10  //时间片轮转调度任务2
#define TMR_TASK_PRIO      11  //软件定时器

/*
*********************************************************************************************************
*                                         任务控制块TCB
*********************************************************************************************************
*/
OS_TCB StartTaskTCB;        
OS_TCB DS0TaskTCB;        
OS_TCB DS1TaskTCB;
OS_TCB SEM0TaskTCB;        
OS_TCB SEM1TaskTCB;  
OS_TCB FLAGGRP0TCB;        
OS_TCB FLAGGRP1TCB;  
OS_TCB RR1TaskTCB;
OS_TCB RR2TaskTCB;
OS_TCB TmrTaskTCB;
/*
*********************************************************************************************************
*                                         任务堆栈
*********************************************************************************************************
*/
#define START_STK_SIZE 128 
CPU_STK START_TASK_STK[START_STK_SIZE];//任务堆栈 

#define DS0_STK_SIZE 64  
CPU_STK DS0_TASK_STK[DS0_STK_SIZE];//任务堆栈 

#define DS1_STK_SIZE 64 
CPU_STK DS1_TASK_STK[DS1_STK_SIZE];//任务堆栈 

#define SEM0_STK_SIZE 64 
CPU_STK SEM0_TASK_STK[SEM0_STK_SIZE];//任务堆栈 

#define SEM1_STK_SIZE 64  
CPU_STK SEM1_TASK_STK[SEM1_STK_SIZE];//任务堆栈 

#define FLAG_GRP0_STK_SIZE 64 
CPU_STK FLAG_GRP0_STK[FLAG_GRP0_STK_SIZE];//任务堆栈 

#define FLAG_GRP1_STK_SIZE 64  
CPU_STK FLAG_GRP1_STK[FLAG_GRP1_STK_SIZE];//任务堆栈 

#define RR1_STK_SIZE 64 
CPU_STK RR1_TASK_STK[RR1_STK_SIZE];//任务堆栈 

#define RR2_STK_SIZE 64  
CPU_STK RR2_TASK_STK[RR2_STK_SIZE];//任务堆栈 

#define TMR_STK_SIZE 64  
CPU_STK TMR_TASK_STK[TMR_STK_SIZE];//任务堆栈 
/*
*********************************************************************************************************
*                                            函数原型
*********************************************************************************************************
*/
void start_task(void *p_arg);//任务函数


void ucos_init(void)
{
    OS_ERR err;
    CPU_SR_ALLOC();//定义cpu_sr变量
    
    CPU_Init();    //初始化 CPU 组件（时间戳、关中断时间测量和主机名）
    OSInit(&err);  //初始化 UCOSIII
    
    OS_CRITICAL_ENTER(); //进入临界区
    
    //创建开始任务
    OSTaskCreate((OS_TCB*  )&StartTaskTCB, //任务控制块
        (CPU_CHAR* )"start task",          //任务名字
        (OS_TASK_PTR )start_task,          //任务函数
        (void* )0,                         //传递给任务函数的参数
        (OS_PRIO  )START_TASK_PRIO,        //任务优先级
        (CPU_STK* )&START_TASK_STK[0],     //任务堆栈基地址
        (CPU_STK_SIZE )START_STK_SIZE/10,  //任务堆栈深度限位
        (CPU_STK_SIZE )START_STK_SIZE,     //任务堆栈大小
        (OS_MSG_QTY )0,                    //任务内部消息队列能够接收的最大消息数目,为 0 时禁止接收消息
        (OS_TICK )0,                       //当使能时间片轮转时的时间片长度，为 0 时为默认长度。
        (void* )0,                         //用户补充的存储区
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //存放该函数错误时的返回值
    OS_CRITICAL_EXIT();                    //退出临界区
    OSStart(&err); //开启 UCOSIII
}


//开始任务函数
void start_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err); //统计任务
#endif
#ifdef CPU_CFG_INT_DIS_MEAS_EN  
    CPU_IntDisMeasMaxCurReset();//复位（清零）当前最大关中断时间
#endif
#if OS_CFG_SCHED_ROUND_ROBIN_EN //当使用时间片轮转的时候
    //使能时间片轮转调度功能,设置默认的时间片长度10ms
    OSSchedRoundRobinCfg(DEF_ENABLED,5,&err);
#endif
    /* 创建消息队列 queue */
    OSQCreate((OS_Q *)&msg_q, //指向消息队列的指针
              (CPU_CHAR *)"Queue For Test", //队列的名字
              (OS_MSG_QTY )20, //最多可存放消息的数目
              (OS_ERR *)&err); //返回错误类型
    
    //创建一个信号量
    OSSemCreate ((OS_SEM*  )&SYNC_SEM, //指向信号量
                (CPU_CHAR*  )"MY_SEM", //信号量名字
                (OS_SEM_CTR )1, //信号量值为 1
                (OS_ERR* )&err);
                
                
    //创建一个互斥信号量
    OSMutexCreate((OS_MUTEX* )&TEST_MUTEX,
                  (CPU_CHAR* )"TEST_MUTEX",
                  (OS_ERR* )&err);
    
    //创建事件标志组
    OSFlagCreate ((OS_FLAG_GRP *)&flag_grp, //指向事件标志组的指针
                  (CPU_CHAR *)"FLAG For Test", //事件标志组的名字
                  (OS_FLAGS )0,    //事件标志组的初始值
                  (OS_ERR *)&err); //返回错误类型
    
    /*  创建内存管理对象 mem */
    OSMemCreate ((OS_MEM *)&mem, // 指向内存管理对象
                (CPU_CHAR *)"Mem For Test", // 命名内存管理对象
                (void *)ucArray, // 内存分区的首地址
                (OS_MEM_QTY )3, // 内存分区中内存块数目
                (OS_MEM_SIZE )20, // 内存块的字节数目
                (OS_ERR *)&err); //

    //创建LED0任务
    OSTaskCreate((OS_TCB*  )&DS0TaskTCB, //任务控制块
        (CPU_CHAR* )"led0 task",          //任务名字
        (OS_TASK_PTR )led0_task,          //任务函数
        (void* )0,                         //传递给任务函数的参数
        (OS_PRIO  )DS0_TASK_PRIO,        //任务优先级
        (CPU_STK* )&DS0_TASK_STK[0],     //任务堆栈基地址
        (CPU_STK_SIZE )DS0_STK_SIZE/10,  //任务堆栈深度限位
        (CPU_STK_SIZE )DS0_STK_SIZE,     //任务堆栈大小
        (OS_MSG_QTY )5,                    //任务内部消息队列能够接收的最大消息数目,为 0 时禁止接收消息
        (OS_TICK )0,                       //当使能时间片轮转时的时间片长度，为 0 时为默认长度。
        (void* )0,                         //用户补充的存储区
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //存放该函数错误时的返回值
    
    //创建LED1任务
    OSTaskCreate((OS_TCB*  )&DS1TaskTCB, //任务控制块
        (CPU_CHAR* )"led1 task",         //任务名字
        (OS_TASK_PTR )led1_task,         //任务函数
        (void* )0,                       //传递给任务函数的参数
        (OS_PRIO  )DS1_TASK_PRIO,        //任务优先级
        (CPU_STK* )&DS1_TASK_STK[0],     //任务堆栈基地址
        (CPU_STK_SIZE )DS1_STK_SIZE/10,  //任务堆栈深度限位
        (CPU_STK_SIZE )DS1_STK_SIZE,     //任务堆栈大小
        (OS_MSG_QTY )5,                  //任务内部消息队列能够接收的最大消息数目,为 0 时禁止接收消息
        (OS_TICK )0,                     //当使能时间片轮转时的时间片长度，为 0 时为默认长度。
        (void* )0,                       //用户补充的存储区
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //存放该函数错误时的返回值
     
    //创建SEM0任务
    OSTaskCreate((OS_TCB*  )&SEM0TaskTCB, //任务控制块
        (CPU_CHAR* )"sem0 task",          //任务名字
        (OS_TASK_PTR )sem0_task,          //任务函数
        (void* )0,                         //传递给任务函数的参数
        (OS_PRIO  )SEM0_TASK_PRIO,        //任务优先级
        (CPU_STK* )&SEM0_TASK_STK[0],     //任务堆栈基地址
        (CPU_STK_SIZE )SEM0_STK_SIZE/10,  //任务堆栈深度限位
        (CPU_STK_SIZE )SEM0_STK_SIZE,     //任务堆栈大小
        (OS_MSG_QTY )5,                    //任务内部消息队列能够接收的最大消息数目,为 0 时禁止接收消息
        (OS_TICK )0,                       //当使能时间片轮转时的时间片长度，为 0 时为默认长度。
        (void* )0,                         //用户补充的存储区
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //存放该函数错误时的返回值
    
    //创建SEM1任务
    OSTaskCreate((OS_TCB*  )&SEM1TaskTCB, //任务控制块
        (CPU_CHAR* )"sem1 task",         //任务名字
        (OS_TASK_PTR )sem1_task,         //任务函数
        (void* )0,                       //传递给任务函数的参数
        (OS_PRIO  )SEM1_TASK_PRIO,        //任务优先级
        (CPU_STK* )&SEM1_TASK_STK[0],     //任务堆栈基地址
        (CPU_STK_SIZE )SEM1_STK_SIZE/10,  //任务堆栈深度限位
        (CPU_STK_SIZE )SEM1_STK_SIZE,     //任务堆栈大小
        (OS_MSG_QTY )5,                  //任务内部消息队列能够接收的最大消息数目,为 0 时禁止接收消息
        (OS_TICK )0,                     //当使能时间片轮转时的时间片长度，为 0 时为默认长度。
        (void* )0,                       //用户补充的存储区
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //存放该函数错误时的返回值
        
    
    //创建FLAG_GRP0任务
    OSTaskCreate((OS_TCB*  )&FLAGGRP0TCB, //任务控制块
        (CPU_CHAR* )"flag grp0 task",     //任务名字
        (OS_TASK_PTR )flag_grp0_task,          //任务函数
        (void* )0,                         //传递给任务函数的参数
        (OS_PRIO  )FLAG_GRP0_PRIO,        //任务优先级
        (CPU_STK* )&FLAG_GRP0_STK[0],     //任务堆栈基地址
        (CPU_STK_SIZE )FLAG_GRP0_STK_SIZE/10,  //任务堆栈深度限位
        (CPU_STK_SIZE )FLAG_GRP0_STK_SIZE,     //任务堆栈大小
        (OS_MSG_QTY )5,                    //任务内部消息队列能够接收的最大消息数目,为 0 时禁止接收消息
        (OS_TICK )0,                       //当使能时间片轮转时的时间片长度，为 0 时为默认长度。
        (void* )0,                         //用户补充的存储区
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //存放该函数错误时的返回值

    //创建FLAG_GRP1任务
    OSTaskCreate((OS_TCB*  )&FLAGGRP1TCB, //任务控制块
        (CPU_CHAR* )"flag grp1 task",     //任务名字
        (OS_TASK_PTR )flag_grp1_task,          //任务函数
        (void* )0,                         //传递给任务函数的参数
        (OS_PRIO  )FLAG_GRP1_PRIO,        //任务优先级
        (CPU_STK* )&FLAG_GRP1_STK[0],     //任务堆栈基地址
        (CPU_STK_SIZE )FLAG_GRP1_STK_SIZE/10,  //任务堆栈深度限位
        (CPU_STK_SIZE )FLAG_GRP1_STK_SIZE,     //任务堆栈大小
        (OS_MSG_QTY )5,                    //任务内部消息队列能够接收的最大消息数目,为 0 时禁止接收消息
        (OS_TICK )0,                       //当使能时间片轮转时的时间片长度，为 0 时为默认长度。
        (void* )0,                         //用户补充的存储区
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //存放该函数错误时的返回值
        
    //创建时间片轮转任务1
    OSTaskCreate((OS_TCB*  )&RR1TaskTCB, //任务控制块
        (CPU_CHAR* )"RR1 task",          //任务名字
        (OS_TASK_PTR )RR1_task,          //任务函数
        (void* )0,                       //传递给任务函数的参数
        (OS_PRIO  )ROUND_ROBIN_TASK1,    //任务优先级
        (CPU_STK* )&RR1_TASK_STK[0],     //任务堆栈基地址
        (CPU_STK_SIZE )RR1_STK_SIZE/10,  //任务堆栈深度限位
        (CPU_STK_SIZE )RR1_STK_SIZE,     //任务堆栈大小
        (OS_MSG_QTY )5,                  //任务内部消息队列能够接收的最大消息数目,为 0 时禁止接收消息
        (OS_TICK )10,                    //当使能时间片轮转时的时间片长度，为 0 时为默认长度。
        (void* )0,                       //用户补充的存储区
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                 //存放该函数错误时的返回值
   
    //创建时间片轮转任务2    
    OSTaskCreate((OS_TCB*  )&RR2TaskTCB, //任务控制块
        (CPU_CHAR* )"RR1 task",          //任务名字
        (OS_TASK_PTR )RR2_task,          //任务函数
        (void* )0,                       //传递给任务函数的参数
        (OS_PRIO  )ROUND_ROBIN_TASK2,    //任务优先级
        (CPU_STK* )&RR2_TASK_STK[0],     //任务堆栈基地址
        (CPU_STK_SIZE )RR2_STK_SIZE/10,  //任务堆栈深度限位
        (CPU_STK_SIZE )RR2_STK_SIZE,     //任务堆栈大小
        (OS_MSG_QTY )5,                  //任务内部消息队列能够接收的最大消息数目,为 0 时禁止接收消息
        (OS_TICK )10,                    //当使能时间片轮转时的时间片长度，为 0 时为默认长度。
        (void* )0,                       //用户补充的存储区
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                 //存放该函数错误时的返回值

    //创建软件定时器任务
    OSTaskCreate((OS_TCB*  )&TmrTaskTCB, //任务控制块
        (CPU_CHAR* )"tmr task",         //任务名字
        (OS_TASK_PTR )tmr_task,         //任务函数
        (void* )0,                       //传递给任务函数的参数
        (OS_PRIO  )TMR_TASK_PRIO,        //任务优先级
        (CPU_STK* )&TMR_TASK_STK[0],     //任务堆栈基地址
        (CPU_STK_SIZE )TMR_STK_SIZE/10,  //任务堆栈深度限位
        (CPU_STK_SIZE )TMR_STK_SIZE,     //任务堆栈大小
        (OS_MSG_QTY )5,                  //任务内部消息队列能够接收的最大消息数目,为 0 时禁止接收消息
        (OS_TICK )0,                     //当使能时间片轮转时的时间片长度，为 0 时为默认长度。
        (void* )0,                       //用户补充的存储区
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //存放该函数错误时的返回值
        
    OSTaskDel(0, &err ); //删除起始任务本身，该任务不再运行
}




