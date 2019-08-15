#ifndef __STEMWIN_FONT_H
#define __STEMWIN_FONT_H

//字库区域占用的总扇区数大小(4个字库+unigbk表+字库信息=6302984字节,约占1539个W25QXX扇区,一个扇区4K字节)
#define FONTSECSIZE	 	1539
//字库存放起始地址
#define FONTINFOADDR 	1024*1024*25 	//前面25M被fatfs占用了;从25M地址以后开始存放字库;
//25M以后紧跟4个字库+UNIGBK.BIN,总大小6.01M;31.01M以后,用户可以自由使用.

/* USER CODE BEGIN Prototypes */
//字库信息结构体定义
//用来保存字库基本信息，地址，大小等
__packed typedef struct 
{
	uint8_t  fontok;			//字库存在标志，0XAA，字库正常；其他，字库不存在
	uint32_t ugbkaddr; 			//unigbk的地址
	uint32_t ugbksize;			//unigbk的大小	 
	uint32_t f12addr;			//gbk12地址	
	uint32_t gbk12size;			//gbk12的大小	 
	uint32_t f16addr;			//gbk16地址
	uint32_t gbk16size;			//gbk16的大小		 
	uint32_t f24addr;			//gbk24地址
	uint32_t gbk24size;			//gbk24的大小 	 
	uint32_t f32addr;			//gbk32地址
	uint32_t gbk32size;			//gbk32的大小 
}_font_info; 

extern _font_info ftinfo;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ16;

void update_font(void);

#endif
