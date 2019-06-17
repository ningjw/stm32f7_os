#ifndef __LTDC_DRIVER
#define __LTDC_DRIVER

#define LTDC_WIDTH  480
#define LTDC_HEIGHT 272



uint32_t LCD_ReadPoint(uint16_t x,uint16_t y);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color);
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);
void LTDC_Clear(uint32_t color);

#endif
