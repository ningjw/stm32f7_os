/**
  ******************************************************************************
  * @file   
  * @author  
  * @brief   
  ******************************************************************************
  */ 
#include "soft_iic_driver.h"
#include "utility.h"
#include "stm32f7xx_hal.h"

//控制I2C速度的延时
void SOFT_IIC_Delay(void)
{
    delay_us(2);
}

/***************************************************************************************
  * @brief  电容触摸芯片IIC接口初始化
***************************************************************************************/
void SOFT_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOH_CLK_ENABLE();   //使能GPIOH时钟
    
    //PH4,5初始化设置
    GPIO_Initure.Pin = SOFT_IIC_SCL_PIN | SOFT_IIC_SDA_PIN;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed = GPIO_SPEED_FAST;     //快速
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
    SOFT_IIC_SDA(1);
    SOFT_IIC_SCL(1);  
}

/***************************************************************************************
  * @brief  产生IIC起始信号,当SCL为高期间，SDA由高到低的跳变
***************************************************************************************/
void SOFT_IIC_Start(void)
{
    SOFT_IIC_SDA_OUT();     //sda线输出
    SOFT_IIC_SCL(1);
    SOFT_IIC_SDA(1);
    SOFT_IIC_Delay();
    SOFT_IIC_SDA(0);
}


/***************************************************************************************
  * @brief  产生IIC停止信号,当SCL为高期间，SDA由低到高的跳变
***************************************************************************************/
void SOFT_IIC_Stop(void)
{
    SOFT_IIC_SDA_OUT();//sda线输出
    SOFT_IIC_SCL(1);
    SOFT_IIC_Delay();
    SOFT_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
    SOFT_IIC_Delay();
    SOFT_IIC_SDA(1);
}

/***************************************************************************************
  * @brief   等待应答信号到来,等待接受数据端拉低SDA
  * @input
  * @return  1，接收应答失败 ; 0，接收应答成功
***************************************************************************************/
uint8_t SOFT_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SOFT_IIC_SDA(1);
    SOFT_IIC_SCL(1);
    SOFT_IIC_SDA_IN();      //SDA设置为输入
    while(SOFT_IIC_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime > 250) {
            SOFT_IIC_Stop();
            return 1;
        }
        SOFT_IIC_Delay();
    }
    SOFT_IIC_SCL(0);
    return 0;
}

/***************************************************************************************
  * @brief  产生ACK应答,在SCL高电平期间，SDA为低电平状态
***************************************************************************************/
void SOFT_IIC_Ack(void)
{
    SOFT_IIC_SCL(0);
    SOFT_IIC_Delay();
    SOFT_IIC_SDA_OUT();
    SOFT_IIC_SDA(0);
    SOFT_IIC_SCL(1);
    SOFT_IIC_Delay();
    SOFT_IIC_SCL(0);
}

/***************************************************************************************
  * @brief  不产生ACK应答
***************************************************************************************/
void SOFT_IIC_NAck(void)
{
    SOFT_IIC_SCL(0);
    SOFT_IIC_Delay();
    SOFT_IIC_SDA_OUT();
    SOFT_IIC_SDA(1);
    SOFT_IIC_SCL(1);
    SOFT_IIC_Delay();
    SOFT_IIC_SCL(0);
}

/***************************************************************************************
  * @brief   IIC发送一个字节
  * @input
  * @return
***************************************************************************************/
void SOFT_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SOFT_IIC_SDA_OUT();
    SOFT_IIC_SCL(0);//拉低时钟开始数据传输
    SOFT_IIC_Delay();
    for(t = 0; t < 8; t++)
    {
        SOFT_IIC_SDA((txd & 0x80) >> 7); //先发送高字节
        txd <<= 1;
        SOFT_IIC_SCL(1);
        SOFT_IIC_Delay();
        SOFT_IIC_SCL(0);
        SOFT_IIC_Delay();
    }
}

/***************************************************************************************
  * @brief   读1个字节
  * @input   ack=1时，发送ACK，ack=0，发送nACK
  * @return
***************************************************************************************/
uint8_t SOFT_IIC_Read_Byte(unsigned char ack)
{
    uint8_t i, receive = 0;
    SOFT_IIC_SDA_IN();//SDA设置为输入
    SOFT_IIC_Delay();
    for(i = 0; i < 8; i++ )
    {
        SOFT_IIC_SCL(0);
        SOFT_IIC_Delay();
        SOFT_IIC_SCL(1);
        receive <<= 1;
        if(SOFT_IIC_READ_SDA)receive++;
    }
    if (!ack)
        SOFT_IIC_NAck();//发送nACK
    else
        SOFT_IIC_Ack(); //发送ACK
    return receive;
}
