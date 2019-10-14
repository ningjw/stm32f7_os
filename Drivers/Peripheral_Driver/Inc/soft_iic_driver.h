#ifndef __SOFT_IIC_DRIVER_H
#define __SOFT_IIC_DRIVER_H

#include "stdint.h"

#define SOFT_IIC_SDA_GPIO  GPIOH
#define SOFT_IIC_SDA_PIN   GPIO_PIN_5
#define SOFT_IIC_SCL_GPIO      GPIOH
#define SOFT_IIC_SCL_PIN   GPIO_PIN_4

#define SOFT_IIC_SDA_IN()  MODIFY_REG(SOFT_IIC_SDA_GPIO->MODER, \
                                     (GPIO_MODER_MODER0   << (POSITION_VAL(SOFT_IIC_SDA_PIN) * 2U)), \
                                     (GPIO_MODE_INPUT     << (POSITION_VAL(SOFT_IIC_SDA_PIN) * 2U)));
#define SOFT_IIC_SDA_OUT() MODIFY_REG(SOFT_IIC_SDA_GPIO->MODER, \
                                     (GPIO_MODER_MODER0   << (POSITION_VAL(SOFT_IIC_SDA_PIN) * 2U)), \
                                     (GPIO_MODE_OUTPUT_PP << (POSITION_VAL(SOFT_IIC_SDA_PIN) * 2U)));


//IO²Ù×÷º¯Êý
#define SOFT_IIC_SCL(n)     (n ? HAL_GPIO_WritePin(SOFT_IIC_SCL_GPIO,SOFT_IIC_SCL_PIN,GPIO_PIN_SET) \
                               : HAL_GPIO_WritePin(SOFT_IIC_SCL_GPIO,SOFT_IIC_SCL_PIN,GPIO_PIN_RESET)) //SCL
                               
#define SOFT_IIC_SDA(n)     (n ? HAL_GPIO_WritePin(SOFT_IIC_SDA_GPIO,SOFT_IIC_SDA_PIN,GPIO_PIN_SET) \
                               : HAL_GPIO_WritePin(SOFT_IIC_SDA_GPIO,SOFT_IIC_SDA_PIN,GPIO_PIN_RESET)) //SDA 
                               
#define SOFT_IIC_READ_SDA    HAL_GPIO_ReadPin(SOFT_IIC_SDA_GPIO,SOFT_IIC_SDA_PIN)  //ÊäÈëSDA


void SOFT_IIC_Delay(void);
void SOFT_IIC_Init(void);
void SOFT_IIC_Start(void);
void SOFT_IIC_Stop(void);
uint8_t SOFT_IIC_Wait_Ack(void);
void SOFT_IIC_Ack(void);
void SOFT_IIC_NAck(void);
void SOFT_IIC_Send_Byte(uint8_t txd);
uint8_t SOFT_IIC_Read_Byte(unsigned char ack);


#endif
