#include "main.h"

extern OS_FLAG_GRP flag_grp; //声明事件标志组
#define BIT0_EVENT (0x01 << 0)//设置事件掩码的位 0
#define BIT1_EVENT (0x01 << 1)//设置事件掩码的位 1

void flag_grp0_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    while(1)
    {
        OSFlagPost ((OS_FLAG_GRP *)&flag_grp,
                    (OS_FLAGS )BIT0_EVENT, //将标志组的 BIT0 置 1
                    (OS_OPT )OS_OPT_POST_FLAG_SET,
                    (OS_ERR *)&err);
        
        OSFlagPost ((OS_FLAG_GRP *)&flag_grp,
                    (OS_FLAGS )BIT1_EVENT,//将标志组的 BIT1 置 1
                    (OS_OPT )OS_OPT_POST_FLAG_SET,
                    (OS_ERR *)&err);
        
        OSTimeDly (1000, OS_OPT_TIME_DLY, &err );//延时
    }
}

void flag_grp1_task(void *p_arg)
{
    OS_ERR err;
    OS_FLAGS flags_rdy;
    p_arg = p_arg;
    while(1)
    {
        //等待标志组的的 BIT0 和 BIT1 均被置 1
        flags_rdy = OSFlagPend ((OS_FLAG_GRP *)&flag_grp,
                                (OS_FLAGS )( BIT0_EVENT | BIT1_EVENT ),
                                (OS_TICK )0,
                                (OS_OPT)OS_OPT_PEND_FLAG_SET_ALL |
                                OS_OPT_PEND_BLOCKING |
                                OS_OPT_PEND_FLAG_CONSUME,
                                (CPU_TS *)0,
                                (OS_ERR *)&err);
        if ((flags_rdy & (BIT0_EVENT|BIT1_EVENT)) == (BIT0_EVENT|BIT1_EVENT))
        {
            /* 如果接收完成并且正确 */
            printf ( "标志组的BIT0与BIT1 均被置 1\n");
        }
        OSTimeDly (1000, OS_OPT_TIME_DLY, &err );//延时
    }
}

