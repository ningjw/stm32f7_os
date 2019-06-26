/**
  ******************************************************************************
  * @file
  * @author
  * @brief   GT9147电容触摸屏驱动,使用软件模拟IIC方式控制
  SCL -----> PH6
  SDA -----> PI3
  RST -----> PI8
  INT -----> PH7
  ******************************************************************************
  */
#include "main.h"

const uint16_t GT9147_TPX_TBL[5] = {GT_TP1_REG, GT_TP2_REG, GT_TP3_REG, GT_TP4_REG, GT_TP5_REG};

//GT9147配置参数表:第一个字节为版本号(0X60),必须保证新的版本号大于等于GT9147内部flash原有版本号,才会更新配置.
const uint8_t GT9147_CFG_TBL[] =
{
    0x60, 0xe0, 0x01, 0x10, 0x01, 0x05, 0x0C, 0x00, 0x01, 0x08,
    0x28, 0x05, 0x50, 0x32, 0x03, 0x5F, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x89, 0x28, 0x0a,
    0x17, 0x15, 0x31, 0x0d, 0x00, 0x00, 0x02, 0x9b, 0x03, 0x25,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00,
    0x00, 0x0f, 0x94, 0x94, 0xc5, 0x02, 0x07, 0x00, 0x00, 0x04,
    0x8d, 0x13, 0x00, 0x5c, 0x1e, 0x00, 0x3c, 0x30, 0x00, 0x29,
    0x4c, 0x00, 0x1e, 0x78, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12, 0x14, 0x16,
    0x18, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x00, 0x02, 0x04, 0x05, 0x06, 0x08, 0x0a, 0x0c,
    0x0e, 0x1d, 0x1e, 0x1f, 0x20, 0x22, 0x24, 0x28, 0x29, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
};


//控制I2C速度的延时
void CT_Delay(void)
{
    delay_us(2);
}

/***************************************************************************************
  * @brief  电容触摸芯片IIC接口初始化
***************************************************************************************/
void CT_IIC_Init(void)
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
void CT_IIC_Start(void)
{
    CT_SDA_OUT();     //sda线输出
    CT_IIC_SCL(1);
    CT_IIC_SDA(1);
    CT_Delay();
    CT_IIC_SDA(0);
}


/***************************************************************************************
  * @brief  产生IIC停止信号,当SCL为高期间，SDA由低到高的跳变
***************************************************************************************/
void CT_IIC_Stop(void)
{
    CT_SDA_OUT();//sda线输出
    CT_IIC_SCL(1);
    CT_Delay();
    CT_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
    CT_Delay();
    CT_IIC_SDA(1);
}

/***************************************************************************************
  * @brief   等待应答信号到来,等待接受数据端拉低SDA
  * @input
  * @return  1，接收应答失败 ; 0，接收应答成功
***************************************************************************************/
uint8_t CT_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    CT_IIC_SDA(1);
    CT_IIC_SCL(1);
    CT_SDA_IN();      //SDA设置为输入
    while(CT_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime > 250) {
            CT_IIC_Stop();
            return 1;
        }
        CT_Delay();
    }
    CT_IIC_SCL(0);
    return 0;
}

/***************************************************************************************
  * @brief  产生ACK应答,在SCL高电平期间，SDA为低电平状态
***************************************************************************************/
void CT_IIC_Ack(void)
{
    CT_IIC_SCL(0);
    CT_Delay();
    CT_SDA_OUT();
    CT_IIC_SDA(0);
    CT_IIC_SCL(1);
    CT_Delay();
    CT_IIC_SCL(0);
}

/***************************************************************************************
  * @brief  不产生ACK应答
***************************************************************************************/
void CT_IIC_NAck(void)
{
    CT_IIC_SCL(0);
    CT_Delay();
    CT_SDA_OUT();
    CT_IIC_SDA(1);
    CT_IIC_SCL(1);
    CT_Delay();
    CT_IIC_SCL(0);
}

/***************************************************************************************
  * @brief   IIC发送一个字节
  * @input
  * @return
***************************************************************************************/
void CT_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    CT_SDA_OUT();
    CT_IIC_SCL(0);//拉低时钟开始数据传输
    CT_Delay();
    for(t = 0; t < 8; t++)
    {
        CT_IIC_SDA((txd & 0x80) >> 7); //先发送高字节
        txd <<= 1;
        CT_IIC_SCL(1);
        CT_Delay();
        CT_IIC_SCL(0);
        CT_Delay();
    }
}

/***************************************************************************************
  * @brief   读1个字节
  * @input   ack=1时，发送ACK，ack=0，发送nACK
  * @return
***************************************************************************************/
uint8_t CT_IIC_Read_Byte(unsigned char ack)
{
    uint8_t i, receive = 0;
    CT_SDA_IN();//SDA设置为输入
    CT_Delay();
    for(i = 0; i < 8; i++ )
    {
        CT_IIC_SCL(0);
        CT_Delay();
        CT_IIC_SCL(1);
        receive <<= 1;
        if(CT_READ_SDA)receive++;
    }
    if (!ack)
        CT_IIC_NAck();//发送nACK
    else
        CT_IIC_Ack(); //发送ACK
    return receive;
}


/***************************************************************************************
  * @brief   向GT9147写入一次数据
  * @input   reg:起始寄存器地址;
             buf:数据缓缓存区
             len:写数据长度
  * @return  0,成功; 1,失败.
***************************************************************************************/
uint8_t GT9147_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    uint8_t ret = 0;
    CT_IIC_Start();
    CT_IIC_Send_Byte(GT_CMD_WR);   	//发送写命令
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg >> 8);   	//发送高8位地址
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg & 0XFF);   	//发送低8位地址
    CT_IIC_Wait_Ack();
    for(i = 0; i < len; i++)
    {
        CT_IIC_Send_Byte(buf[i]);  	//发数据
        ret = CT_IIC_Wait_Ack();
        if(ret)break;
    }
    CT_IIC_Stop();					//产生一个停止条件
    return ret;
}


/***************************************************************************************
  * @brief   从GT9147读出一次数据
  * @input   reg:起始寄存器地址
             buf:数据缓缓存区
             len:读数据长度
  * @return
***************************************************************************************/
void GT9147_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    CT_IIC_Start();
    CT_IIC_Send_Byte(GT_CMD_WR);   //发送写命令
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg >> 8);   	//发送高8位地址
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg & 0XFF);   	//发送低8位地址
    CT_IIC_Wait_Ack();
    CT_IIC_Start();
    CT_IIC_Send_Byte(GT_CMD_RD);   //发送读命令
    CT_IIC_Wait_Ack();
    for(i = 0; i < len; i++)
    {
        buf[i] = CT_IIC_Read_Byte(i == (len - 1) ? 0 : 1); //发数据
    }
    CT_IIC_Stop();//产生一个停止条件
}

/***************************************************************************************
  * @brief   发送GT9147配置参数
  * @input   0,参数不保存到flash;1,参数保存到flash
  * @return
***************************************************************************************/
uint8_t GT9147_Send_Cfg(uint8_t mode)
{
    uint8_t buf[2];
    uint8_t i = 0;
    buf[0] = 0;
    buf[1] = mode;	//是否写入到GT9147 FLASH?  即是否掉电保存
    for(i = 0; i < sizeof(GT9147_CFG_TBL); i++)
        buf[0] += GT9147_CFG_TBL[i]; //计算校验和
    buf[0] = (~buf[0]) + 1;
    GT9147_WR_Reg(GT_CFGS_REG, (uint8_t*)GT9147_CFG_TBL, sizeof(GT9147_CFG_TBL)); //发送寄存器配置
    GT9147_WR_Reg(GT_CHECK_REG, buf, 2); //写入校验和,和配置更新标记
    return 0;
}


/***************************************************************************************
  * @brief   初始化GT9147触摸屏
  * @input
  * @return  0,初始化成功;1,初始化失败
***************************************************************************************/
uint8_t GT9147_Init(void)
{
    uint8_t temp[5];
    GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin = GPIO_PIN_7;          //PH7
    GPIO_Initure.Mode = GPIO_MODE_INPUT; //输出
    GPIO_Initure.Pull = GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;   //高速
    HAL_GPIO_Init(GPIOH, &GPIO_Initure);    //初始化
    GPIO_Initure.Pin = GPIO_PIN_8;          //PI8
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
    HAL_GPIO_Init(GPIOI, &GPIO_Initure);    //初始化
    CT_IIC_Init();      	//初始化电容屏的I2C总线

    GT_RST(0);				//复位
    delay_ms(1);           //INT引脚电平维持100us以上
    GT_RST(1);				//释放复位
    delay_ms(10);           //释放复位后维持5ms以上，设置INT为悬浮输入
    GPIO_Initure.Pin = GPIO_PIN_7;
    GPIO_Initure.Mode = GPIO_MODE_IT_RISING;
    GPIO_Initure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOH, &GPIO_Initure);    //初始化
    delay_ms(60);

    GT9147_RD_Reg(GT_PID_REG, temp, 4); //读取产品ID
    temp[4] = 0;
    printf("CTP ID:0x%s\r\n", temp);	//打印ID

    GT9147_Send_Cfg(1);//更新并保存配置

    return 1;
}

/***************************************************************************************
  * @brief   PH7中断, 触摸中断
  * @input
  * @return
***************************************************************************************/
void     LCD_LL_FillRect(int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint8_t buf[4];
    uint8_t i = 0, mode = 0, temp = 0;
    uint16_t x[5], y[5];
    if(GPIO_Pin == GPIO_PIN_7) { //读取坐标, 否则会一直有INT脉冲
        GT9147_RD_Reg(GT_GSTID_REG, &mode, 1);	//读取触摸点的状态
        if(mode & 0X80 && ((mode & 0XF) < 6)) //有坐标可读取
        {
            temp = 0;
            GT9147_WR_Reg(GT_GSTID_REG, &temp, 1);//清标志
        }
        if( (mode & 0xF) && ((mode & 0xF) < 6)) //判断触摸点个数
        {
            for(i = 0; i < (mode & 0xF); i++)
            {
                GT9147_RD_Reg(GT9147_TPX_TBL[i], buf, 4);	//读取XY坐标值
                x[i] = (((uint16_t)buf[1] << 8) + buf[0]);
                y[i] = (((uint16_t)buf[3] << 8) + buf[2]);
            }
            for(i = 0; i < (mode & 0xF); i++)
            {
                LCD_DrawPoint(x[i], y[i], i * 0xaa + 0xff);
//                printf("x=%d, y=%d\r\n",x[i],y[i]);
            }
        }
    }
}


