#include "stm32f7xx_hal.h"
#include "utility.h"
#include "rl_fs.h"
#include "stdio.h"

#define BUSY_GPIO	GPIOD
#define BUSY_PIN	GPIO_PIN_6

/* 定义NAND Flash的物理地址。这个是由硬件决定的 */
#define Bank_NAND_ADDR     ((uint32_t)0x80000000)

/* 定义操作NAND Flash用到3个宏 */
#define NAND_CMD_AREA		*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA)
#define NAND_ADDR_AREA		*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA)
#define NAND_DATA_AREA		*(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA)

//NAND FLASH操作相关延时函数
#define NAND_TADL_DELAY				30			//tADL等待延迟,最少70ns
#define NAND_TWHR_DELAY				25			//tWHR等待延迟,最少60ns
#define NAND_TRHW_DELAY				35			//tRHW等待延迟,最少100ns
#define NAND_TPROG_DELAY			200			//tPROG等待延迟,典型值200us,最大需要700us
#define NAND_TBERS_DELAY			4			//tBERS等待延迟,典型值3.5ms,最大需要10ms

#define NAND_OK   0
#define NAND_FAIL 1

/* FlashFS API的返回值 */
static const char * ReVal_Table[]= 
{
	"fsOK：成功",				                        
	"fsError：未指定的错误",
	"fsUnsupported：操作不支持",
	"fsAccessDenied：资源访问被拒绝",
	
	"fsInvalidParameter：参数无效",
	"fsInvalidDrive：驱动无效或驱动不存在",
	"fsInvalidPath：路径无效",
	"fsUninitializedDrive：驱动未初始化 ",

	"fsDriverError：读写操作",
	"fsMediaError：媒介错误",
	"fsNoMedia：媒介不存在，或者未初始化",
	"fsNoFileSystem：文件系统未格式化",

	"fsNoFreeSpace：没有可用空间",
	"fsFileNotFound：文件未找到",
	"fsDirNotEmpty：文件夹非空",
	"fsTooManyOpenFiles：打开文件太多",
};

/* 必须等待，否则读出数据异常, 此处应该判断超时*/
#define WAIT_BUSY()	{	\
	uint16_t k; 	\
	for (k = 0; k < 200; k++)	\
	{	\
		if ((BUSY_GPIO->IDR & BUSY_PIN) == 0) break;	\
	}	\
	for (k = 0; k < 2000; k++)	\
	{	\
		if ((BUSY_GPIO->IDR & BUSY_PIN) != 0) break;	\
	}	\
}



/**
  \fn          int32_t Driver_NANDx_GetDeviceBusy (uint32_t dev_num)
  \brief       NAND Driver GetDeviceBusy callback.
               Needs to be implemented by user.
  \param[in]   dev_num   Device number
  \return      1=busy, 0=not busy, or error
*/
int32_t Driver_NAND0_GetDeviceBusy (uint32_t dev_num)
{
	WAIT_BUSY();
	
	return 0;
}


/*
*********************************************************************************************************
*	函 数 名: FSMC_NAND_ReadStatus
*	功能说明: 使用Read statuc 命令读NAND Flash内部状态
*	形    参:  - Address: 被擦除的快内任意地址
*	返 回 值: NAND操作状态，有如下几种值：
*             - NAND_BUSY: 内部正忙
*             - NAND_READY: 内部空闲，可以进行下步操作
*             - NAND_ERROR: 先前的命令执行失败
*********************************************************************************************************
*/
static uint32_t FMC_NAND_ReadStatus(void)
{
	uint8_t ucData;
	uint8_t ucStatus = NAND_BUSY;

	/* 读状态操作 */
	NAND_CMD_AREA = NAND_CMD_STATUS;
	ucData = *(__IO uint8_t *)(Bank_NAND_ADDR);

	if((ucData & NAND_ERROR) == NAND_ERROR){
		ucStatus = NAND_ERROR;
	} else if((ucData & NAND_READY) == NAND_READY){
		ucStatus = NAND_READY;
	} else {
		ucStatus = NAND_BUSY;
	}

	return (ucStatus);
}

/*
*********************************************************************************************************
*	函 数 名: FSMC_NAND_GetStatus
*	功能说明: 获取NAND Flash操作状态
*	形    参:  - Address: 被擦除的快内任意地址
*	返 回 值: NAND操作状态，有如下几种值：
*             - NAND_TIMEOUT_ERROR  : 超时错误
*             - NAND_READY          : 操作成功
*             - NAND_ERROR: 先前的命令执行失败
*********************************************************************************************************
*/
static uint32_t FMC_NAND_WaitForReady(void)
{
	uint32_t ulTimeout = 0x10000;
	uint32_t ucStatus = NAND_READY;

	ucStatus = FMC_NAND_ReadStatus();

	/* 等待NAND操作结束，超时后会退出 */
	while ((ucStatus != NAND_READY) &&( ulTimeout != 0x00))
	{
		ucStatus = FMC_NAND_ReadStatus();
		if(ucStatus == NAND_ERROR){
			return (ucStatus);/* 返回操作状态 */
		}
		ulTimeout--;
	}

	if(ulTimeout == 0x00)
	{
		ucStatus =  NAND_TIMEOUT_ERROR;
	}

	/* 返回操作状态 */
	return (ucStatus);
}

/*
*********************************************************************************************************
*	函 数 名: FMC_NAND_Reset
*	功能说明: 复位NAND Flash
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t NAND_Reset(void)
{
	NAND_CMD_AREA = NAND_CMD_RESET;

	/* 检查操作状态 */
	if (FMC_NAND_WaitForReady() == NAND_READY){
		return NAND_OK;
	}
	return NAND_FAIL;
}

/*
*********************************************************************************************************
*	函 数 名: NAND_ReadID
*	功能说明: 读NAND Flash的ID。ID存储到形参指定的结构体变量中。
*	形    参:  无
*	返 回 值: 32bit的NAND Flash ID
*********************************************************************************************************
*/
uint32_t NAND_ReadID(void)
{
	uint8_t deviceId[5];
    uint32_t id;  
	/* 发送命令 Command to the command area */
	NAND_CMD_AREA = 0x90;
	NAND_ADDR_AREA = 0x00;
    
	/* 顺序读取NAND Flash的ID */
    	//ID一共有5个字节
    deviceId[0]=*(__IO uint8_t*)Bank_NAND_ADDR;      
    deviceId[1]=*(__IO uint8_t*)Bank_NAND_ADDR;  
    deviceId[2]=*(__IO uint8_t*)Bank_NAND_ADDR; 
    deviceId[3]=*(__IO uint8_t*)Bank_NAND_ADDR; 
    deviceId[4]=*(__IO uint8_t*)Bank_NAND_ADDR;  

    id = ((uint32_t)deviceId[1])<<24|((uint32_t)deviceId[2])<<16|((uint32_t)deviceId[3])<<8|deviceId[4];
	return id;
}

/*
*********************************************************************************************************
*	函 数 名: DotFormatjavascript:;
*	功能说明: 将数据规范化显示，方便用户查看
*             比如
*             2345678   ---->  2.345.678
*             334426783 ---->  334.426.783
*             以三个数为单位进行显示
*	形    参: _ullVal   需要规范显示的数值
*             _sp       规范显示后数据存储的buf。
*	返 回 值: 无
*********************************************************************************************************
*/
static void DotFormat(uint64_t _ullVal, char *_sp) 
{
	/* 数值大于等于10^9 */
	if (_ullVal >= (uint64_t)1e9) 
	{
		_sp += sprintf (_sp, "%d.", (uint32_t)(_ullVal / (uint64_t)1e9));
		_ullVal %= (uint64_t)1e9;
		_sp += sprintf (_sp, "%03d.", (uint32_t)(_ullVal / (uint64_t)1e6));
		_ullVal %= (uint64_t)1e6;
		sprintf (_sp, "%03d.%03d", (uint32_t)(_ullVal / 1000), (uint32_t)(_ullVal % 1000));
		return;
	}
	
	/* 数值大于等于10^6 */
	if (_ullVal >= (uint64_t)1e6) 
	{
		_sp += sprintf (_sp,"%d.", (uint32_t)(_ullVal / (uint64_t)1e6));
		_ullVal %= (uint64_t)1e6;
		sprintf (_sp,"%03d.%03d", (uint32_t)(_ullVal / 1000), (uint32_t)(_ullVal % 1000));
		return;
	}
	
	/* 数值大于等于10^3 */
	if (_ullVal >= 1000) 
	{
		sprintf (_sp, "%d.%03d", (uint32_t)(_ullVal / 1000), (uint32_t)(_ullVal % 1000));
		return;
	}
	
	/* 其它数值 */
	sprintf (_sp,"%d",(uint32_t)(_ullVal));
}
/*
*********************************************************************************************************
*	函 数 名: ViewRootDir
*	功能说明: 获取Nand flash容量
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void GetCapacity(void)
{
    uint64_t capacity;
    uint8_t buf[15] = {0};
    fsMediaInfo info;
    fsStatus restatus;
    int32_t id;
     uint32_t ser_num;
    
        /* 获取volume label */
	if (fvol ("N0:", (char *)buf, &ser_num) == 0) 
	{
		if (buf[0]) 
		{
			printf ("NAND的volume label是 %s\r\n", buf);
		}
		else 
		{
			printf ("NAND没有volume label\r\n");
		}
		
		printf ("NAND的volume serial number是 %d\r\n", ser_num);
	}
	else 
	{
		printf ("Volume访问错误\r\n");
	}
    
    /* 获取Nand Flash剩余容量 */
	capacity = ffree("N0:");
	DotFormat(capacity, (char *)buf);
	printf("NAND剩余容量 = %10s字节\r\n", buf);
    
    /* 获取存储设备的扇区信息 */
	restatus = fs_ioc_read_info (id, &info);
	if(restatus == fsOK)
	{
		/* 总的扇区数 * 扇区大小，SD卡的扇区大小是512字节 */
		capacity = (uint64_t)info.block_cnt << 9;
		DotFormat(capacity, (char *)buf);
		printf("NAND总容量 = %10s字节\r\nSD卡的总扇区数 = %d \r\n", buf, info.block_cnt);
	}
	else
	{
		printf("获取配置信息失败 %s\r\n", ReVal_Table[restatus]);
	}
	
	/* 访问结束要解锁 */
	fs_ioc_unlock (id);

	printf("NAND读扇区大小 = %d字节\r\n", info.read_blen);
	printf("NAND写扇区大小 = %d字节\r\n", info.write_blen);
}

/*
*********************************************************************************************************
*	函 数 名: ViewRootDir
*	功能说明: 显示nand flash根目录下的文件名
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ViewRootDir(void)
{
    uint8_t result;
	fsFileInfo info;
	uint8_t buf[15];
	
    info.fileID = 0;   /* 每次使用ffind函数前，info.fileID必须初始化为0 */
    printf("------------------------------------------------------------------\r\n");
	printf("文件名                 |  文件大小     | 文件ID  | 属性      |日期\r\n");
	
	/* 
	   将根目录下的所有文件列出来。
	   1. "*" 或者 "*.*" 搜索指定路径下的所有文件
	   2. "abc*"         搜索指定路径下以abc开头的所有文件
	   3. "*.jpg"        搜索指定路径下以.jpg结尾的所有文件
	   4. "abc*.jpg"     搜索指定路径下以abc开头和.jpg结尾的所有文件
	
	   以下是实现搜索根目录下所有文件
	*/
	while(ffind ("N0:*.*", &info) == 0)  
	{ 
		/* 调整文件显示大小格式 */
		DotFormat(info.size, (char *)buf);
		
		/* 打印根目录下的所有文件 */
		printf ("%-20s %12s bytes, ID: %04d  ",
				info.name,
				buf,
				info.fileID);
		
		/* 判断是文件还是子目录 */
		if (info.attrib & FS_FAT_ATTR_DIRECTORY){
			printf("(0x%02x)目录", info.attrib);
		}else{
			printf("(0x%02x)文件", info.attrib);
		}
		
		/* 显示文件日期 */
		printf ("  %04d.%02d.%02d  %02d:%02d\r\n",
                 info.time.year, info.time.mon, info.time.day,
               info.time.hr, info.time.min);
    }
	
	if (info.fileID == 0)  
	{
		printf ("卡中没有存放文件\r\n");
	}
}


/*
*********************************************************************************************************
*	函 数 名: CreateNewFile
*	功能说明: 在SD卡创建三个text文本，分别使用fwrite，fprintf和fputs写数据。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void CreateNewFile(void)
{
	const uint8_t WriteText[] = {"武汉安富莱电子有限公司\r\n2015-09-06\r\nwww.armfly.com\r\nWWW.ARMFLY.COM"};
	const uint8_t WriteText1[] = {"武汉安富莱电子有限公司\r"};
	FILE *fout;
	uint32_t bw;
	uint32_t i = 2;
	uint8_t result;
    
	/**********************************************************************************************************/
	/* 打开文件夹test中的文件test1.txt，如果没有子文件夹和txt文件会自动创建*/
	fout = fopen ("N0:\\test\\test1.txt", "w"); 
	if (fout != NULL) 
	{
		/* 写数据 */
		bw = fwrite (WriteText, sizeof(uint8_t), sizeof(WriteText)/sizeof(uint8_t), fout);
		if(bw == sizeof(WriteText)/sizeof(uint8_t)){
			printf("写入内容成功\r\n");
		}else{ 
			printf("写入内容失败\r\n");
		}
		
		/* 关闭文件 */
		fclose(fout);
	}
}



/*
*********************************************************************************************************
*	函 数 名: ReadFileData
*	功能说明: 分别使用fread，fscan和fgets读取三个不同的txt文件。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ReadFileData(void)
{
    const uint8_t WriteText[] = {"武汉安富莱电子有限公司\r\n2015-09-06\r\nwww.armfly.com\r\nWWW.ARMFLY.COM"};
	uint8_t Readbuf[100];
	FILE *fin;
	uint32_t bw;
	uint32_t index1, index2;
	float  count = 0.0f;
	uint8_t result;
    
    	/* 加载SD卡 */
	result = fmount("N0:");
	if(result != NULL)
	{
		/* 如果挂载失败，务必不要再调用FlashFS的其它API函数，防止进入硬件异常 */
		printf("挂载文件系统失败 (%s)\r\n", ReVal_Table[result]);
		goto access_fail;
	}
	else
	{
		printf("挂载文件系统成功 (%s)\r\n", ReVal_Table[result]);
	}
    
    	/* 打开文件夹test中的文件test1.txt */
	fin = fopen ("N0:\\test\\test1.txt", "r"); 
	if (fin != NULL) 
	{
		printf("<1>打开文件M0:\\test\\test1.txt成功\r\n");
		
		/* 防止警告 */
		(void) WriteText;
		
		/* 读数据 方式一 */
		bw = fread(Readbuf, sizeof(uint8_t), sizeof(WriteText)/sizeof(uint8_t), fin);
		if(bw == sizeof(WriteText)/sizeof(uint8_t))
		{
			Readbuf[bw] = NULL;
			printf("test1.txt 文件内容 : \r\n%s\r\n", Readbuf);
		}else{ 
			printf("读内容失败\r\n");
		}
        
        /* 读数据 方式二*/
//		if(fgets((char *)Readbuf, sizeof(Readbuf), fin) != NULL){
//			printf("test1.txt 文件内容 : \r\n%s\r\n", Readbuf);
//		}else{
//			printf("读内容失败\r\n");
//		}
        
//        /* 读数据 方式三*/
//        bw = fscanf(fin, "%d %d %f", &index1, &index2, &count);
//		if (bw == 3)  {
//			printf("读出的数值\r\nindex1 = %d index2 = %d count = %f\r\n", index1, index2, count);
//		}else{
//			printf("读内容失败\r\n");	
//		}
		
		/* 关闭文件 */
		fclose(fin);
	}
    
access_fail:
	/* 卸载SD卡 */
	result = funmount("N0:");
	if(result != NULL)
	{
		printf("卸载文件系统失败\r\n");
	}
	else
	{
		printf("卸载文件系统成功\r\n");
	}
}

/*
*********************************************************************************************************
*	函 数 名: DeleteDirFile
*	功能说明: 删除文件夹和文件
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void DeleteDirFile(void)
{
	fsStatus restatus;
    
    /* 删除文件夹test */
	restatus = fdelete ("N0:\\test\\", NULL);
	if (restatus != NULL) 
	{
		printf("test文件夹非空或不存在\r\n");
	}
	else
	{
		printf("删除test文件夹成功\r\n");
	}
    
}
/***************************************************************************************
  * @brief   
  * @input   
  * @return  
***************************************************************************************/
void NAND_Init(void)
{
    uint8_t result;
    char *opt;
   
    NAND_Reset();
    delay_ms(100);
    NAND_ReadID();	        //读取ID
    result = finit("N0:");
    if(result != NULL){
        printf("初始化文件系统失败 (%s)\r\n", ReVal_Table[result]);/* 如果挂载失败，务必不要再调用FlashFS的其它API函数，防止进入硬件异常 */
    }else{
        printf("初始化文件系统成功 (%s)\r\n", ReVal_Table[result]);
    }
    
	result = fmount("N0:");
	if(result != NULL){
		/* 如果挂载失败，务必不要再调用FlashFS的其它API函数，防止进入硬件异常 */
		printf("挂载文件系统失败 (%s)\r\n", ReVal_Table[result]);
        
        if (result == 0x0B){
            /* 格式化 */
            opt = "/LL /L nand /FAT32";
            printf("文件系统格式中......\r\n");
            result = fformat ("N0:", opt);
            printf("文件系统格式化 (%s)\r\n", ReVal_Table[result]);
        } else return;
	}else{
		printf("挂载文件系统成功 (%s)\r\n", ReVal_Table[result]);
	}

    GetCapacity();
	CreateNewFile();
    
    ViewRootDir();
    
	/* 卸载nand文件系统 */
	result = funmount("N0:");
	if(result != NULL)
	{
		printf("卸载文件系统失败\r\n");
	}
	else
	{
		printf("卸载文件系统成功\r\n");
	}
    
    
    ReadFileData();
    
}
