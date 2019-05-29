/******************************************************************************
 * File Name   : MX_Device.h
 * Date        : 26/05/2019 17:49:34
 * Description : STM32Cube MX parameter definitions
 * Note        : This file is generated by STM32CubeMX (DO NOT EDIT!)
 ******************************************************************************/

#ifndef __MX_DEVICE_H
#define __MX_DEVICE_H

/*---------------------------- Clock Configuration ---------------------------*/

#define MX_LSI_VALUE                            32000
#define MX_LSE_VALUE                            32768
#define MX_HSI_VALUE                            16000000
#define MX_HSE_VALUE                            25000000
#define MX_EXTERNAL_CLOCK_VALUE                 12288000
#define MX_SYSCLKFreq_VALUE                     216000000
#define MX_HCLKFreq_Value                       216000000
#define MX_FCLKCortexFreq_Value                 216000000
#define MX_CortexFreq_Value                     216000000
#define MX_AHBFreq_Value                        216000000
#define MX_APB1Freq_Value                       54000000
#define MX_APB2Freq_Value                       108000000
#define MX_APB1TimFreq_Value                    108000000
#define MX_APB2TimFreq_Value                    216000000
#define MX_EthernetFreq_Value                   216000000
#define MX_CECFreq_Value                        32786
#define MX_I2C1Freq_Value                       54000000
#define MX_I2C2Freq_Value                       54000000
#define MX_I2C3Freq_Value                       54000000
#define MX_I2C4Freq_Value                       54000000
#define MX_I2SFreq_Value                        192000000
#define MX_SAI1Freq_Value                       192000000
#define MX_SAI2Freq_Value                       192000000
#define MX_SDMMCFreq_Value                      216000000
#define MX_RTCFreq_Value                        32000
#define MX_USART1Freq_Value                     108000000
#define MX_USART2Freq_Value                     54000000
#define MX_USART3Freq_Value                     54000000
#define MX_UART4Freq_Value                      54000000
#define MX_UART5Freq_Value                      54000000
#define MX_UART8Freq_Value                      54000000
#define MX_UART7Freq_Value                      54000000
#define MX_USART6Freq_Value                     108000000
#define MX_USBFreq_Value                        216000000
#define MX_WatchDogFreq_Value                   32000
#define MX_LPTIM1Freq_Value                     54000000
#define MX_SPDIFRXFreq_Value                    192000000
#define MX_MCO1PinFreq_Value                    16000000
#define MX_MCO2PinFreq_Value                    216000000

/*-------------------------------- CORTEX_M7  --------------------------------*/

#define MX_CORTEX_M7                            1

/* GPIO Configuration */

/*-------------------------------- CRC        --------------------------------*/

#define MX_CRC                                  1

/* GPIO Configuration */

/*-------------------------------- FMC        --------------------------------*/

#define MX_FMC                                  1

/* GPIO Configuration */

/* Pin PE11 */
#define MX_FMC_D8_DA8_Pin                       PE11
#define MX_FMC_D8_DA8_GPIOx                     GPIOE
#define MX_FMC_D8_DA8_GPIO_PuPd                 GPIO_NOPULL
#define MX_FMC_D8_DA8_GPIO_Speed_High_Default   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D8_DA8_GPIO_Pin                  GPIO_PIN_11
#define MX_FMC_D8_DA8_GPIO_AF                   GPIO_AF12_FMC
#define MX_FMC_D8_DA8_GPIO_Mode                 GPIO_MODE_AF_PP

/* Pin PD14 */
#define MX_FMC_D0_DA0_Pin                       PD14
#define MX_FMC_D0_DA0_GPIOx                     GPIOD
#define MX_FMC_D0_DA0_GPIO_PuPd                 GPIO_NOPULL
#define MX_FMC_D0_DA0_GPIO_Speed_High_Default   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D0_DA0_GPIO_Pin                  GPIO_PIN_14
#define MX_FMC_D0_DA0_GPIO_AF                   GPIO_AF12_FMC
#define MX_FMC_D0_DA0_GPIO_Mode                 GPIO_MODE_AF_PP

/* Pin PC7 */
#define MX_FMC_NE1_Pin                          PC7
#define MX_FMC_NE1_GPIOx                        GPIOC
#define MX_FMC_NE1_GPIO_PuPd                    GPIO_NOPULL
#define MX_FMC_NE1_GPIO_Speed_High_Default      GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_NE1_GPIO_Pin                     GPIO_PIN_7
#define MX_FMC_NE1_GPIO_AF                      GPIO_AF9_FMC
#define MX_FMC_NE1_GPIO_Mode                    GPIO_MODE_AF_PP

/* Pin PE8 */
#define MX_FMC_D5_DA5_Pin                       PE8
#define MX_FMC_D5_DA5_GPIOx                     GPIOE
#define MX_FMC_D5_DA5_GPIO_PuPd                 GPIO_NOPULL
#define MX_FMC_D5_DA5_GPIO_Speed_High_Default   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D5_DA5_GPIO_Pin                  GPIO_PIN_8
#define MX_FMC_D5_DA5_GPIO_AF                   GPIO_AF12_FMC
#define MX_FMC_D5_DA5_GPIO_Mode                 GPIO_MODE_AF_PP

/* Pin PE13 */
#define MX_FMC_D10_DA10_Pin                     PE13
#define MX_FMC_D10_DA10_GPIOx                   GPIOE
#define MX_FMC_D10_DA10_GPIO_PuPd               GPIO_NOPULL
#define MX_FMC_D10_DA10_GPIO_Speed_High_Default GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D10_DA10_GPIO_Pin                GPIO_PIN_13
#define MX_FMC_D10_DA10_GPIO_AF                 GPIO_AF12_FMC
#define MX_FMC_D10_DA10_GPIO_Mode               GPIO_MODE_AF_PP

/* Pin PD1 */
#define MX_FMC_D3_DA3_Pin                       PD1
#define MX_FMC_D3_DA3_GPIOx                     GPIOD
#define MX_FMC_D3_DA3_GPIO_PuPd                 GPIO_NOPULL
#define MX_FMC_D3_DA3_GPIO_Speed_High_Default   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D3_DA3_GPIO_Pin                  GPIO_PIN_1
#define MX_FMC_D3_DA3_GPIO_AF                   GPIO_AF12_FMC
#define MX_FMC_D3_DA3_GPIO_Mode                 GPIO_MODE_AF_PP

/* Pin PD8 */
#define MX_FMC_D13_DA13_Pin                     PD8
#define MX_FMC_D13_DA13_GPIOx                   GPIOD
#define MX_FMC_D13_DA13_GPIO_PuPd               GPIO_NOPULL
#define MX_FMC_D13_DA13_GPIO_Speed_High_Default GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D13_DA13_GPIO_Pin                GPIO_PIN_8
#define MX_FMC_D13_DA13_GPIO_AF                 GPIO_AF12_FMC
#define MX_FMC_D13_DA13_GPIO_Mode               GPIO_MODE_AF_PP

/* Pin PD9 */
#define MX_FMC_D14_DA14_Pin                     PD9
#define MX_FMC_D14_DA14_GPIOx                   GPIOD
#define MX_FMC_D14_DA14_GPIO_PuPd               GPIO_NOPULL
#define MX_FMC_D14_DA14_GPIO_Speed_High_Default GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D14_DA14_GPIO_Pin                GPIO_PIN_9
#define MX_FMC_D14_DA14_GPIO_AF                 GPIO_AF12_FMC
#define MX_FMC_D14_DA14_GPIO_Mode               GPIO_MODE_AF_PP

/* Pin PD5 */
#define MX_FMC_NWE_Pin                          PD5
#define MX_FMC_NWE_GPIOx                        GPIOD
#define MX_FMC_NWE_GPIO_PuPd                    GPIO_NOPULL
#define MX_FMC_NWE_GPIO_Speed_High_Default      GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_NWE_GPIO_Pin                     GPIO_PIN_5
#define MX_FMC_NWE_GPIO_AF                      GPIO_AF12_FMC
#define MX_FMC_NWE_GPIO_Mode                    GPIO_MODE_AF_PP

/* Pin PE14 */
#define MX_FMC_D11_DA11_Pin                     PE14
#define MX_FMC_D11_DA11_GPIOx                   GPIOE
#define MX_FMC_D11_DA11_GPIO_PuPd               GPIO_NOPULL
#define MX_FMC_D11_DA11_GPIO_Speed_High_Default GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D11_DA11_GPIO_Pin                GPIO_PIN_14
#define MX_FMC_D11_DA11_GPIO_AF                 GPIO_AF12_FMC
#define MX_FMC_D11_DA11_GPIO_Mode               GPIO_MODE_AF_PP

/* Pin PE7 */
#define MX_FMC_D4_DA4_Pin                       PE7
#define MX_FMC_D4_DA4_GPIOx                     GPIOE
#define MX_FMC_D4_DA4_GPIO_PuPd                 GPIO_NOPULL
#define MX_FMC_D4_DA4_GPIO_Speed_High_Default   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D4_DA4_GPIO_Pin                  GPIO_PIN_7
#define MX_FMC_D4_DA4_GPIO_AF                   GPIO_AF12_FMC
#define MX_FMC_D4_DA4_GPIO_Mode                 GPIO_MODE_AF_PP

/* Pin PD13 */
#define MX_FMC_A18_Pin                          PD13
#define MX_FMC_A18_GPIOx                        GPIOD
#define MX_FMC_A18_GPIO_PuPd                    GPIO_NOPULL
#define MX_FMC_A18_GPIO_Speed_High_Default      GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_A18_GPIO_Pin                     GPIO_PIN_13
#define MX_FMC_A18_GPIO_AF                      GPIO_AF12_FMC
#define MX_FMC_A18_GPIO_Mode                    GPIO_MODE_AF_PP

/* Pin PE15 */
#define MX_FMC_D12_DA12_Pin                     PE15
#define MX_FMC_D12_DA12_GPIOx                   GPIOE
#define MX_FMC_D12_DA12_GPIO_PuPd               GPIO_NOPULL
#define MX_FMC_D12_DA12_GPIO_Speed_High_Default GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D12_DA12_GPIO_Pin                GPIO_PIN_15
#define MX_FMC_D12_DA12_GPIO_AF                 GPIO_AF12_FMC
#define MX_FMC_D12_DA12_GPIO_Mode               GPIO_MODE_AF_PP

/* Pin PE10 */
#define MX_FMC_D7_DA7_Pin                       PE10
#define MX_FMC_D7_DA7_GPIOx                     GPIOE
#define MX_FMC_D7_DA7_GPIO_PuPd                 GPIO_NOPULL
#define MX_FMC_D7_DA7_GPIO_Speed_High_Default   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D7_DA7_GPIO_Pin                  GPIO_PIN_10
#define MX_FMC_D7_DA7_GPIO_AF                   GPIO_AF12_FMC
#define MX_FMC_D7_DA7_GPIO_Mode                 GPIO_MODE_AF_PP

/* Pin PD15 */
#define MX_FMC_D1_DA1_Pin                       PD15
#define MX_FMC_D1_DA1_GPIOx                     GPIOD
#define MX_FMC_D1_DA1_GPIO_PuPd                 GPIO_NOPULL
#define MX_FMC_D1_DA1_GPIO_Speed_High_Default   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D1_DA1_GPIO_Pin                  GPIO_PIN_15
#define MX_FMC_D1_DA1_GPIO_AF                   GPIO_AF12_FMC
#define MX_FMC_D1_DA1_GPIO_Mode                 GPIO_MODE_AF_PP

/* Pin PD4 */
#define MX_FMC_NOE_Pin                          PD4
#define MX_FMC_NOE_GPIOx                        GPIOD
#define MX_FMC_NOE_GPIO_PuPd                    GPIO_NOPULL
#define MX_FMC_NOE_GPIO_Speed_High_Default      GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_NOE_GPIO_Pin                     GPIO_PIN_4
#define MX_FMC_NOE_GPIO_AF                      GPIO_AF12_FMC
#define MX_FMC_NOE_GPIO_Mode                    GPIO_MODE_AF_PP

/* Pin PE12 */
#define MX_FMC_D9_DA9_Pin                       PE12
#define MX_FMC_D9_DA9_GPIOx                     GPIOE
#define MX_FMC_D9_DA9_GPIO_PuPd                 GPIO_NOPULL
#define MX_FMC_D9_DA9_GPIO_Speed_High_Default   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D9_DA9_GPIO_Pin                  GPIO_PIN_12
#define MX_FMC_D9_DA9_GPIO_AF                   GPIO_AF12_FMC
#define MX_FMC_D9_DA9_GPIO_Mode                 GPIO_MODE_AF_PP

/* Pin PE9 */
#define MX_FMC_D6_DA6_Pin                       PE9
#define MX_FMC_D6_DA6_GPIOx                     GPIOE
#define MX_FMC_D6_DA6_GPIO_PuPd                 GPIO_NOPULL
#define MX_FMC_D6_DA6_GPIO_Speed_High_Default   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D6_DA6_GPIO_Pin                  GPIO_PIN_9
#define MX_FMC_D6_DA6_GPIO_AF                   GPIO_AF12_FMC
#define MX_FMC_D6_DA6_GPIO_Mode                 GPIO_MODE_AF_PP

/* Pin PD0 */
#define MX_FMC_D2_DA2_Pin                       PD0
#define MX_FMC_D2_DA2_GPIOx                     GPIOD
#define MX_FMC_D2_DA2_GPIO_PuPd                 GPIO_NOPULL
#define MX_FMC_D2_DA2_GPIO_Speed_High_Default   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D2_DA2_GPIO_Pin                  GPIO_PIN_0
#define MX_FMC_D2_DA2_GPIO_AF                   GPIO_AF12_FMC
#define MX_FMC_D2_DA2_GPIO_Mode                 GPIO_MODE_AF_PP

/* Pin PD10 */
#define MX_FMC_D15_DA15_Pin                     PD10
#define MX_FMC_D15_DA15_GPIOx                   GPIOD
#define MX_FMC_D15_DA15_GPIO_PuPd               GPIO_NOPULL
#define MX_FMC_D15_DA15_GPIO_Speed_High_Default GPIO_SPEED_FREQ_VERY_HIGH
#define MX_FMC_D15_DA15_GPIO_Pin                GPIO_PIN_10
#define MX_FMC_D15_DA15_GPIO_AF                 GPIO_AF12_FMC
#define MX_FMC_D15_DA15_GPIO_Mode               GPIO_MODE_AF_PP

/*-------------------------------- SYS        --------------------------------*/

#define MX_SYS                                  1

/* GPIO Configuration */

/* Pin PA13 */
#define MX_SYS_JTMS-SWDIO_Pin                   PA13

/* Pin PA14 */
#define MX_SYS_JTCK-SWCLK_Pin                   PA14

/*-------------------------------- USART1     --------------------------------*/

#define MX_USART1                               1

#define MX_USART1_VM                            VM_ASYNC

/* GPIO Configuration */

/* Pin PA9 */
#define MX_USART1_TX_GPIO_ModeDefaultPP         GPIO_MODE_AF_PP
#define MX_USART1_TX_GPIO_Speed                 GPIO_SPEED_FREQ_VERY_HIGH
#define MX_USART1_TX_Pin                        PA9
#define MX_USART1_TX_GPIOx                      GPIOA
#define MX_USART1_TX_GPIO_PuPd                  GPIO_NOPULL
#define MX_USART1_TX_GPIO_Pin                   GPIO_PIN_9
#define MX_USART1_TX_GPIO_AF                    GPIO_AF7_USART1

/* Pin PA10 */
#define MX_USART1_RX_GPIO_ModeDefaultPP         GPIO_MODE_AF_PP
#define MX_USART1_RX_GPIO_Speed                 GPIO_SPEED_FREQ_VERY_HIGH
#define MX_USART1_RX_Pin                        PA10
#define MX_USART1_RX_GPIOx                      GPIOA
#define MX_USART1_RX_GPIO_PuPd                  GPIO_NOPULL
#define MX_USART1_RX_GPIO_Pin                   GPIO_PIN_10
#define MX_USART1_RX_GPIO_AF                    GPIO_AF7_USART1

/* NVIC Configuration */

/* NVIC USART1_IRQn */
#define MX_USART1_IRQn_interruptPremptionPriority 0
#define MX_USART1_IRQn_PriorityGroup            NVIC_PRIORITYGROUP_4
#define MX_USART1_IRQn_Subriority               0

/*-------------------------------- NVIC       --------------------------------*/

#define MX_NVIC                                 1

/*-------------------------------- GPIO       --------------------------------*/

#define MX_GPIO                                 1

/* GPIO Configuration */

/* Pin PC13 */
#define MX_PC13_Pin                             PC13
#define MX_PC13_GPIOx                           GPIOC
#define MX_PC13_GPIO_PuPd                       GPIO_NOPULL
#define MX_PC13_GPIO_Pin                        GPIO_PIN_13
#define MX_PC13_GPIO_ModeDefaultEXTI            GPIO_MODE_IT_RISING

/* Pin PA0/WKUP */
#define MX_PA0_WKUP_Pin                         PA0_WKUP
#define MX_PA0_WKUP_GPIOx                       GPIOA
#define MX_PA0_WKUP_GPIO_PuPd                   GPIO_NOPULL
#define MX_PA0_WKUP_GPIO_Pin                    GPIO_PIN_0
#define MX_PA0_WKUP_GPIO_ModeDefaultEXTI        GPIO_MODE_IT_RISING

/* Pin PH3 */
#define MX_PH3_Pin                              PH3
#define MX_PH3_GPIOx                            GPIOH
#define MX_PH3_GPIO_PuPd                        GPIO_NOPULL
#define MX_PH3_GPIO_Pin                         GPIO_PIN_3
#define MX_PH3_GPIO_ModeDefaultEXTI             GPIO_MODE_IT_RISING

/* Pin PH2 */
#define MX_PH2_Pin                              PH2
#define MX_PH2_GPIOx                            GPIOH
#define MX_PH2_GPIO_PuPd                        GPIO_NOPULL
#define MX_PH2_GPIO_Pin                         GPIO_PIN_2
#define MX_PH2_GPIO_ModeDefaultEXTI             GPIO_MODE_IT_RISING

/* Pin PB1 */
#define MX_PB1_GPIO_Speed                       GPIO_SPEED_FREQ_LOW
#define MX_PB1_Pin                              PB1
#define MX_PB1_GPIOx                            GPIOB
#define MX_PB1_PinState                         GPIO_PIN_RESET
#define MX_PB1_GPIO_PuPd                        GPIO_NOPULL
#define MX_PB1_GPIO_Pin                         GPIO_PIN_1
#define MX_PB1_GPIO_ModeDefaultOutputPP         GPIO_MODE_OUTPUT_PP

/* Pin PB0 */
#define MX_PB0_GPIO_Speed                       GPIO_SPEED_FREQ_LOW
#define MX_PB0_Pin                              PB0
#define MX_PB0_GPIOx                            GPIOB
#define MX_PB0_PinState                         GPIO_PIN_RESET
#define MX_PB0_GPIO_PuPd                        GPIO_NOPULL
#define MX_PB0_GPIO_Pin                         GPIO_PIN_0
#define MX_PB0_GPIO_ModeDefaultOutputPP         GPIO_MODE_OUTPUT_PP

#endif  /* __MX_DEVICE_H */

