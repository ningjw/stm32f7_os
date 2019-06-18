#include "main.h"


/***************************************************************************************
  * @brief   
  * @input   
  * @return  
***************************************************************************************/
uint8_t SDRAM_Send_Cmd(SDRAM_HandleTypeDef *hsdram, uint8_t cmd,uint8_t refresh,uint16_t regval)
{
    FMC_SDRAM_CommandTypeDef Command;
    Command.CommandMode = cmd;                           //命令
    Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;  //目标SDRAM存储区域
    Command.AutoRefreshNumber = refresh;                 //自刷新次数
    Command.ModeRegisterDefinition = regval;             //要写入模式寄存器的值
    if(HAL_SDRAM_SendCommand(hsdram, &Command, 0x1000)==HAL_OK) {//向SDRAM发送命令
        return 0;  
    }
    else 
        return 1;    
}


/***************************************************************************************
  * @brief   发送SDRAM初始化序列
  * @input   
  * @return  
***************************************************************************************/
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
	uint32_t temp=0;
    //SDRAM控制器初始化完成以后还需要按照如下顺序初始化SDRAM
    SDRAM_Send_Cmd(hsdram, FMC_SDRAM_CMD_CLK_ENABLE, 1, 0); //时钟配置使能
    delay_us(500);                                          //至少延时200us
    SDRAM_Send_Cmd(hsdram, FMC_SDRAM_CMD_PALL, 1, 0);       //对所有存储区预充电
    SDRAM_Send_Cmd(hsdram, FMC_SDRAM_CMD_AUTOREFRESH_MODE, 8, 0);//设置自刷新次数 
    
    //配置模式寄存器,SDRAM的bit0~bit2为指定突发访问的长度，
	//bit3为指定突发访问的类型，bit4~bit6为CAS值，bit7和bit8为运行模式
	//bit9为指定的写突发模式，bit10和bit11位保留位
	temp = SDRAM_MODEREG_BURST_LENGTH_1          |	//设置突发长度:1(可以是1/2/4/8)
           SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |	//设置突发类型:连续(可以是连续/交错)
           SDRAM_MODEREG_CAS_LATENCY_3           |	//设置CAS值:3(可以是2/3)
           SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //设置操作模式:0,标准模式
           SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     //设置突发写模式:1,单点访问
    
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_LOAD_MODE,1,temp);   //设置SDRAM的模式寄存器
    
    //刷新频率计数器(以SDCLK频率计数),计算方法:
	//COUNT=SDRAM刷新周期/行数-20=SDRAM刷新周期(us)*SDCLK频率(Mhz)/行数
    //我们使用的SDRAM刷新周期为64ms,SDCLK=216/2=108Mhz,行数为8192(2^13).
	//所以,COUNT=64*1000*108/8192-20=823
	HAL_SDRAM_ProgramRefreshRate(hsdram, 823);
    
//      for(uint8_t i = 0; i< 255; i++){
//        ltdc_layer0[0][i] = i;
//      }
}








