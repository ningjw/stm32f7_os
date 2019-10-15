#include "mpu9250_iic_driver.h"
#include "soft_iic_driver.h"
#include "utility.h"


/***************************************************************************************
  * @brief   初始化MPU9250
  * @input   
  * @return  返回值:0,成功
***************************************************************************************/
uint8_t MPU9250_Init(void)
{
    uint8_t res=0;
    SOFT_IIC_Init();     //初始化IIC总线
    MPU_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X80);//复位MPU9250
    delay_ms(100);  //延时100ms
    MPU_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X00);  //唤醒MPU9250
    MPU_Set_Gyro_Fsr(3);					        	  //陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);					       	 	  //加速度传感器,±2g
    MPU_Set_Rate(50);						       	 	  //设置采样率50Hz
    MPU_Write_Byte(MPU9250_ADDR, MPU_INT_EN_REG,    0X00);//关闭所有中断
	MPU_Write_Byte(MPU9250_ADDR, MPU_USER_CTRL_REG, 0X00);//I2C主模式关闭
	MPU_Write_Byte(MPU9250_ADDR, MPU_FIFO_EN_REG,   0X00);//关闭FIFO
	MPU_Write_Byte(MPU9250_ADDR, MPU_INTBP_CFG_REG, 0X82);//INT引脚低电平有效，开启bypass模式，可以直接读取磁力计
    res = MPU_Read_Byte(MPU9250_ADDR, MPU_DEVICE_ID_REG); //读取MPU6500的ID
    if(res == MPU6500_ID1 || res == MPU6500_ID2) 		  //器件ID正确
    {
        MPU_Write_Byte(MPU9250_ADDR, MPU_PWR_MGMT1_REG, 0X01);  	//设置CLKSEL,PLL X轴为参考
        MPU_Write_Byte(MPU9250_ADDR, MPU_PWR_MGMT2_REG, 0X00);  	//加速度与陀螺仪都工作
		MPU_Set_Rate(50);						       			//设置采样率为50Hz   
    }else 
        return 1;
 
    res = MPU_Read_Byte(AK8963_ADDR, MAG_WIA);    			//读取AK8963 ID   
    if(res == AK8963_ID)
    {
        MPU_Write_Byte(AK8963_ADDR,MAG_CNTL2,0X01);		//复位AK8963
		delay_ms(50);
        MPU_Write_Byte(AK8963_ADDR,MAG_CNTL1,0X11);		//设置AK8963为单次测量
    }else 
        return 1;

    return 0;
}


/***************************************************************************************
  * @brief   设置MPU9250陀螺仪传感器满量程范围
  * @input   fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
  * @return  返回值:0,设置成功
***************************************************************************************/
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU9250_ADDR,MPU_GYRO_CFG_REG,(fsr<<3)|3);//设置陀螺仪满量程范围  
}


/***************************************************************************************
  * @brief   设置MPU9250加速度传感器满量程范围
  * @input   fsr:0,±2g;1,±4g;2,±8g;3,±16g
  * @return  返回值:0,设置成功
***************************************************************************************/
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU9250_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}


/***************************************************************************************
  * @brief   设置MPU9250的数字低通滤波器
  * @input   lpf:数字低通滤波频率(Hz)
  * @return  返回值:0,设置成功
***************************************************************************************/
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU9250_ADDR,MPU_CFG_REG,data);//设置数字低通滤波器  
}


/***************************************************************************************
  * @brief   设置MPU9250的采样率(假定Fs=1KHz)
  * @input   rate:4~1000(Hz)
  * @return  返回值:0,设置成功
***************************************************************************************/
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU9250_ADDR,MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}


/***************************************************************************************
  * @brief   得到温度值
  * @input   
  * @return  返回值:温度值(扩大了100倍)
***************************************************************************************/
short MPU_Get_Temperature(void)
{
    uint8_t buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(MPU9250_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((uint16_t)buf[0]<<8)|buf[1];  
    temp=21+((double)raw)/333.87;  
    return temp*100;;
}


/***************************************************************************************
  * @brief   得到陀螺仪值(原始值)
  * @input   gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
  * @return  返回值:0,成功
***************************************************************************************/
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    uint8_t buf[6],res; 
	res=MPU_Read_Len(MPU9250_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((uint16_t)buf[0]<<8)|buf[1];  
		*gy=((uint16_t)buf[2]<<8)|buf[3];  
		*gz=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


/***************************************************************************************
  * @brief   得到加速度值(原始值)
  * @input   gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
  * @return  返回值:0,成功
***************************************************************************************/
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    uint8_t buf[6],res;  
	res=MPU_Read_Len(MPU9250_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((uint16_t)buf[0]<<8)|buf[1];  
		*ay=((uint16_t)buf[2]<<8)|buf[3];  
		*az=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


/***************************************************************************************
  * @brief   得到磁力计值(原始值)
  * @input   mx,my,mz:磁力计x,y,z轴的原始读数(带符号)
  * @return  返回值:0,成功
***************************************************************************************/
uint8_t MPU_Get_Magnetometer(short *mx,short *my,short *mz)
{
    uint8_t buf[6],res;  
 	res=MPU_Read_Len(AK8963_ADDR,MAG_XOUT_L,6,buf);
	if(res==0)
	{
		*mx=((uint16_t)buf[1]<<8)|buf[0];  
		*my=((uint16_t)buf[3]<<8)|buf[2];  
		*mz=((uint16_t)buf[5]<<8)|buf[4];
	} 	 
	MPU_Write_Byte(AK8963_ADDR,MAG_CNTL1,0X11); //AK8963每次读完以后都需要重新设置为单次测量模式
    return res;;
}


/***************************************************************************************
  * @brief   IIC连续写
  * @input   addr:器件地址 
             reg:寄存器地址
             len:写入长度
             buf:数据区
  * @return  返回值:0,正常
***************************************************************************************/
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    uint8_t i;
    SOFT_IIC_Start();
    SOFT_IIC_Send_Byte((addr<<1)|0); //发送器件地址+写命令
    if(SOFT_IIC_Wait_Ack())          //等待应答
    {
        SOFT_IIC_Stop();
        return 1;
    }
    SOFT_IIC_Send_Byte(reg);         //写寄存器地址
    SOFT_IIC_Wait_Ack();             //等待应答
    for(i=0;i<len;i++)
    {
        SOFT_IIC_Send_Byte(buf[i]);  //发送数据
        if(SOFT_IIC_Wait_Ack())      //等待ACK
        {
            SOFT_IIC_Stop();
            return 1;
        }
    }
    SOFT_IIC_Stop();
    return 0;
} 


/***************************************************************************************
  * @brief   IIC连续读
  * @input   addr:器件地址
             reg:要读取的寄存器地址
             len:要读取的长度
             buf:读取到的数据存储区
  * @return  返回值:0,正常
***************************************************************************************/
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
    SOFT_IIC_Start();
    SOFT_IIC_Send_Byte((addr<<1)|0); //发送器件地址+写命令
    if(SOFT_IIC_Wait_Ack())          //等待应答
    {
        SOFT_IIC_Stop();
        return 1;
    }
    SOFT_IIC_Send_Byte(reg);         //写寄存器地址
    SOFT_IIC_Wait_Ack();             //等待应答
	SOFT_IIC_Start();                
    SOFT_IIC_Send_Byte((addr<<1)|1); //发送器件地址+读命令
    SOFT_IIC_Wait_Ack();             //等待应答
    while(len)
    {
        if(len==1)*buf=SOFT_IIC_Read_Byte(0);//读数据,发送nACK 
		else *buf=SOFT_IIC_Read_Byte(1);		//读数据,发送ACK  
		len--;
		buf++;  
    }
    SOFT_IIC_Stop();                 //产生一个停止条件
    return 0;       
}


/***************************************************************************************
  * @brief   IIC写一个字节 
  * @input   devaddr:器件IIC地址
             reg:寄存器地址
             data:数据
  * @return  返回值:0,正常
***************************************************************************************/
uint8_t MPU_Write_Byte(uint8_t addr,uint8_t reg,uint8_t data)
{
    SOFT_IIC_Start();
    SOFT_IIC_Send_Byte((addr<<1)|0); //发送器件地址+写命令
    if(SOFT_IIC_Wait_Ack())          //等待应答
    {
        SOFT_IIC_Stop();
        return 1;
    }
    SOFT_IIC_Send_Byte(reg);         //写寄存器地址
    SOFT_IIC_Wait_Ack();             //等待应答
    SOFT_IIC_Send_Byte(data);        //发送数据
    if(SOFT_IIC_Wait_Ack())          //等待ACK
    {
        SOFT_IIC_Stop();
        return 1;
    }
    SOFT_IIC_Stop();
    return 0;
}


/***************************************************************************************
  * @brief   IIC读一个字节 
  * @input   reg:寄存器地址 
  * @return  返回值:读到的数据
***************************************************************************************/
uint8_t MPU_Read_Byte(uint8_t addr,uint8_t reg)
{
    uint8_t res;
    SOFT_IIC_Start();
    SOFT_IIC_Send_Byte((addr<<1)|0); //发送器件地址+写命令
    SOFT_IIC_Wait_Ack();             //等待应答
    SOFT_IIC_Send_Byte(reg);         //写寄存器地址
    SOFT_IIC_Wait_Ack();             //等待应答
	SOFT_IIC_Start();                
    SOFT_IIC_Send_Byte((addr<<1)|1); //发送器件地址+读命令
    SOFT_IIC_Wait_Ack();             //等待应答
    res=SOFT_IIC_Read_Byte(0);		//读数据,发送nACK  
    SOFT_IIC_Stop();                 //产生一个停止条件
    return res;  
}


