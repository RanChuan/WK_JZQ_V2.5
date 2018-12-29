#ifndef __MY_LCD_H
#define __MY_LCD_H
#include "lcd.h"





#define  PAGE_MAIN    0x01		//��ҳ
#define  PAGE_GSET    0x02		//�߼�����
#define  PAGE_PASS    0x03		//����





			//��Ļ��ʼ��
void my_lcd_init (void);


			//����һ��ҳ��
void lcd_page (u8 page);

				//�ı�һ�����ص�״̬
void lcd_chan_state (u8 type,u8 state);


void Set_Init(void);//���ó�ʼ��

void Hand_Set_Init(void);//�ֶ����ó�ʼ������

void lcd_set_data (u8 type,u16 data);

void Set_Save(void);//����ҳ�汣��

void Auto_sleep();//����ʱ���
					//��Ļ��Ϣ����02
void lcd_messeg_02 (u8 *meg);
void	lcd_messeg_03 (u8 *meg);
			//�����ȱ���
void lcd_beep(u8 l);

				//��Ļ�ϴ���ʪ������
void lcd_dataUp (u8 *);

						//��Ļ˯�߰���
void lcd_sleep(void);

void my_lcd (void *);

void Time_Set();//����ʱ�����ñ���


void BUT_UP_TEMP(void);//�ֶ��������°���
void BUT_DOWN_TEMP(void);//�ֶ����ý��°���
void BUT_UP_HUMI(void);
void BUT_DOWN_HUMI(void);
void BUT_PM(void);
				//�ֶ��Զ�ת��
void AutoCtr(void);

void Into_Senior(u8 *buf);//����߼�����

void lcd_timetosleep(void);//�Զ�����������Լ5���ж�һ��


				//����˯��
void LCD_tosleep (void);

				//��������
void LCD_tolight (void);

//u16 Lcd_GetLimitData (u16 type);

//u16 Lcd_SetLimitData (u16 type,u16 newlimit);

#endif


