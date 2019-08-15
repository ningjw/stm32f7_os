/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

#include "GUI.h"
#include "HW_Init.h"
#include "GUI_App.h"
#include "STemwin_wrapper.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "WM.h"
#include "string.h"

#include "ltdc_driver.h"
#include "gt9147_iic_driver.h"
#include "w25q256_driver.h"
#include "utility.h"
#include "STemWin_BasicDemo.h"
#include "STemWin_Font.h"
#include "STemWin_Picture.h"
#include "malloc.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LAYER1_ADDR 0xC003FC00
#define LAYER0_ADDR 0xC0000000
#define LCD_WIDTH 480
#define LCD_HEIGHT 272
#define KEY_LEFT_Pin GPIO_PIN_13
#define KEY_LEFT_GPIO_Port GPIOC
#define KEY_UP_Pin GPIO_PIN_0
#define KEY_UP_GPIO_Port GPIOA
#define KEY_DOWN_Pin GPIO_PIN_2
#define KEY_DOWN_GPIO_Port GPIOH
#define KEY_RIGHT_Pin GPIO_PIN_3
#define KEY_RIGHT_GPIO_Port GPIOH
#define LED_DS1_Pin GPIO_PIN_0
#define LED_DS1_GPIO_Port GPIOB
#define LED_DS1B1_Pin GPIO_PIN_1
#define LED_DS1B1_GPIO_Port GPIOB
#define TOUCH_INT_Pin GPIO_PIN_7
#define TOUCH_INT_GPIO_Port GPIOH
#define TOUCH_INT_EXTI_IRQn EXTI9_5_IRQn
#define LCD_BL_Pin GPIO_PIN_5
#define LCD_BL_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
