#ifndef __GT9147_IIC_DRIVER_H
#define __GT9147_IIC_DRIVER_H

#define GT9147_SDA_GPIO  GPIOI
#define GT9147_SDA_PIN   GPIO_PIN_3
#define GT9147_SCL_GPIO  GPIOH
#define GT9147_SCL_PIN   GPIO_PIN_6

#define CT_SDA_IN()  MODIFY_REG(GT9147_SDA_GPIO->MODER, (GPIO_MODER_MODER0 << (POSITION_VAL(GT9147_SDA_PIN) * 2U)), (GPIO_MODE_INPUT << (POSITION_VAL(GT9147_SDA_PIN) * 2U)));
#define CT_SDA_OUT() MODIFY_REG(GT9147_SDA_GPIO->MODER, (GPIO_MODER_MODER0 << (POSITION_VAL(GT9147_SDA_PIN) * 2U)), (GPIO_MODE_OUTPUT_PP << (POSITION_VAL(GT9147_SDA_PIN) * 2U)));

//IO操作函数	 
#define CT_IIC_SCL(n) (n ? HAL_GPIO_WritePin(GT9147_SCL_GPIO, GT9147_SCL_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(GT9147_SCL_GPIO, GT9147_SCL_PIN, GPIO_PIN_RESET))//SCL
#define CT_IIC_SDA(n) (n ? HAL_GPIO_WritePin(GT9147_SDA_GPIO, GT9147_SDA_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(GT9147_SDA_GPIO, GT9147_SDA_PIN, GPIO_PIN_RESET))//SDA	 
#define CT_READ_SDA   HAL_GPIO_ReadPin(GT9147_SDA_GPIO,GT9147_SDA_PIN)//输入SDA 


//IO操作函数	 
#define GT_RST(n)  (n?HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_RESET))//GT9147复位引脚
#define GT_INT      HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_7)  //GT9147中断引脚		
 
//I2C读写命令	
#define GT_CMD_WR 		0X28     	//写命令
#define GT_CMD_RD 		0X29		//读命令

//GT9147 部分寄存器定义 
#define GT_CTRL_REG 	0X8040   	//GT9147控制寄存器
#define GT_CFGS_REG 	0X8047   	//GT9147配置起始地址寄存器
#define GT_CHECK_REG 	0X80FF   	//GT9147校验和寄存器
#define GT_PID_REG 		0X8140   	//GT9147产品ID寄存器

#define GT_GSTID_REG 	0X814E   	//GT9147当前检测到的触摸情况
#define GT_TP1_REG 		0X8150  	//第一个触摸点数据地址
#define GT_TP2_REG 		0X8158		//第二个触摸点数据地址
#define GT_TP3_REG 		0X8160		//第三个触摸点数据地址
#define GT_TP4_REG 		0X8168		//第四个触摸点数据地址
#define GT_TP5_REG 		0X8170		//第五个触摸点数据地址  



extern unsigned short touch_x,touch_y;

unsigned char GT9147_Init(void);


#endif
