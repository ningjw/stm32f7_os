/**
  ******************************************************************************
  * @file   
  * @author  
  * @brief   
  ******************************************************************************
  */ 
  

#include "ltdc_driver.h"


uint16_t ltdc_layer0[LTDC_HEIGHT][LTDC_WIDTH] __attribute__((at(LAYER0_ADDR)));	//定义最大屏分辨率时,LCD所需的帧缓存数组大小
uint16_t ltdc_layer1[LTDC_WIDTH][LTDC_HEIGHT] __attribute__((at(LAYER1_ADDR)));	//定义最大屏分辨率时,LCD所需的帧缓存数组大小

uint32_t *ltdc_framebuf[2];					//LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域
/***************************************************************************************
  * @brief   在指定区域内填充单个颜色
  * @input   (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
  * @input   color:要填充的颜色
  * @return
***************************************************************************************/
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{
    uint32_t timeout = 0;
    uint32_t addr = ((uint32_t)ltdc_layer0[0] + 2 * (LTDC_WIDTH * sy + sx));//输出存储器地址
    uint16_t offline = LTDC_WIDTH - (ex - sx + 1); //设置行偏移 
    
    RCC->AHB1ENR |= RCC_AHB1LPENR_DMA2DLPEN; //使能DM2D时钟
	DMA2D->CR = DMA2D_CR_MODE;				 //寄存器到存储器模式
	DMA2D->OPFCCR = LTDC_PIXEL_FORMAT_RGB565;//设置颜色格式
	DMA2D->OOR = offline;
	DMA2D->CR &= ~(DMA2D_CR_START);			//先停止DMA2D
	DMA2D->OMAR = addr;
	DMA2D->NLR = (ey - sy + 1) | ((ex - sx + 1)<<16);//设定行数寄存器
	DMA2D->OCOLR = color;				     //设定输出颜色寄存器 

	DMA2D->CR |= DMA2D_CR_START;		    //启动DMA2D
	while((DMA2D->ISR & DMA2D_ISR_TCIF) == 0)	         //等待传输完成
	{
		timeout++;
		if(timeout>0X1FFFFF)
            break;	//超时退出
	}
	DMA2D->IFCR |= DMA2D_IFCR_CTCIF;				    //清除传输完成标志 	
}

/***************************************************************************************
  * @brief   清屏
  * @input   x,y:坐标  color:颜色
  * @return
***************************************************************************************/
void LTDC_Clear(uint32_t color)
{
	LCD_Fill(0, 0, LTDC_WIDTH-1, LTDC_HEIGHT-1, color);
}

