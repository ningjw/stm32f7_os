#include "main.h"

void delay_us(uint32_t nus)
{
    uint32_t ticks;
	uint32_t t_old,t_now,t_cnt=0;
	uint32_t reload = SysTick->LOAD;//LOAD的值	    	 
	ticks = nus * 216; 			//需要的节拍数 
    t_old = SysTick->VAL;        	//刚进入时的计数器值
    while(1)
    {
        t_now = SysTick->VAL;	
		if(t_now!=t_old)
		{	    
			if(t_now<t_old)
                t_cnt += t_old - t_now;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else
                t_cnt += reload - t_now + t_old;	
            
			t_old = t_now;
            
			if(t_cnt>=ticks)
                break;			//时间超过/等于要延迟的时间,则退出.
		}
    }	
}

void delay_ms(uint32_t ms){
    delay_us(ms * 1000);
}










