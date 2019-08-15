#include "main.h"
#include "fatfs.h"
//#include "GUI.h"
#include "GUI_Private.h"
#include "EmWin_ASCII_Font.h"
/* USER CODE BEGIN Variables */


//字库存放在磁盘中的路径
char* const FONT_PATH[5] =
{
    "FONT/UNIGBK.BIN",	//UNIGBK.BIN的存放位置
    "FONT/GBK12.FON",	//GBK12的存放位置
    "FONT/GBK16.FON",	//GBK16的存放位置
    "FONT/GBK24.FON",	//GBK24的存放位置
    "FONT/GBK32.FON",	//GBK32的存放位置
};

//用来保存字库基本信息，地址，大小等
_font_info ftinfo;
char     full_font_path[64];
uint8_t  font_buf[4096];
uint32_t fontSectorSize;  
uint32_t offx = 0, flashaddr = 0;

//字模数据暂存数组,单个字模的最大字节数
//单个字模最大为1024
#define BYTES_PER_FONT	1024	//32*32=1024，最大显示32*32点阵字库	
U8 GUI_FontDataBuf[BYTES_PER_FONT];

//声明一种新的字体结构
#define GUI_FONTTYPE_PROP_USER      \
   GUIPROP_X_DispChar,             	\
  (GUI_GETCHARDISTX*)GUIPROP_X_GetCharDistX,\
   GUIMONO_GetFontInfo,          	\
   GUIMONO_IsInFont,             	\
  (GUI_GETCHARINFO *)0,         	\
  (tGUI_ENC_APIList*)0  

void GUIPROP_X_DispChar(U16P c);
int GUIPROP_X_GetCharDistX(U16P c);
/*-----12×12字体----------------------------------------------------------*/
GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ12_CharInfo[2] = 
{    
	{ 6, 	6, 	1, (void*)"0"},  //
	{ 12, 	12, 2, (void*)"0"},  //表示此点阵字符的X轴长度是12个像素点，实际显示也是12个像素点，显示一行需要2字节，使用字符“0”作为标识
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ12_PropHZ = {
      0x4081, 
      0xFFFF, 
      &GUI_FontHZ12_CharInfo[1],
      (void *)0, 
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ12_PropASC = {
      0x0000, 
      0x007F, 
      &GUI_FontHZ12_CharInfo[0],
      (void GUI_CONST_STORAGE *)&GUI_FontHZ12_PropHZ, 
};

GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ12 = 
{
      GUI_FONTTYPE_PROP_USER, 
      12, 
      12, 
      1,  
      1,  
      (void GUI_CONST_STORAGE *)&GUI_FontHZ12_PropASC
};
/*-----16×16字体----------------------------------------------------------*/
GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ16_CharInfo[2] = 
{    
	{ 8, 	8, 	1, (void*)"0"},  
	{ 16, 	16, 2, (void*)"0"},
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ16_PropHZ = {
      0x4081, 
      0xFFFF, 
      &GUI_FontHZ16_CharInfo[1],
      (void *)0, 
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ16_PropASC = {
      0x0000, 
      0x007F, 
      &GUI_FontHZ16_CharInfo[0],
      (void GUI_CONST_STORAGE *)&GUI_FontHZ16_PropHZ, 
};


GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ16 = 
{
      GUI_FONTTYPE_PROP_USER, 
      16, 
      16, 
      1,  
      1,  
      (void GUI_CONST_STORAGE *)&GUI_FontHZ16_PropASC
};
/*-----24×24字体----------------------------------------------------------*/
GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ24_CharInfo[2] = 
{
	{ 12, 	12, 2, (void*)"0"},  
	{ 24, 	24, 3, (void*)"0"},
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ24_PropHZ = {
      0x4081, 
      0xFFFF, 
      &GUI_FontHZ24_CharInfo[1],
      (void *)0, 
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ24_PropASC = {
      0x0000, 
      0x007F, 
      &GUI_FontHZ24_CharInfo[0],
      (void GUI_CONST_STORAGE *)&GUI_FontHZ24_PropHZ, 
};

GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ24 = 
{
      GUI_FONTTYPE_PROP_USER, 
      24, 
      24, 
      1,  
      1,  
      (void GUI_CONST_STORAGE *)&GUI_FontHZ24_PropASC
};

GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ24x2 = 
{
      GUI_FONTTYPE_PROP_USER, 
      24, 
      24, 
      2,  
      2,  
      (void GUI_CONST_STORAGE *)&GUI_FontHZ24_PropASC
};
/*-----32×32字体----------------------------------------------------------*/
GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ32_CharInfo[2] = 
{    
	{ 16, 	16, 2, (void*)"0"},  
	{ 32, 	32, 4, (void*)"0"},
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ32_PropHZ = {
      0x4081, 
      0xFFFF, 
      &GUI_FontHZ32_CharInfo[1],
      (void *)0, 
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ32_PropASC = {
      0x0000, 
      0x007F, 
      &GUI_FontHZ32_CharInfo[0],
      (void GUI_CONST_STORAGE *)&GUI_FontHZ32_PropHZ, 
};

GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ32 = 
{
      GUI_FONTTYPE_PROP_USER, 
      32, 
      32, 
      1,  
      1,  
      (void GUI_CONST_STORAGE *)&GUI_FontHZ32_PropASC
};


/***************************************************************************************
  * @brief   更新字体文件,UNIGBK,GBK12,GBK16,GBK24,GBK32一起更新
  * @input   src:字库来源磁盘."0:",SD卡;"1:",FLASH盘,"2:",U盘.
  * @return
***************************************************************************************/
void update_font(void)
{
    uint8_t rval;
    UINT br;
    
    /*#### 1.finnd UNIGBK,GBK12,GBK16,GBK24,GBK32,calculate the total size.######## */
    fontSectorSize = 0;
    for(uint8_t i = 0; i < 5; i++)	//
    {
        strcpy(full_font_path, SDPath);			//copy src内容到pname
        strcat(full_font_path, FONT_PATH[i]);		//追加具体文件路径
        retSD = f_open(&SDFile, (const TCHAR*)full_font_path, FA_READ);	//尝试打开
        if(retSD){
            rval |= 1 << 7;	//标记打开文件失败
            break;		   //出错了,直接退出
        }
        fontSectorSize += SDFile.obj.objsize;
        f_close(&SDFile);
    }
    GUI_SetFont(&GUI_FontHZ16); // 设置字体
    GUI_DispStringAt("开始更新字库!!",100,180);
    if(rval == 0)			//字库文件都存在.
    {
        fontSectorSize = fontSectorSize / 4096  + (fontSectorSize%4096 ? 1 : 0);
        
        /*#### 2.Erase sector #################################################### */
        for(uint32_t i = 0; i < fontSectorSize; i++)			//先擦除字库区域,提高写入速度
        {
            W25QXX_EraseSector((FONTINFOADDR / 4096) + i);	//需要擦除的扇区
        }
        
        for(uint8_t i = 0; i < 5; i++)	//依次更新UNIGBK,GBK12,GBK16,GBK24,GBK32
        {
            offx = 0;
            strcpy(full_font_path, SDPath);			//copy src内容到pname
            strcat(full_font_path, FONT_PATH[i]);		//追加具体文件路径
            if(f_open(&SDFile, (const TCHAR*)full_font_path, FA_READ) == FR_OK) {
                switch(i)
                {
                case 0:												//更新UNIGBK.BIN
                    ftinfo.ugbkaddr = FONTINFOADDR + sizeof(ftinfo);//信息头之后，紧跟UNIGBK转换码表
                    ftinfo.ugbksize = SDFile.obj.objsize;			//UNIGBK大小
                    flashaddr = ftinfo.ugbkaddr;
                    break;
                case 1:
                    ftinfo.f12addr = ftinfo.ugbkaddr + ftinfo.ugbksize;	//UNIGBK之后，紧跟GBK12字库
                    ftinfo.gbk12size = SDFile.obj.objsize;				//GBK12字库大小
                    flashaddr = ftinfo.f12addr;                         //GBK12的起始地址
                    break;
                case 2:
                    ftinfo.f16addr = ftinfo.f12addr + ftinfo.gbk12size;	//GBK12之后，紧跟GBK16字库
                    ftinfo.gbk16size = SDFile.obj.objsize;				//GBK16字库大小
                    flashaddr = ftinfo.f16addr;						    //GBK16的起始地址
                    break;
                case 3:
                    ftinfo.f24addr = ftinfo.f16addr + ftinfo.gbk16size;	//GBK16之后，紧跟GBK24字库
                    ftinfo.gbk24size = SDFile.obj.objsize;				//GBK24字库大小
                    flashaddr = ftinfo.f24addr;						    //GBK24的起始地址
                    break;    
                case 4:
                    ftinfo.f32addr = ftinfo.f24addr + ftinfo.gbk24size;	//GBK24之后，紧跟GBK32字库
                    ftinfo.gbk32size = SDFile.obj.objsize;				//GBK32字库大小
                    flashaddr = ftinfo.f32addr;						    //GBK32的起始地址
                    break;
                }
                /*#### 3.copy font frome SD card to SPI flash. ##################### */
                while(retSD == FR_OK)//死循环执行
                {
                    retSD = f_read(&SDFile, font_buf, 4096, (UINT *)&br);//读取数据
                    if(retSD != FR_OK) break;
                    W25QXX_Write_NoCheck(font_buf, offx + flashaddr, 4096);//将读出来的字库数据写入SPI Flash
                    offx += br;
                    if(br != 4096) break;
                }
            }
            f_close(&SDFile);
        }
        //全部更新好了
		ftinfo.fontok = 0xAA;
		W25QXX_Write((uint8_t*)&ftinfo, FONTINFOADDR, sizeof(ftinfo));	//保存字库信息
        GUI_SetFont(&GUI_FontHZ32); // 设置字体
        GUI_DispStringAt("成功更新字库!!\n",100,200);
    }
}

/***************************************************************************************
  * @brief   获取字模数据
  * @input   
  * @return  
***************************************************************************************/
static void GUI_GetDataFromMemory(const GUI_FONT_PROP GUI_UNI_PTR *pProp, U16P c)
{
	unsigned char qh,ql;
	unsigned char i;					  
    unsigned long foffset; 
	unsigned char t;
	unsigned char *mat;
    U8 size,csize;//字体大小
    U16 BytesPerFont;
	GUI_FONT EMWINFONT;
	EMWINFONT = *GUI_pContext->pAFont;
    BytesPerFont = GUI_pContext->pAFont->YSize * pProp->paCharInfo->BytesPerLine; //每个字模的数据字节数
    if (BytesPerFont > BYTES_PER_FONT) BytesPerFont = BYTES_PER_FONT;
	
	//判断字体的大小
	if(memcmp(&EMWINFONT,&GUI_FontHZ12,sizeof(GUI_FONT)) == 0) size=12;			//12字体
	else if(memcmp(&EMWINFONT,&GUI_FontHZ16,sizeof(GUI_FONT)) == 0) size=16;	//16字体
	else if(memcmp(&EMWINFONT,&GUI_FontHZ24,sizeof(GUI_FONT)) == 0)	size=24;	//24字体  
    else if(memcmp(&EMWINFONT,&GUI_FontHZ32,sizeof(GUI_FONT)) == 0)	size=32;	//32字体
    
	csize = (size/8+((size%8)?1:0))*(size);	//得到字体一个字符对应点阵集所占的字节数	
    memset(GUI_FontDataBuf,0,csize);        //先清零    
    if (c < 0x80)	//英文字符地址偏移算法
    {
		switch(size)
		{
			case 12:
				for(t=0;t<12;t++) GUI_FontDataBuf[t]=emwin_asc2_1206[c-0x20][t];
				break;
			case 16:
				for(t=0; t<16; t++) GUI_FontDataBuf[t] = emwin_asc2_1608[c-0x20][t];
				break;
			case 24:
				for(t=0;t<48;t++) GUI_FontDataBuf[t]=emwin_asc2_2412[c-0x20][t];
				break;	
            case 32:
				for(t=0;t<64;t++) GUI_FontDataBuf[t]=emwin_asc2_3216[c-0x20][t];
				break;	
		} 
        if(c=='\r'||c=='\n')  memset(GUI_FontDataBuf,0,csize);    
	}else{
		ql=c/256;
		qh=c%256;
		if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非常用汉字
		{
			for(i=0;i<(size*2);i++) *mat++=0x00;//填充满格
			return; //结束访问
		}          
		if(ql<0x7f)ql -= 0x40;
		else ql-=0x41;
		qh -= 0x81;   
		foffset=((unsigned long)190*qh+ql) * csize;//得到字库中的字节偏移量	    
		switch(size)
		{
			case 12:
				W25QXX_Read(GUI_FontDataBuf,foffset+ftinfo.f12addr,csize);
				break;
			case 16:
				W25QXX_Read(GUI_FontDataBuf,foffset + ftinfo.f16addr,csize);
				break;
			case 24:
				W25QXX_Read(GUI_FontDataBuf,foffset+ftinfo.f24addr,csize);
				break;	
            case 32:
				W25QXX_Read(GUI_FontDataBuf,foffset+ftinfo.f32addr,csize);
				break;
		} 
	}   	
}


/*********************************************************************
*
*       GUIPROP_DispChar
*
* Purpose:
*   This is the routine that displays a character. It is used by all
*   other routines which display characters as a subroutine.
*/
void GUIPROP_X_DispChar(U16P c)
{
	int BytesPerLine;
    GUI_DRAWMODE DrawMode = GUI_pContext->TextMode;
    const GUI_FONT_PROP GUI_UNI_PTR *pProp = GUI_pContext->pAFont->p.pProp;
    //搜索定位字库数据信息 
    for (; pProp; pProp = pProp->pNext)                                         
    {
        if ((c >= pProp->First) && (c <= pProp->Last)) break;
    }
    if (pProp)
    {
        GUI_DRAWMODE OldDrawMode;
        const GUI_CHARINFO GUI_UNI_PTR * pCharInfo = pProp->paCharInfo;
        GUI_GetDataFromMemory(pProp, c);//取出字模数据
        BytesPerLine = pCharInfo->BytesPerLine;                
        OldDrawMode  = LCD_SetDrawMode(DrawMode);
        LCD_DrawBitmap(GUI_pContext->DispPosX, GUI_pContext->DispPosY,
                       pCharInfo->XSize, GUI_pContext->pAFont->YSize,
                       GUI_pContext->pAFont->XMag, GUI_pContext->pAFont->YMag,
                       1,     /* Bits per Pixel */
                       BytesPerLine,
                       &GUI_FontDataBuf[0],
                       &LCD_BKCOLORINDEX
                       );
        /* Fill empty pixel lines */
        if (GUI_pContext->pAFont->YDist > GUI_pContext->pAFont->YSize) 
        {
            int YMag = GUI_pContext->pAFont->YMag;
            int YDist = GUI_pContext->pAFont->YDist * YMag;
            int YSize = GUI_pContext->pAFont->YSize * YMag;
            if (DrawMode != LCD_DRAWMODE_TRANS) 
            {
                LCD_COLOR OldColor = GUI_GetColor();
                GUI_SetColor(GUI_GetBkColor());
                LCD_FillRect(GUI_pContext->DispPosX, GUI_pContext->DispPosY + YSize, 
                             GUI_pContext->DispPosX + pCharInfo->XSize, 
                             GUI_pContext->DispPosY + YDist);
                GUI_SetColor(OldColor);
            }
        }
        LCD_SetDrawMode(OldDrawMode); /* Restore draw mode */
		GUI_pContext->DispPosX += pCharInfo->XDist * GUI_pContext->pAFont->XMag;
    }
}

/*********************************************************************
*
*       GUIPROP_GetCharDistX
*/
int GUIPROP_X_GetCharDistX(U16P c) 
{
    const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUI_pContext->pAFont->p.pProp;  
    for (; pProp; pProp = pProp->pNext)                                         
    {
        if ((c >= pProp->First) && (c <= pProp->Last))break;
    }
    return (pProp) ? (pProp->paCharInfo)->XSize * GUI_pContext->pAFont->XMag : 0;
}

