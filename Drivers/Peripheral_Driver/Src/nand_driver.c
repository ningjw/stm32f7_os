#include "stm32f7xx_hal.h"

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

