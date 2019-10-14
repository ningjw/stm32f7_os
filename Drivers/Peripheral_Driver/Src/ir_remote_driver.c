#include "stm32f7xx.h"
#include "ir_remote_driver.h"


/***************************************************************************************
  * @brief   定时器更新（溢出）中断回调函数
  * @input   
  * @return  
***************************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM1)
    {
        
    }
}


/***************************************************************************************
  * @brief   定时器输入捕获中断回调函数
  * @input   
  * @return  
***************************************************************************************/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获中断发生时执行
{
    if(htim->Instance==TIM1)
    {
    }
}
