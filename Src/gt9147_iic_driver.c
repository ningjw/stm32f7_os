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

const uint16_t GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};

//GT9147配置参数表:第一个字节为版本号(0X60),必须保证新的版本号大于等于GT9147内部flash原有版本号,才会更新配置.
const uint8_t GT9147_CFG_TBL[] = { 
	0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
	0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF,
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
    
    GPIO_Initure.Pin=GPIO_PIN_6;            //PH6
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);     //初始化
	
    GPIO_Initure.Pin=GPIO_PIN_3;            //PI3
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //初始化
}

/***************************************************************************************
  * @brief  产生IIC起始信号
***************************************************************************************/
void CT_IIC_Start(void)
{
	CT_SDA_OUT();     //sda线输出
	CT_IIC_SDA(1);	  	  
	CT_IIC_SCL(1);
	delay_us(30);
 	CT_IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	CT_Delay();
	CT_IIC_SCL(0);//钳住I2C总线，准备发送或接收数据 
}


/***************************************************************************************
  * @brief  产生IIC停止信号
***************************************************************************************/
void CT_IIC_Stop(void)
{
	CT_SDA_OUT();//sda线输出
	CT_IIC_SCL(1);
	delay_us(30);
	CT_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
	CT_Delay();
	CT_IIC_SDA(1);//发送I2C总线结束信号  
}
     
/***************************************************************************************
  * @brief   等待应答信号到来
  * @input   
  * @return  1，接收应答失败 ; 0，接收应答成功
***************************************************************************************/
uint8_t CT_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	CT_SDA_IN();      //SDA设置为输入  
	CT_IIC_SDA(1);	   
	CT_IIC_SCL(1);
	CT_Delay();
	while(CT_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CT_IIC_Stop();
			return 1;
		} 
		CT_Delay();
	}
	CT_IIC_SCL(0);//时钟输出0 	   
	return 0;  
}

/***************************************************************************************
  * @brief  产生ACK应答
***************************************************************************************/
void CT_IIC_Ack(void)
{
	CT_IIC_SCL(0);
	CT_SDA_OUT();
	CT_Delay();
	CT_IIC_SDA(0);
	CT_Delay();
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
	CT_SDA_OUT();
	CT_Delay();
	CT_IIC_SDA(1);
	CT_Delay();
	CT_IIC_SCL(1);
	CT_Delay();
	CT_IIC_SCL(0);
}					 				     

/***************************************************************************************
  * @brief   IIC发送一个字节
  * @input   
  * @return  1，有应答 ;  0，无应答		
***************************************************************************************/
void CT_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	CT_SDA_OUT(); 	    
    CT_IIC_SCL(0);//拉低时钟开始数据传输
	CT_Delay();
	for(t=0;t<8;t++)
    {
        CT_IIC_SDA((txd&0x80)>>7);
        txd<<=1; 	      
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
	uint8_t i,receive=0;
 	CT_SDA_IN();//SDA设置为输入
	delay_us(30);
	for(i=0;i<8;i++ )
	{ 
		CT_IIC_SCL(0); 	    	   
		CT_Delay();
		CT_IIC_SCL(1);	 
		receive<<=1;
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
uint8_t GT9147_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i;
	uint8_t ret=0;
	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   	//发送写命令 	 
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte(reg>>8);   	//发送高8位地址
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
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
void GT9147_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   //发送写命令 	 
	CT_IIC_Wait_Ack();
 	CT_IIC_Send_Byte(reg>>8);   	//发送高8位地址
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(GT_CMD_RD);   //发送读命令		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
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
	uint8_t i=0;
	buf[0]=0;
	buf[1]=mode;	//是否写入到GT9147 FLASH?  即是否掉电保存
	for(i=0;i<sizeof(GT9147_CFG_TBL);i++)
        buf[0]+=GT9147_CFG_TBL[i];//计算校验和
    buf[0]=(~buf[0])+1;
	GT9147_WR_Reg(GT_CFGS_REG,(uint8_t*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//发送寄存器配置
	GT9147_WR_Reg(GT_CHECK_REG,buf,2);//写入校验和,和配置更新标记
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
    
    __HAL_RCC_GPIOH_CLK_ENABLE();			//开启GPIOH时钟
    __HAL_RCC_GPIOI_CLK_ENABLE();			//开启GPIOI时钟
                
    //PH7
    GPIO_Initure.Pin=GPIO_PIN_7;            //PH7
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);     //初始化
    
    //PI8
    GPIO_Initure.Pin=GPIO_PIN_8;            //PI8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //初始化
	CT_IIC_Init();      	//初始化电容屏的I2C总线  
	GT_RST(0);				//复位
	delay_ms(10);
 	GT_RST(1);				//释放复位		    
	delay_ms(10); 
    GPIO_Initure.Pin=GPIO_PIN_7;            //PH7
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉，浮空输入
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);     //初始化
	delay_ms(100);  
	GT9147_RD_Reg(GT_PID_REG,temp,4);//读取产品ID
	temp[4]=0;
	printf("CTP ID:%s\r\n",temp);	//打印ID
    /*Configure GPIO pin : PH7 */

	if(strcmp((char*)temp,"9147")==0)//ID==9147
	{
		temp[0]=0X02;			
		GT9147_WR_Reg(GT_CTRL_REG,temp,1);//软复位GT9147
		GT9147_RD_Reg(GT_CFGS_REG,temp,1);//读取GT_CFGS_REG寄存器
//		if(temp[0]<0X60)//默认版本比较低,需要更新flash配置
		{
			printf("Default Ver:%d\r\n",temp[0]);
            GT9147_Send_Cfg(1);//更新并保存配置
		}
		delay_ms(10);
		temp[0]=0X00;	 
		GT9147_WR_Reg(GT_CTRL_REG,temp,1);//结束复位   
		return 0;
	}
	return 1;
}

/***************************************************************************************
  * @brief   PH7中断, 触摸中断
  * @input   
  * @return  
***************************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint8_t buf[4];
    uint8_t i = 0, mode = 0, temp = 0;
    uint16_t x[5],y[5];
    if(GPIO_Pin == GPIO_PIN_7){//读取坐标, 否则会一直有INT脉冲
        GT9147_RD_Reg(GT_GSTID_REG,&mode,1);	//读取触摸点的状态  
        if(mode&0X80&&((mode&0XF)<6))
		{
			temp=0;
			GT9147_WR_Reg(GT_GSTID_REG, &temp, 1);//清标志 		
		}
        if((mode&0XF)&&((mode&0XF)<6))
		{
            GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//读取XY坐标值
            temp = 0XFF << (mode&0XF);		//将点的个数转换为1的位数,匹配tp_dev.sta定义 
            for(i=0;i<5;i++)
			{
                GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//读取XY坐标值
                x[i] = (((uint16_t)buf[1]<<8)+buf[0]);
                y[i] = (((uint16_t)buf[3]<<8)+buf[2]);
            }
            for(i=0;i<5;i++)
			{
                printf("x=%d, y=%d\r\n",x[i],y[i]);
            }
        }
    }
}


