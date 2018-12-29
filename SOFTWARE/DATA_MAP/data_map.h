
#ifndef __DATA_MAP_H
#define __DATA_MAP_H

#include "stmflash.h"


					//������Ļ���õ����ݳ���
#define LCD_DATA_NUM 20

					//�����������ݳ���
#define CONFIG_DATA_NUM 200


					//���ò����ܳ���
#define ALL_PARA_LENGTH (LCD_DATA_NUM+CONFIG_DATA_NUM)


//����һ���ܵĴ洢������Ϣ�ı�������
extern u16 ALL_PARAMETERS[ALL_PARA_LENGTH];

					//������Ļ������
#define LCD_DATA (&ALL_PARAMETERS[0])

						//������������
#define EN_CONFIG (&ALL_PARAMETERS[LCD_DATA_NUM])



//�Ҽӵ����ڴ洢������Ϣ�ĺ���
void Save_Config (void);
void Load_Config(void);
						//������Ļ������
void Save_LCD (void);

void Load_LCD(void);





#endif






