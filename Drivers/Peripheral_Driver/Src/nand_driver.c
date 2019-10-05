#include "main.h"

#define BUSY_GPIO	GPIOD
#define BUSY_PIN	GPIO_PIN_6

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
*	函 数 名: FMC_NAND_Reset
*	功能说明: 复位NAND Flash
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t FMC_NAND_Reset(void)
{
//	NAND_CMD_AREA = NAND_CMD_RESET;

//	/* 检查操作状态 */
//	if (FMC_NAND_GetStatus() == NAND_READY)
//	{
//		return NAND_OK;
//	}

	return 0;
}


