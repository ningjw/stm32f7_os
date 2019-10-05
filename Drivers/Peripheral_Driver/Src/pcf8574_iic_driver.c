#include "main.h"

//控制I2C速度的延时
void PCF8574_Delay(void)
{
    delay_us(2);
}

/***************************************************************************************
  * @brief  电容触摸芯片IIC接口初始化
***************************************************************************************/
void PCF8574_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOH_CLK_ENABLE();			//开启GPIOH时钟
    __HAL_RCC_GPIOI_CLK_ENABLE();			//开启GPIOI时钟

    GPIO_Initure.Pin = GPIO_PIN_6;          //PH6
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;   //高速
    HAL_GPIO_Init(GPIOH, &GPIO_Initure);    //初始化

    GPIO_Initure.Pin = GPIO_PIN_3;          //PI3
    HAL_GPIO_Init(GPIOI, &GPIO_Initure);    //初始化
}

/***************************************************************************************
  * @brief  产生IIC起始信号,当SCL为高期间，SDA由高到低的跳变
***************************************************************************************/
void PCF8574_IIC_Start(void)
{
    PCF8574_SDA_OUT();     //sda线输出
    PCF8574_IIC_SCL(1);
    PCF8574_IIC_SDA(1);
    PCF8574_Delay();
    PCF8574_IIC_SDA(0);
}


/***************************************************************************************
  * @brief  产生IIC停止信号,当SCL为高期间，SDA由低到高的跳变
***************************************************************************************/
void PCF8574_IIC_Stop(void)
{
    PCF8574_SDA_OUT();//sda线输出
    PCF8574_IIC_SCL(1);
    PCF8574_Delay();
    PCF8574_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
    PCF8574_Delay();
    PCF8574_IIC_SDA(1);
}

/***************************************************************************************
  * @brief   等待应答信号到来,等待接受数据端拉低SDA
  * @input
  * @return  1，接收应答失败 ; 0，接收应答成功
***************************************************************************************/
uint8_t PCF8574_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    PCF8574_IIC_SDA(1);
    PCF8574_IIC_SCL(1);
    PCF8574_SDA_IN();      //SDA设置为输入
    while(PCF8574_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime > 250) {
            PCF8574_IIC_Stop();
            return 1;
        }
        PCF8574_Delay();
    }
    PCF8574_IIC_SCL(0);
    return 0;
}

/***************************************************************************************
  * @brief  产生ACK应答,在SCL高电平期间，SDA为低电平状态
***************************************************************************************/
void PCF8574_IIC_Ack(void)
{
    PCF8574_IIC_SCL(0);
    PCF8574_Delay();
    PCF8574_SDA_OUT();
    PCF8574_IIC_SDA(0);
    PCF8574_IIC_SCL(1);
    PCF8574_Delay();
    PCF8574_IIC_SCL(0);
}

/***************************************************************************************
  * @brief  不产生ACK应答
***************************************************************************************/
void PCF8574_IIC_NAck(void)
{
    PCF8574_IIC_SCL(0);
    PCF8574_Delay();
    PCF8574_SDA_OUT();
    PCF8574_IIC_SDA(1);
    PCF8574_IIC_SCL(1);
    PCF8574_Delay();
    PCF8574_IIC_SCL(0);
}

/***************************************************************************************
  * @brief   IIC发送一个字节
  * @input
  * @return
***************************************************************************************/
void PCF8574_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    PCF8574_SDA_OUT();
    PCF8574_IIC_SCL(0);//拉低时钟开始数据传输
    PCF8574_Delay();
    for(t = 0; t < 8; t++)
    {
        PCF8574_IIC_SDA((txd & 0x80) >> 7); //先发送高字节
        txd <<= 1;
        PCF8574_IIC_SCL(1);
        PCF8574_Delay();
        PCF8574_IIC_SCL(0);
        PCF8574_Delay();
    }
}

/***************************************************************************************
  * @brief   读1个字节
  * @input   ack=1时，发送ACK，ack=0，发送nACK
  * @return
***************************************************************************************/
uint8_t PCF8574_IIC_Read_Byte(unsigned char ack)
{
    uint8_t i, receive = 0;
    PCF8574_SDA_IN();//SDA设置为输入
    PCF8574_Delay();
    for(i = 0; i < 8; i++ )
    {
        PCF8574_IIC_SCL(0);
        PCF8574_Delay();
        PCF8574_IIC_SCL(1);
        receive <<= 1;
        if(PCF8574_READ_SDA)receive++;
    }
    if (!ack)
        PCF8574_IIC_NAck();//发送nACK
    else
        PCF8574_IIC_Ack(); //发送ACK
    return receive;
}




