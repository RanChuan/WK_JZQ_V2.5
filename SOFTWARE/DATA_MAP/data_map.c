
#include "data_map.h"

/************************************

����ļ�����ϵͳ�����Ĵ洢�ռ����
������Ҫ�õ�FLASH�洢�Ĳ���
�ڴ����������ļ�����
������
		��Ļ���ò���
		�������ò���
		�������ò���

**************************************/

//����һ���ܵĴ洢������Ϣ�ı�������
u16 ALL_PARAMETERS[ALL_PARA_LENGTH];


//u16 LCD_DATA[LCD_DATA_NUM]={0};


			//��������������,��������ַ���ɼ�����ַ����������ַ
			//����[0]��ʾ�豸��ַ��[1]��8λ��ʾ�豸���͸�ʽ����8λ���豸״̬
			//�豸״̬λ��0����ʾ����״̬����1,2��λ��ʾ����״̬����3��λ��ʾ����״̬,
			//�豸��ַ���ռ��������ɼ�������������˳��
//u16 EN_CONFIG[CONFIG_DATA_NUM]={1,0,2,1,3,3,4,2,5,4,6,5};

 
 
 
 
#define FLASH_SAVE_ADDR  (0x8040000-1024)
#define FLASH_LCD_ADDR FLASH_SAVE_ADDR
#define FLASH_CFG_ADDR (FLASH_SAVE_ADDR+LCD_DATA_NUM*2)
#define DEVICEOFFLINE 	0x0800


void Save_Config (void)
{
	STMFLASH_Write(FLASH_CFG_ADDR,EN_CONFIG,CONFIG_DATA_NUM);
}

void Load_Config(void)
{
	u16 i=0;
	STMFLASH_Read(FLASH_CFG_ADDR,EN_CONFIG,CONFIG_DATA_NUM);
	for (i=0;i<CONFIG_DATA_NUM;i++)
	{
		if (EN_CONFIG[i]==0xffff) EN_CONFIG[i]=0;
		if (i&1) {EN_CONFIG[i]&=0x00ff;//��ʼ��֮�󣬰��豸״̬Ϊ0��2018.8.18
			if (EN_CONFIG[i]&0x00ff) EN_CONFIG[i]|=DEVICEOFFLINE;//��ʼ��Ϊ����״̬��2018.8.22
		}
	}
	EN_CONFIG[1]|=0x0100;//��������ʼ��Ϊ����
}
 
 
				//�����ֶ���������λ
#define CHUSHI_STATE 			LCD_DATA[14]
#define KONGTIAO_STATE 		LCD_DATA[15]
#define JINHUA_STATE  		LCD_DATA[16]
#define HAND_STATE   			LCD_DATA[17]

						//������Ļ������
void Save_LCD (void)
{
	STMFLASH_Write(FLASH_LCD_ADDR,LCD_DATA,LCD_DATA_NUM);
}


void Load_LCD(void)
{
	u16 i=0;
	u16 tt[4]={0};
	tt[0]=CHUSHI_STATE;
	tt[1]=KONGTIAO_STATE;
	tt[2]=JINHUA_STATE;
	tt[3]=HAND_STATE;
	STMFLASH_Read(FLASH_LCD_ADDR,LCD_DATA,LCD_DATA_NUM);
	for (i=0;i<LCD_DATA_NUM;i++)
	{
		if (LCD_DATA[i]==0xffff)
			LCD_DATA[i]=0;
	}
	CHUSHI_STATE=tt[0];
	KONGTIAO_STATE=tt[1];
	JINHUA_STATE =tt[2];
//	HAND_STATE  =tt[3];ע����һ���ֶ��Զ�ģʽ���籣��
	
}
 
 

