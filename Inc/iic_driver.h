#ifndef __IIC_DRIVER_H
#define __IIC_DRIVER_H

#include "main.h"

/* 配置SDA线为输入*/
__inline void SDA_CfgInput(void){
    GPIOI->MODER&=~(3<<(3*2));
    GPIOI->MODER|=0<<3*2;
}

/* 配置SDA线为输出*/
__inline void SDA_CfgOutput(void){
    GPIOI->MODER&=~(3<<(3*2));
    GPIOI->MODER|=1<<3*2;
}

/* 设置SCL线*/
__inline void IIC_SetSCL(GPIO_PinState PinState){
    HAL_GPIO_WritePin(GPIOH,GPIO_PIN_6, PinState);
}

/* 设置SDA线*/
__inline void IIC_SetSDA(GPIO_PinState PinState){
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,PinState);
}
/* 获取SDA线引脚状态*/
__inline GPIO_PinState IIC_ReadSDA(void){
    return HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_3);
}

//IO方向设置
#define CT_SDA_IN()  {GPIOI->MODER&=~(3<<(3*2));GPIOI->MODER|=0<<3*2;}	//PI3输入模式
#define CT_SDA_OUT() {GPIOI->MODER&=~(3<<(3*2));GPIOI->MODER|=1<<3*2;} 	//PI3输出模式

//IO操作函数
#define CT_IIC_SCL(n) (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_6,GPIO_PIN_RESET))//SCL
#define CT_IIC_SDA(n) (n?HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_RESET))//SDA	 
#define CT_READ_SDA   HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_3)//输入SDA 
 

//IIC所有操作函数
void CT_IIC_Init(void);                	//初始化IIC的IO口				 
void CT_IIC_Start(void);				//发送IIC开始信号
void CT_IIC_Stop(void);	  				//发送IIC停止信号
void CT_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t CT_IIC_Read_Byte(unsigned char ack);	//IIC读取一个字节
uint8_t CT_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void CT_IIC_Ack(void);					//IIC发送ACK信号
void CT_IIC_NAck(void);					//IIC不发送ACK信号




#endif
