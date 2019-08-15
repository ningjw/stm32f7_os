使用STemWin生成工程后还需要做以下修改
a. 将GUIConf.c中extMem变量添加__attribute__((at(0xC0100000)))进行修饰
b. 屏蔽cc936.c文件

1. SDRAM空间分配(0xC0000000 - 0xC2000000:32M)

LAYER0_ADDR 0xC0000000 - 0xC003FC00
LAYER1_ADDR 0xC003FC00 - 0xC007F800
extMem      0xC0100000 - 0xC0600000 static U32 extMem[GUI_NUMBYTES / 4] __attribute__((at(0xC0100000)));
mem_base    0xC0600000 - 0xC1A00000
mem_map     0xC1A00000 - 0xC1B40000


