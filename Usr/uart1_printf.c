#include <stdio.h>
#include "main.h"

//标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;

//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}

int fputc(int ch, FILE *f)
{
    while((USART1->ISR & 0X40) == 0); //循环发送,直到发送完毕
    USART1->TDR = (uint8_t)ch;
    return ch;
}

/**
  * @brief 重映射串口 DEBUG_USARTx 到 rt_kprintf()函数
  * @param str：要输出到串口的字符串
  * @retval 无
  *
  * @attention
  *
*/
void rt_hw_console_output(const char *str)
{
    /* 进入临界段 */
    rt_enter_critical();

    /* 直到字符串结束 */
    while (*str != '\0')
    {
        /* 换行 */
        if (*str == '\n')
        {
            USART1->TDR = '\r';
            while((USART1->ISR & 0X40) == 0); //直到发送完毕
        }

        USART1->TDR = *str++;
        while((USART1->ISR & 0X40) == 0); //直到发送完毕
    }

    /* 退出临界段 */
    rt_exit_critical();
}




