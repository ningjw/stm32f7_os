/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */
DIR      dirSD;
FILINFO  fileInfo;	//文件信息
uint32_t sd_total_size,sd_free_size;
extern   SD_HandleTypeDef hsd1;
/* USER CODE END Variables */    

void MX_FATFS_Init(void) 
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */     
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */  
}

/* USER CODE BEGIN Application */

/***************************************************************************************
  * @brief   遍历文件
  * @input   path:路径
  * @return  
***************************************************************************************/
uint8_t sd_scan_files(char * path)
{
	FRESULT res;	 
    res = f_opendir(&dirSD, (const TCHAR*)path); //打开一个目录
    if (res == FR_OK) 
	{
		printf("\r\n"); 
		for(;;)
		{
	        res = f_readdir(&dirSD, &fileInfo);                   //读取目录下的一个文件
	        if (res != FR_OK || fileInfo.fname[0] == 0) 
                break;  //错误了/到末尾了,退出

            if(fileInfo.fattrib == AM_DIR){//目录
                printf("dir :");
            }else if(fileInfo.fattrib == AM_ARC){//文档
                printf("file:");
            }else{
                printf("null:");
            }
			printf("%s  %dByte %d-%d-%d %02d:%02d\r\n", 
                    fileInfo.fname, (int)fileInfo.fsize,
                    1980+((fileInfo.fdate & 0xFE00)>>9), (fileInfo.fdate & 0x1E0)>>5, fileInfo.fdate & 0x1F,
                    ((fileInfo.ftime & 0xF800)>>11), ((fileInfo.ftime & 0x1E0)>>5));
		}
        f_closedir(&dirSD);
    }
    return res;	  
}

/***************************************************************************************
  * @brief   Search a directory for objects and display it
  * @input   
  * @return  
***************************************************************************************/
void find_txt_file (void)
{
    FRESULT fr;     /* Return value */

//    fr = f_opendir(&dirSD, (const TCHAR*)SDPath); //打开一个目录
    
    fr = f_findfirst(&dirSD, &fileInfo, "", "DATA*");  /* Start to search for photo files */
    
    while (fr == FR_OK && fileInfo.fname[0]) {            /* Repeat while an item is found */
        printf("%s\n", fileInfo.fname);                   /* Display the object name */
        fr = f_findnext(&dirSD, &fileInfo);               /* Search for next item */
    }

    f_closedir(&dirSD);
}

								  
/***************************************************************************************
  * @brief   更新字体文件,UNIGBK,GBK12,GBK16,GBK24,GBK32一起更新
  * @input   src:字库来源磁盘."0:",SD卡;"1:",FLASH盘,"2:",U盘.
  * @return  
***************************************************************************************/
uint8_t update_font(uint8_t* src)
{
    
}

/***************************************************************************************
  * @brief   
  * @input   
  * @return  
***************************************************************************************/
void Fatfs_test(void)
{
//    char read_buff[100]; /* Line buffer */
    
    /*##-1- Register the file system object to the FatFs module ##############*/
    retSD = f_mount(&SDFatFS, SDPath, 1); 					//挂载SD卡
    if(retSD)
    {
        Error_Handler();
    }
    sd_total_size = (hsd1.SdCard.BlockNbr >> 20) *  hsd1.SdCard.BlockSize;
    sd_total_size = (SDFatFS.csize * (SDFatFS.n_fatent-2) / 2) >> 10;
    sd_free_size  = (SDFatFS.csize * SDFatFS.free_clst / 2) >> 10;
    
    GUI_DispStringAt("SD Capacity:      M",10,40);
    GUI_DispDecAt(sd_free_size , 85, 40, 4);
    printf("SD Capacity: = %d M", (hsd1.SdCard.BlockNbr >> 20) *  hsd1.SdCard.BlockSize);
    printf("SD Total Size = %d M", sd_total_size);
    printf("SD Free  Size = %d M", sd_free_size);
    
//    sd_scan_files(SDPath);
    
//    retSD = f_mkdir("DATA");

    find_txt_file();
//    
//    /*##-2- Create and Open new text file objects with write access ######*/
//    retSD = f_open(&SDFile, "fatfs_test.txt", FA_CREATE_ALWAYS | FA_WRITE);
//    if(retSD)
//        printf(" open file error : %d\r\n",retSD);
//    else
//        printf(" open file sucess!!! \r\n");
//    
//    /* Read every line and display it */
//    while (f_gets(read_buff, sizeof read_buff, &SDFile)) {
//        printf("%s",read_buff);
//    }
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
