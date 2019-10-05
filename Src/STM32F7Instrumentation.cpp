
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_tim.h"

#include <touchgfx/hal/HAL.hpp>
#include <STM32F7Instrumentation.hpp>

/* USER CODE BEGIN user includes */

/* USER CODE END user includes */

namespace touchgfx
{
static TIM_HandleTypeDef htim1;

void STM32F7Instrumentation::init()
{
   RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock, uwAPB1Prescaler = 0U;
    uint32_t pFLatency;

    __TIM2_CLK_ENABLE();

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 215;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 10000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler( );
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler( );
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 3;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler( );
  }

    /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    /* TIM2 is on APB1 bus */
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;

    if (uwAPB1Prescaler == RCC_HCLK_DIV1)
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    else
        uwTimclock = 2 * HAL_RCC_GetPCLK1Freq();

    m_sysclkRatio = HAL_RCC_GetHCLKFreq() / uwTimclock;

    HAL_TIM_Base_Start(&htim1);
}

//Board specific clockfrequency
unsigned int STM32F7Instrumentation::getElapsedUS(unsigned int start, unsigned int now, unsigned int clockfrequency)
{
    return ((now - start) + (clockfrequency / 2)) / clockfrequency;
}

unsigned int STM32F7Instrumentation::getCPUCycles()
{
    return __HAL_TIM_GET_COUNTER(&htim1) * m_sysclkRatio;
}

void STM32F7Instrumentation::setMCUActive(bool active)
{
    if (active) //idle task sched out
    {
        cc_consumed += getCPUCycles() - cc_in;
    }
    else //idle task sched in
    {
        cc_in = getCPUCycles();
    }
}

}
