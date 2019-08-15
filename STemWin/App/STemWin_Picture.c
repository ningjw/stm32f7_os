#include "main.h"
#include "fatfs.h"

#define BMP_MAX_SIZE	(5120 * 1024)	//图片大小不大于5M,该宏定义大小应与GUIConf.c中的GUI_NUMBYTES一样

//绘制无需加载到RAM中的BMP图片时，图片每行的字节数
#define BMP_PERLINE_SIZE	2*1024	

static FIL PicFile;
static char bmpBuffer[BMP_PERLINE_SIZE];
/*********************************************************************
*
*       BmpGetData
*
* Function description
*   This routine is called by GUI_BMP_DrawEx(). The routine is responsible
*   for setting the data pointer to a valid data location with at least
*   one valid byte.
*
* Parameters:
*   p           - Pointer to application defined data.
*   NumBytesReq - Number of bytes requested.
*   ppData      - Pointer to data pointer. This pointer should be set to
*                 a valid location.
*   StartOfFile - If this flag is 1, the data pointer should be set to the
*                 beginning of the data stream.
*
* Return value:
*   Number of data bytes available.
*/
static int GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off) 
{
	static int readAddr = 0;
	FIL * phFile;
	UINT br;
	
	phFile = (FIL *)p;
	
	if (NumBytesReq > sizeof(bmpBuffer)) {
		NumBytesReq = sizeof(bmpBuffer);
	}

	//移动指针到应该读取的位置
	if(Off == 1) 
        readAddr = 0;
	else 
        readAddr = Off;

	f_lseek(phFile,readAddr);
	f_read(phFile, bmpBuffer, NumBytesReq, &br);//读取数据到缓冲区中

	*ppData = (U8 *)bmpBuffer;
    
	return br;//返回读取到的字节数
}

/***************************************************************************************
  * @brief   
  * @input   BMPFileName:图片在SD卡或者其他存储设备中的路径(需文件系统支持！)
  * @return  
***************************************************************************************/
uint8_t ReadDisp_bmp(char *fileName)
{
    int XSize,YSize;
    
    /*######1. 打开bmp文件,判断文件大小#######*/
    retSD = f_open(&PicFile, (const TCHAR*)fileName, FA_READ);
    if(retSD != FR_OK || PicFile.obj.objsize > BMP_MAX_SIZE){
        return 1;
    }
    
    /*######2. 获取bmp图像尺寸#######*/
    XSize = GUI_BMP_GetXSizeEx(GetData, &PicFile);	//获取图片的X轴大小
    YSize = GUI_BMP_GetYSizeEx(GetData, &PicFile);	//获取图片的Y轴大小
    
    /*######3. 通过GUI_BMP_GetXSizeEx函数在LCD中间显示bmp图像#######*/
    GUI_BMP_DrawEx(GetData, &PicFile, (LCD_WIDTH-XSize)/2-1, (LCD_HEIGHT-YSize)/2-1);
    
    
    GUI_BMP_DrawScaledEx(GetData, &PicFile, (LCD_WIDTH - XSize * 0.5)/2, (LCD_HEIGHT - YSize * 0.5)/2, 1, 2);
    
    f_close(&PicFile);
    
    return 0;
}


/***************************************************************************************
  * @brief   
  * @input   JPGFileName:图片在SD卡或者其他存储设备中的路径(需文件系统支持！)
  * @return  
***************************************************************************************/
uint8_t ReadDisp_jpg(char *FileName)
{
    GUI_JPEG_INFO JpegInfo;
    
    /*######1. 打开jpg文件,判断文件大小#######*/
    retSD = f_open(&PicFile, (const TCHAR*)FileName, FA_READ);
    if(retSD != FR_OK || PicFile.obj.objsize > BMP_MAX_SIZE){
        return 1;
    }
    
    /*######3. 在LCD中间显示jpg图像#######*/
    GUI_JPEG_DrawEx(GetData, &PicFile,(LCD_WIDTH-JpegInfo.XSize)/2, (LCD_HEIGHT-JpegInfo.YSize)/2);
    GUI_Delay(500);
    GUI_Clear();
    
    /*######4. 按0.5倍在LCD中间显示jpg图像#######*/
    GUI_JPEG_DrawScaledEx(GetData, &PicFile, (LCD_WIDTH - JpegInfo.XSize * 0.5)/2, (LCD_HEIGHT - JpegInfo.YSize * 0.5)/2, 1, 2);
    
    f_close(&PicFile);
    return 0;
}


/***************************************************************************************
  * @brief   
  * @input   FileName:图片在SD卡或者其他存储设备中的路径(需文件系统支持！)
  * @return  
***************************************************************************************/
uint8_t ReadDisp_gif(char *FileName)
{
    UINT br;
    char *bmpBuf;
    GUI_GIF_INFO       GifInfo;
    GUI_GIF_IMAGE_INFO ImageInfo;
    
    /*######1. 打开gif文件,判断文件大小#######*/
    retSD = f_open(&PicFile, (const TCHAR*)FileName, FA_READ);
    if(retSD != FR_OK || PicFile.obj.objsize > BMP_MAX_SIZE){
        return 1;
    }
    
    /*######2. 分配内存用于保存bmp文件内容#######*/
    bmpBuf = mem_malloc(PicFile.obj.objsize);
    if(bmpBuf == NULL){
        return 2;
    }
    
    /*######3. 将文件内容读入分配好的bmpBuf缓存#######*/
    retSD = f_read(&PicFile, bmpBuf, PicFile.obj.objsize, &br);
    if(retSD != FR_OK){
        return 3;
    }
    
    /*######4. 获取gif文件信息#######*/
    GUI_GIF_GetInfo(bmpBuf, PicFile.obj.objsize, &GifInfo);
    
    /*######5. 在LCD中间显示gif图像#######*/
    for(int i = 0; i < GifInfo.NumImages; i++)
    {
        GUI_MULTIBUF_Begin();
        GUI_GIF_DrawSub(bmpBuf, PicFile.obj.objsize, (LCD_WIDTH - GifInfo.xSize)/2, (LCD_HEIGHT - GifInfo.ySize)/2,i);
        GUI_GIF_GetImageInfo(bmpBuf, PicFile.obj.objsize, &ImageInfo, i);
        GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay*10 : 100 );//延时
    }
    
    /*######6. 按0.5倍关系在LCD中间显示gif图像#######*/
    for(int i=0; i < GifInfo.NumImages; i++)
    {
        GUI_GIF_DrawSubScaled(bmpBuf, PicFile.obj.objsize, (LCD_WIDTH - GifInfo.xSize * 0.5)/2, (LCD_HEIGHT - GifInfo.ySize * 0.5)/2, i, 1 ,2);
        GUI_GIF_GetImageInfo(bmpBuf, PicFile.obj.objsize, &ImageInfo, i);
        GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay*10 : 100 );//延时
    }
    
    mem_free(bmpBuf);
    f_close(&PicFile);
    return 0;
}
/***************************************************************************************
  * @brief   
  * @input   FileName:图片在SD卡或者其他存储设备中的路径(需文件系统支持！)
  * @return  
***************************************************************************************/
uint8_t ReadDisp_png(char *FileName)
{
    int XSize,YSize;
    
    /*######1. 打开png文件,判断文件大小#######*/
    retSD = f_open(&PicFile, (const TCHAR*)FileName, FA_READ);
    if(retSD != FR_OK || PicFile.obj.objsize > BMP_MAX_SIZE){
        return 1;
    }
    
    /*######2. 获取png尺寸#######*/
//    XSize = GUI_PNG_GetXSizeEx(GetData, &PicFile);//PNG图片X大小
//	YSize = GUI_PNG_GetYSizeEx(GetData, &PicFile);//PNG图片Y大小
//    
//    /*######3. 在LCD中间显示png图像#######*/
//    GUI_PNG_DrawEx(GetData,&PicFile,(LCD_WIDTH - XSize)/2,(LCD_HEIGHT - YSize)/2);
    
    f_close(&PicFile);
    return 0;
}

