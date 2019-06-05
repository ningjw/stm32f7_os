#include "main.h"




void Drow_2D(void)
{
    static uint8_t count = 0;
    count++;
    switch(count % 2) {
    case 0:
//        LCD_Fill(0, 0, 480, 800, GUI_GREEN);
        GUI_SetBkColor(GUI_BLUE);
        GUI_Clear();
        GUI_SetFont(&GUI_Font16_ASCII);
        GUI_DispStringHCenterAt("ALIENEK BIN&HEX NUM DISPLAY", 160, 0);
        GUI_SetColor(GUI_RED);
        GUI_SetFont(&GUI_Font13_ASCII);
//基本绘制例程函数演示
        GUI_SetBkColor(GUI_YELLOW);
        GUI_ClearRect(5, 20, 95, 110); //此函数使用背景颜色填充
        GUI_SetBkColor(GUI_BLUE);
//绘制水平颜色梯度填充的矩形
        GUI_DrawGradientH(100, 20, 190, 110, 0X4117BB, 0XC6B6F5);
//绘制垂直颜色梯度填充的圆角矩形
        GUI_DrawGradientRoundedV(195, 20, 285, 110, 25, 0X4117BB, 0XC6B6F5);
        GUI_DrawRect(5, 115, 95, 205); //在指定位置绘制矩形
        GUI_FillRect(100, 115, 190, 205); //在指定位置绘制填充的矩形区域
        GUI_FillRoundedRect(195, 115, 285, 205, 25); //在指定位置绘制填充的圆角矩形
        break;
    case 1:
//        LCD_Fill(0, 0, 480, 800, GUI_WHITE);
        GUI_SetBkColor(GUI_BLACK);
        GUI_Clear();
        GUI_SetFont(&GUI_Font16_ASCII);
        GUI_SetColor(GUI_BLACK);
        GUI_DispStringHCenterAt("ALIENEK ALPHA DISPLAY", 160, 0);
        GUI_EnableAlpha(1); //开启 Alpha
        GUI_SetColor(GUI_BLACK);
        GUI_DispStringHCenterAt("Alphablending", 45, 61);
        GUI_SetAlpha(40); //设置 Alpha 值
        GUI_SetColor( GUI_RED);
        GUI_FillRect(0, 20, 49, 69);
        GUI_SetAlpha(80); //设置 Alpha 值
        GUI_SetColor(GUI_GREEN);
        GUI_FillRect(20, 40, 69, 89);
        GUI_SetAlpha(40); //设置 Alpha 值
        GUI_SetColor(GUI_BLUE);
        GUI_FillRect(40, 60, 89, 109);
        GUI_EnableAlpha(0); //关闭 Alpha
        break;
        
    default:
        LCD_Fill(0, 0, 480, 800, 0xff);
        break;
    }






}





