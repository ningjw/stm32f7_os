#ifndef __MALLOC_H
#define __MALLOC_H

#include "stdint.h"
#include "string.h"

#ifndef NULL
#define NULL 0
#endif

//SDRAM内存参数设定
#define MEM_MAX_SIZE			20*1024*1024  					//最大管理内存20M
#define MEM_BLOCK_SIZE			64  	  						//内存块大小为64字节
#define MEM_MAP_SIZE        	(MEM_MAX_SIZE / MEM_BLOCK_SIZE)   //内存表大小


//用户调用函数
void sdram_mem_free(void *ptr)  ;  			//内存释放(外部调用)
void *sdram_mem_malloc(uint32_t size);			//内存分配(外部调用)
#endif
