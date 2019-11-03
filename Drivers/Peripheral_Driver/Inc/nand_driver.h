#ifndef __NAND_DRIVER_H
#define __NAND_DRIVER_H


//int32_t Driver_NAND0_GetDeviceBusy (uint32_t dev_num);
//uint8_t FMC_NAND_Reset(void);
//uint32_t NAND_ReadID(void);

uint8_t NAND_Reset(void);
uint32_t NAND_ReadID(void);

#endif
