#include "main.h"


/***************************************************************************************
  * @brief   设置xy坐标
  * @input   Xpos:横坐标   Ypos:纵坐标
  * @return
***************************************************************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    LCD->reg = SET_X_CMD; //写x坐标
    LCD->data = (Xpos >> 8);
    LCD->data = (Xpos & 0xFF);

    LCD->reg = SET_Y_CMD; //写y坐标
    LCD->data = (Ypos >> 8);
    LCD->data = (Ypos & 0xFF);
}

/***************************************************************************************
  * @brief   读取个某点的颜色值
  * @input   x,y:坐标
  * @return  返回值:此点的颜色
***************************************************************************************/
uint32_t LCD_ReadPoint(uint16_t x, uint16_t y)
{
    uint16_t r = 0, g = 0, b = 0;
    if(x >= LCD_WIDTH || y >= LCD_LENGTH)
        return 0;	//超过了范围,直接返回

    LCD_SetCursor(x, y);

    LCD->reg = MEM_READ;
    r = LCD->data;		//dummy Read
    __NOP;
    r = LCD->data;  	//实际坐标颜色
    g = (r & 0XFF) << 8;		//对于9341,第一次读取的是RG的值,R在前,G在后,各占8位
    b = LCD->data;

    return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11)); //ILI9341需要公式转换一下
}


/***************************************************************************************
  * @brief   画点
  * @input   x,y:坐标  color:颜色
  * @return
***************************************************************************************/
void LCD_DrawPoint(uint16_t x, uint16_t y, uint32_t color)
{
    LCD_SetCursor(x, y);

    LCD->reg = MEM_WRITE;
    LCD->data = color;
}


/***************************************************************************************
  * @brief   在指定区域内填充单个颜色
  * @input   (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
  * @input   color:要填充的颜色
  * @return
***************************************************************************************/
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{
    uint16_t i, j;
    uint16_t xlen = 0;
    xlen = ex - sx + 1;
    for(i = sy; i <= ey; i++)
    {
        LCD_SetCursor(sx, i);      				//设置光标位置
        LCD->reg = MEM_WRITE; ;     			//开始写入GRAM
        for(j = 0; j < xlen; j++)
            LCD->data = color;	//显示颜色
    }
}


/***************************************************************************************
  * @brief  LCD初始化序列 
  * @input   
  * @return  
***************************************************************************************/
void LCD_InitSequence(void)
{
    LCD->reg =  (0xCF);
    LCD->data = (0x00);
    LCD->data = (0xC1);
    LCD->data = (0X30);
    LCD->reg =  (0xED);
    LCD->data = (0x64);
    LCD->data = (0x03);
    LCD->data = (0X12);
    LCD->data = (0X81);
    LCD->reg =  (0xE8);
    LCD->data = (0x85);
    LCD->data = (0x10);
    LCD->data = (0x7A);
    LCD->reg =  (0xCB);
    LCD->data = (0x39);
    LCD->data = (0x2C);
    LCD->data = (0x00);
    LCD->data = (0x34);
    LCD->data = (0x02);
    LCD->reg =  (0xF7);
    LCD->data = (0x20);
    LCD->reg =  (0xEA);
    LCD->data = (0x00);
    LCD->data = (0x00);
    LCD->reg =  (0xC0);    //Power control
    LCD->data = (0x1B);   //VRH[5:0]
    LCD->reg =  (0xC1);    //Power control
    LCD->data = (0x01);   //SAP[2:0];BT[3:0]
    LCD->reg =  (0xC5);    //VCM control
    LCD->data = (0x30); 	 //3F
    LCD->data = (0x30); 	 //3C
    LCD->reg =  (0xC7);    //VCM control2
    LCD->data = (0XB7);
    LCD->reg =  (0x36);    // Memory Access Control
    LCD->data = (0x48);
    LCD->reg =  (0x3A);
    LCD->data = (0x55);
    LCD->reg =  (0xB1);
    LCD->data = (0x00);
    LCD->data = (0x1A);
    LCD->reg =  (0xB6);    // Display Function Control
    LCD->data = (0x0A);
    LCD->data = (0xA2);
    LCD->reg =  (0xF2);    // 3Gamma Function Disable
    LCD->data = (0x00);
    LCD->reg =  (0x26);    //Gamma curve selected
    LCD->data = (0x01);
    LCD->reg =  (0xE0);    //Set Gamma
    LCD->data = (0x0F);
    LCD->data = (0x2A);
    LCD->data = (0x28);
    LCD->data = (0x08);
    LCD->data = (0x0E);
    LCD->data = (0x08);
    LCD->data = (0x54);
    LCD->data = (0XA9);
    LCD->data = (0x43);
    LCD->data = (0x0A);
    LCD->data = (0x0F);
    LCD->data = (0x00);
    LCD->data = (0x00);
    LCD->data = (0x00);
    LCD->data = (0x00);
    LCD->reg =  (0XE1);    //Set Gamma
    LCD->data = (0x00);
    LCD->data = (0x15);
    LCD->data = (0x17);
    LCD->data = (0x07);
    LCD->data = (0x11);
    LCD->data = (0x06);
    LCD->data = (0x2B);
    LCD->data = (0x56);
    LCD->data = (0x3C);
    LCD->data = (0x05);
    LCD->data = (0x10);
    LCD->data = (0x0F);
    LCD->data = (0x3F);
    LCD->data = (0x3F);
    LCD->data = (0x0F);
    LCD->reg =  (0x2B);
    LCD->data = (0x00);
    LCD->data = (0x00);
    LCD->data = (0x01);
    LCD->data = (0x3f);
    LCD->reg =  (0x2A);
    LCD->data = (0x00);
    LCD->data = (0x00);
    LCD->data = (0x00);
    LCD->data = (0xef);
    LCD->reg = (0x11); //Exit Sleep
//    HAL_Delay(120);
    vTaskDelay(1120);
    LCD->reg = (0x29); //display on
}



