#include "main.h"

void delay_ms(uint16_t nms)
{
    delay_us((uint32_t)(nms * 1000));				//普通方式延时
}

void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t t_old, t_now, t_cnt = 0;
    uint32_t reload = SysTick->LOAD;//LOAD的值
    ticks = nus * 216; 			//需要的节拍数
    t_old = SysTick->VAL;        	//刚进入时的计数器值
    while(1)
    {
        t_now = SysTick->VAL;
        if(t_now != t_old)
        {
            if(t_now < t_old)
                t_cnt += t_old - t_now;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
            else
                t_cnt += reload - t_now + t_old;

            t_old = t_now;

            if(t_cnt >= ticks)
                break;			//时间超过/等于要延迟的时间,则退出.
        }
    }
}

//#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};

//FILE __stdout;

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









