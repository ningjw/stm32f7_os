#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H

#include "stdint.h"
#include "cmsis_armcc.h"

typedef struct
{
    volatile uint16_t reg;
    volatile uint16_t data;
} LCD_TypeDef;
#define LCD_BASE ((uint32_t)(0x60000000 | 0x0007FFFE))
#define LCD      ((LCD_TypeDef *) LCD_BASE)

#define LCD_CMD   (*(uint32_t *)(0x60000000 | 0))
#define LCD_DATA  (*(uint32_t *)(0x60000000 | 0x80000))

#define SET_X_CMD 0x2A00
#define SET_Y_CMD 0x2B00
#define SET_DIR   0x3600
#define MEM_WRITE 0x2C00
#define MEM_READ  0x2E00


//扫描方向定义
#define L2R_U2D  0 		//从左到右,从上到下
#define L2R_D2U  1 		//从左到右,从下到上
#define R2L_U2D  2 		//从右到左,从上到下
#define R2L_D2U  3 		//从右到左,从下到上

#define U2D_L2R  4 		//从上到下,从左到右
#define U2D_R2L  5 		//从上到下,从右到左
#define D2U_L2R  6 		//从下到上,从左到右
#define D2U_R2L  7		//从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

#define LCD_WIDTH   800
#define LCD_LENGTH  480

//LCD重要参数集
typedef struct  
{		 	 
	uint16_t width;			//LCD 宽度
	uint16_t height;			//LCD 高度
	uint16_t id;				//LCD ID
	uint8_t  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t wramcmd;		//开始写gram指令
	uint16_t setxcmd;		//设置x坐标指令
	uint16_t setycmd;		//设置y坐标指令 
}_lcd_dev; 	
extern _lcd_dev lcddev;

/*往指定的寄存器写数据*/
__inline void lcd_w_reg_data(uint16_t reg,uint16_t data)
{
	LCD->reg = reg;		//写入要写的寄存器序号	 
	LCD->data = data;   //写入数据	    		 
}

void LCD_ReadId(void);
void LCD_Display_Dir(uint8_t dir);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
uint32_t LCD_ReadPoint(uint16_t x,uint16_t y);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color);
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);
void LCD_InitSequence(void);

#endif
