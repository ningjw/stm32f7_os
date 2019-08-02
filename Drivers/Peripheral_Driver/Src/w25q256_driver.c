#include "main.h"

extern QSPI_HandleTypeDef hqspi;

#define W25Q_RESET()             QSPI_SendCmd(RESET_ENABLE_CMD,QSPI_INSTRUCTION_1_LINE,0,0,0,0);\
                                 QSPI_SendCmd(RESET_MEMORY_CMD,QSPI_INSTRUCTION_1_LINE,0,0,0,0)
#define W25QXX_ENTER_QSPI()      QSPI_SendCmd(W25X_EnterQPIMode,QSPI_INSTRUCTION_1_LINE,0,0,0,0)
#define W25Q_WRITE_ENABLE()      QSPI_SendCmd(W25X_WriteEnable,QSPI_INSTRUCTION_4_LINES,0,0,0,0)
#define W25QXX_ENTER_4BYTEADDR() QSPI_SendCmd(W25X_Enable4ByteAddr,QSPI_INSTRUCTION_4_LINES,0,0,0,0)


/***************************************************************************************
  * @brief   发送指令
  * @input   
  * @return  
***************************************************************************************/
void QSPI_SendCmd(uint32_t cmd,uint32_t cmdMode,uint32_t addr,uint32_t addrMode,uint32_t addrSize,uint32_t dataMode)
{
    QSPI_CommandTypeDef s_command = {0};
    
    s_command.Instruction     = cmd;          //指令
    s_command.InstructionMode = cmdMode;	  //指令模式
    s_command.Address         = addr;         //地址
    s_command.AddressMode     = addrMode;     //地址模式
    s_command.AddressSize     = addrSize;     //地址长度
    s_command.DataMode        = dataMode;     //数据模式
    
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
}


/***************************************************************************************
  * @brief   QSPI接收指定长度的数据
  * @input   buf:接收数据缓冲区首地址
             len:要传输的数据长度
  * @return  
***************************************************************************************/
void QSPI_Receive(uint8_t* buf,uint32_t len)
{
    hqspi.Instance->DLR = len - 1;                 //配置数据长度
    if(HAL_QSPI_Receive(&hqspi, buf, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
}


/***************************************************************************************
  * @brief   QSPI发送指定长度的数据
  * @input   buf:发送数据缓冲区首地址
             len:要传输的数据长度
  * @return  
***************************************************************************************/
void QSPI_Transmit(uint8_t* buf,uint32_t len)
{
    hqspi.Instance->DLR = len - 1;                            //配置数据长度
    if(HAL_QSPI_Transmit(&hqspi, buf, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
}


/***************************************************************************************
  * @brief   W25QXX进入QSPI模式
  * @input
  * @return
***************************************************************************************/
void W25QXX_SetQE(void)
{
    uint8_t value = 0x02;
    /* 1.写使能 */
    QSPI_SendCmd(W25X_WriteEnable,QSPI_INSTRUCTION_1_LINE,0,0,0,0);
    /* 2.发送写状态寄存器2命令 */
    QSPI_SendCmd(W25X_WriteStatusReg2,QSPI_INSTRUCTION_1_LINE,0,0,0,QSPI_DATA_1_LINE);
    /* 3.发送数据 */
    QSPI_Transmit(&value, 1);
}


/***************************************************************************************
  * @brief   0XEF18,表示芯片型号为W25Q256
  * @input
  * @return
***************************************************************************************/
uint16_t W25QXX_ReadId(void)
{
    uint8_t pData[2];

    QSPI_SendCmd(W25X_ManufactDeviceID,QSPI_INSTRUCTION_4_LINES,
                 0,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES);
    QSPI_Receive(pData,2);

    return pData[1] | ( pData[0] << 8 );
}

/***************************************************************************************
  * @brief   设置读参数
  * @input   
  * @return  
***************************************************************************************/
void W25QXX_SetReadParam(void)
{
    uint8_t para = 3 << 4;
    
    /* 1.发送命令 */
    QSPI_SendCmd(W25X_SetReadParam,QSPI_INSTRUCTION_4_LINES,0,0,0,QSPI_DATA_4_LINES);
    /* 2.发送数据 */
    QSPI_Transmit(&para, 1);
}


/***************************************************************************************
  * @brief  等待空闲
  * @input   
  * @return  
***************************************************************************************/
void W25QXX_WaitIdle(void)   
{
    uint8_t sta_reg1 = 0x00;
    do{
        QSPI_SendCmd(W25X_ReadStatusReg1,QSPI_INSTRUCTION_4_LINES,0,0,0,QSPI_DATA_4_LINES);
        QSPI_Receive(&sta_reg1,1);
    }while( (sta_reg1&0x01) == 0x01 );
}

/***************************************************************************************
  * @brief   擦除QSPI某个扇区
  * @input   
  * @return  
***************************************************************************************/
void W25QXX_EraseSector(uint32_t sector_id)
{
    uint32_t addr = sector_id * 4096;
	
    W25Q_WRITE_ENABLE();
    W25QXX_WaitIdle();
    QSPI_SendCmd(W25X_SectorErase,QSPI_INSTRUCTION_4_LINES,addr,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,0);
    W25QXX_WaitIdle();
}


/***************************************************************************************
  * @brief   读取SPI FLASH,仅支持QPI模式
  * @input   pBuffer:数据存储区
             ReadAddr:开始读取的地址(最大32bit)
             NumByteToRead:要读取的字节数(最大65535)
  * @return
***************************************************************************************/
void W25QXX_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    QSPI_CommandTypeDef s_command = {0};
    s_command.Instruction       = W25X_FastReadData;
    s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
    s_command.AddressMode       = QSPI_ADDRESS_4_LINES;
    s_command.AddressSize       = QSPI_ADDRESS_32_BITS;
    s_command.DataMode          = QSPI_DATA_4_LINES;
    s_command.DummyCycles       = 8;
    s_command.NbData            = NumByteToRead;
    s_command.Address           = ReadAddr;
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_QSPI_Receive(&hqspi, pBuffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
}




/***************************************************************************************
  * @brief   SPI在一页(0~65535)内写入少于256个字节的数据
  * @input   pBuffer:数据存储区
             WriteAddr:开始写入的地址(最大32bit)
             NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
  * @return  
***************************************************************************************/
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	W25Q_WRITE_ENABLE();				//写使能
    W25QXX_WaitIdle();
    QSPI_SendCmd(W25X_PageProgram,QSPI_INSTRUCTION_4_LINES,
                 WriteAddr,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES);
	QSPI_Transmit(pBuffer, NumByteToWrite);	         	      
	W25QXX_WaitIdle();					//等待写入结束
}

/***************************************************************************************
  * @brief   无检验写SPI FLASH ,具有自动换页功能 
  * @input   pBuffer:数据存储区
             WriteAddr:开始写入的地址(最大32bit)
             NumByteToWrite:要写入的字节数(最大65535)
  * @return  
***************************************************************************************/
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{
	uint16_t page_remain = 256 - WriteAddr % 256; //单页剩余的字节数		
    
	if(NumByteToWrite <= page_remain){
        page_remain = NumByteToWrite;//不大于256个字节
    }
    
	while(1)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, page_remain);
        
		if(NumByteToWrite == page_remain){
            break;//写入结束了
	 	} else {
			pBuffer += page_remain;
			WriteAddr += page_remain;	

			NumByteToWrite -= page_remain;//减去已经写入了的字节数
			if(NumByteToWrite > 256)
                page_remain = 256;        //一次可以写入256个字节
			else 
                page_remain = NumByteToWrite; //不够256个字节了
		}
	}
}

/***************************************************************************************
  * @brief   写SPI FLASH  
  * @input   pBuffer:数据存储区
             WriteAddr:开始写入的地址(最大32bit)	
             NumByteToWrite:要写入的字节数(最大65535)   
  * @return  
***************************************************************************************/
uint8_t W25QXX_Buffer[4096];		 
void W25QXX_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)   
{
	uint32_t sec_id;
	uint16_t sec_offset;
	uint16_t sec_remain;	   
 	uint16_t i;    
	uint8_t * W25Q_Buf = W25QXX_Buffer;	  
    
 	sec_id     = WriteAddr / 4096;//扇区地址  
	sec_offset = WriteAddr % 4096;//在扇区内的偏移
	sec_remain = 4096 - sec_offset;//扇区剩余空间大小   
    
 	if(NumByteToWrite <= sec_remain)//
        sec_remain = NumByteToWrite;//不大于4096个字节
    
	while(1) 
	{
		W25QXX_Read(W25Q_Buf, sec_id*4096, 4096);//读出整个扇区的内容
		for(i=0; i<sec_remain; i++)//校验数据
		{
			if(W25Q_Buf[sec_offset+i] != 0xFF)
                break;//需要擦除  	  
		}
        
		if(i < sec_remain){//需要擦除
			W25QXX_EraseSector(sec_id);//擦除这个扇区
			for(i=0;i<sec_remain;i++){	   //复制
				W25Q_Buf[i+sec_offset] = pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25Q_Buf, sec_id*4096, 4096);//写入整个扇区  
		} else {
            W25QXX_Write_NoCheck(pBuffer,WriteAddr,sec_remain);//写已经擦除了的,直接写入扇区剩余区间. 	
        }
        
		if(NumByteToWrite == sec_remain){
            break;       //写入结束了
        } else {         //写入未结束
			sec_id++;    //扇区地址增1
			sec_offset = 0;//偏移位置为0 	 

		   	pBuffer += sec_remain;       //指针偏移
			WriteAddr += sec_remain;     //写地址偏移	   
		   	NumByteToWrite -= sec_remain;//字节数递减
			if(NumByteToWrite > 4096)
                sec_remain = 4096;	     //下一个扇区还是写不完
			else 
                sec_remain = NumByteToWrite;//下一个扇区可以写完了
		}
	}
}


uint16_t W25_ID;
uint8_t TEXT_Buffer[] = {"abcdefghijklmnopqrstuvwxyz"};
uint8_t W25_RxBuf[50] = {0};
#define SIZE sizeof(TEXT_Buffer)
/***************************************************************************************
  * @brief   W25Q256初始化
  * @input
  * @return
***************************************************************************************/
void W25QXX_Init(void)
{
    W25QXX_SetQE();
    delay_ms(20);
    W25QXX_ENTER_QSPI();
    W25QXX_ENTER_4BYTEADDR();
    W25QXX_SetReadParam();//设置读参数
    
    W25QXX_Write(TEXT_Buffer,W25Q256_SIZE - 100, SIZE);
    W25_ID = W25QXX_ReadId();
    W25QXX_Read(W25_RxBuf, W25Q256_SIZE - 100, SIZE);
}

