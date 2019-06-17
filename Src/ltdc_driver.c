#include "main.h"


uint16_t ltdc_buf[LTDC_WIDTH][LTDC_HEIGHT] __attribute__((at(0xC0000000)));	//定义最大屏分辨率时,LCD所需的帧缓存数组大小

/***************************************************************************************
  * @brief   设置xy坐标
  * @input   Xpos:横坐标   Ypos:纵坐标
  * @return
***************************************************************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{

}

/***************************************************************************************
  * @brief   读取个某点的颜色值
  * @input   x,y:坐标
  * @return  返回值:此点的颜色
***************************************************************************************/
uint32_t LCD_ReadPoint(uint16_t x, uint16_t y)
{
    return *(uint16_t*)((uint32_t)ltdc_buf[0] + 2 * (LTDC_WIDTH * y + x));
}


/***************************************************************************************
  * @brief   画点
  * @input   x,y:坐标  color:颜色
  * @return
***************************************************************************************/
void LCD_DrawPoint(uint16_t x, uint16_t y, uint32_t color)
{
    *(uint16_t*)((uint32_t)ltdc_buf[0] + 2 * (LTDC_WIDTH * y + x)) = color;
}


/***************************************************************************************
  * @brief   在指定区域内填充单个颜色
  * @input   (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
  * @input   color:要填充的颜色
  * @return
***************************************************************************************/
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{
    uint32_t timeout = 0; 
    RCC->AHB1ENR |= 1<<23;			         //使能DM2D时钟
	DMA2D->CR = 3<<16;				         //寄存器到存储器模式
	DMA2D->OPFCCR = LTDC_PIXEL_FORMAT_RGB565;//设置颜色格式
	DMA2D->OOR = LTDC_WIDTH - (ex - sx + 1); //设置行偏移 
	DMA2D->CR &= ~(1<<0);				     //先停止DMA2D
	DMA2D->OMAR = ((uint32_t)ltdc_buf[0] + 2 * (LTDC_WIDTH * sy + sx));//输出存储器地址
	DMA2D->NLR = (ey - sy + 1) | ((ex - sx + 1)<<16);//设定行数寄存器
	DMA2D->OCOLR = color;				     //设定输出颜色寄存器 
	DMA2D->CR |= 1<<0;				         //启动DMA2D
	while((DMA2D->ISR&(1<<1))==0)	         //等待传输完成
	{
		timeout++;
		if(timeout>0X1FFFFF)
            break;	//超时退出
	}  
	DMA2D->IFCR |= 1<<1;				    //清除传输完成标志 	
}

void LTDC_Clear(uint32_t color)
{
	LCD_Fill(0,0,LTDC_WIDTH-1,LTDC_HEIGHT-1,color);
}

void LCD_WriteReg(uint16_t reg, uint16_t value)
{

}

void LCD_ReadId(void)
{
 
}



//设置LCD的自动扫描方向
//注意:其他函数可能会受到此函数设置的影响(尤其是9341),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见lcd.h)
/***************************************************************************************
  * @brief
  * @input
  * @return
***************************************************************************************/
void LCD_Scan_Dir(uint8_t dir)
{

}

//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(uint8_t dir)
{

}

/***************************************************************************************
  * @brief  LCD初始化序列
  * @input
  * @return
***************************************************************************************/
void LCD_InitSequence(void)
{
    
}



