#include "malloc.h"

uint8_t   mem_rdy; 				//内存管理是否就绪

//内存池(32字节对齐), 外部SDRAM内存池,前面2M给LTDC用了(1280*800*2)
__align(32) uint8_t mem_base[MEM_MAX_SIZE] __attribute__((at(0xC0600000)));	

//内存管理表
uint32_t mem_map[MEM_MAP_SIZE] __attribute__((at(0xC0600000 + MEM_MAX_SIZE)));

/***************************************************************************************
  * @brief   内存管理初始化  
  * @input   
  * @return  
***************************************************************************************/
static void mem_init(void)
{
    memset(mem_map, 0, MEM_MAP_SIZE *4);	//内存状态表数据清零  
 	mem_rdy = 1;						//内存管理初始化OK  
}


/***************************************************************************************
  * @brief   内存分配(内部调用)
  * @input   size:要分配的内存大小(字节)
  * @return  返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址 
***************************************************************************************/
static uint32_t sdram_malloc(uint32_t size)  
{
    signed long offset=0;  
    uint32_t nmemb;	//需要的内存块数  
	uint32_t cmemb = 0;//连续空内存块数
    uint32_t i;
    if( !mem_rdy ){
        mem_init();//未初始化,先执行初始化 
    }
    if(size == 0){
        return 0xFFFFFFFF;//不需要分配
    }
    nmemb = size / MEM_BLOCK_SIZE;//获取需要分配的连续内存块数
    if(size % MEM_BLOCK_SIZE){
        nmemb++;  
    }
    for(offset = MEM_MAP_SIZE - 1; offset >= 0; offset--)//搜索整个内存控制区  
    {
		if( !mem_map[offset] )
            cmemb++;//连续空内存块数增加
		else
            cmemb=0;								//连续内存块清零
        
		if(cmemb == nmemb){							//找到了连续nmemb个空内存块
            for(i=0; i < nmemb; i++) { 					//标注内存块非空 
                mem_map[offset+i] = nmemb;  
            }
            return (offset * MEM_BLOCK_SIZE);//返回偏移地址  
		}
    }  
    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
}


/***************************************************************************************
  * @brief   释放内存(内部调用) 
  * @input   memx:所属内存块;
             offset:内存地址偏移
  * @return  返回值:0,释放成功;1,释放失败;  
***************************************************************************************/
static uint8_t sdram_free(uint32_t offset)  
{
    int i;
    if( !mem_rdy )//未初始化,先执行初始化
	{
		mem_init();    
        return 1;//未初始化  
    }
    if(offset < MEM_MAX_SIZE)                 //偏移在内存池内. 
    {  
        int index = offset / MEM_BLOCK_SIZE;  //偏移所在内存块号码  
        int nmemb = mem_map[index];	  //内存块数量
        for(i=0; i < nmemb; i++)  			  //内存块清零
        {
            mem_map[index+i] = 0;  
        }  
        return 0;  
    }else 
        return 2;//偏移超区了.  
}  


/***************************************************************************************
  * @brief   释放内存(外部调用) 
  * @input   ptr:内存首地址 
  * @return  
***************************************************************************************/
void mem_free(void *ptr)  
{
	uint32_t offset;   
	if(ptr == NULL) 
        return;//地址为0.  
 	offset = (uint32_t)ptr - (uint32_t)mem_base;     
    sdram_free(offset);	//释放内存      
}


/***************************************************************************************
  * @brief   分配内存(外部调用)
  * @input   size:内存大小(字节)
  * @return  
***************************************************************************************/
void *mem_malloc(uint32_t size)  
{
    uint32_t offset;   
	offset = sdram_malloc(size);  	   	 	   
    if(offset == 0XFFFFFFFF)
        return NULL;  
    else 
        return (void*)((uint32_t)mem_base + offset);  
}



