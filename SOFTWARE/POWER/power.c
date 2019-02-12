

#include "includes.h"
#include "file.h"
#define SHCSR    (u32 *)0xE000ED24


		//系统开机
void SysPowerOn (void)
{
	void Sys_Init(void);
	Sys_Init();
	if (os_init()!=0) { //Load_down();
	}//不为0初始化失败，一般是文件系统失败
	
//	u8 *buf=malloc(100);
//	read_json(_T("0:/wk_config.json"),buf,10);
	
}

 

			
void SysPowerOff (void)
{
	LCD_tosleep();
	delay_ms(1000);
	NVIC_SystemReset();
}









void Sys_Init(void)
{
	*SHCSR|=7<<16;
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x5000);
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2800);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	TIME3_Init(); 
	TIME4_Init(); 
	BEEP_Init(); 
	SPI_Flash_Init(); 
	
}	
 










