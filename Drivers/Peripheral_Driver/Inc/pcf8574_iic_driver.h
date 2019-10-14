#ifndef __PCF8574_IIC_DRIVER_H
#define __PCF8574_IIC_DRIVER_H


#define PCF8574_INT     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)//PCF8574 INT脚
#define PCF8574_ADDR 	0X40	//PCF8574地址(左移了一位)

//PCF8574各个IO的功能
#define BEEP_IO         0		//蜂鸣器控制引脚  	P0
#define AP_INT_IO       1   	//AP3216C中断引脚	P1
#define DCMI_PWDN_IO    2    	//DCMI的电源控制引脚P2
#define USB_PWR_IO      3    	//USB电源控制引脚	P3
#define EX_IO      		4    	//扩展IO,自定义使用 P4
#define MPU_INT_IO      5   	//MPU9250中断引脚	P5
#define RS485_RE_IO     6    	//RS485_RE引脚		P6
#define ETH_RESET_IO    7    	//以太网复位引脚	P7

unsigned char PCF8574_Init(void);
unsigned char  PCF8574_ReadOneByte(void); 
void PCF8574_WriteOneByte(unsigned char  DataToWrite);
void PCF8574_WriteBit(unsigned char  bit,unsigned char  sta);
unsigned char  PCF8574_ReadBit(unsigned char  bit);


#endif
