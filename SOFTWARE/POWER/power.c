

#include "includes.h"
#include "file.h"
#define SHCSR    (u32 *)0xE000ED24


		//ϵͳ����
void SysPowerOn (void)
{
	void Sys_Init(void);
	Sys_Init();
	if (os_init()!=0) { //Load_down();
	}//��Ϊ0��ʼ��ʧ�ܣ�һ�����ļ�ϵͳʧ��
	
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	TIME3_Init(); 
	TIME4_Init(); 
	BEEP_Init(); 
	SPI_Flash_Init(); 
	
}	
 










