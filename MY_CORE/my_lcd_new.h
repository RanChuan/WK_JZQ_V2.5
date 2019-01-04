
#ifndef __MY_LCD_NEW_H
#define __MY_LCD_NEW_H

#include "includes.h"

void my_lcd_new(void *t);




	//lcd���ؽ���
void lcd_load(void);
		//��1
void lcd_wizard1(void);
		//��2
void lcd_wizard2(void);
			//��ҳ
void lcd_main(void);
				//�ֶ�����
void lcd_operation(void);
				//����ѡ��
void lcd_setchoose(void);
				//��Դ
void lcd_power(void);

				//��������
void lcd_generalSet(void);

				//��������
void lcd_energySaving(void);

				//ʱ������
void lcd_timeSet(void);

				//��������
void lcd_environmentalControl(void);

				//�������
void lcd_passwordInterface(void);

				//���ڽ���
void lcd_about (void);

				//�߼�����
void lcd_advancedSet(void);



/////////////////////һЩ��������///////////////////////////////

//ת��ҳ��
void lcd_turn_page(u8 page);

//�ı��ַ�������ֵ����ַ�����ݣ����ݳ���
void lcd_change_str(u16 addr,char *data,u16 len);

//�ı�һ������
void lcd_change_value(u16 addr,u16 value);


//���������ش��İ������ݣ����㣬�ɹ���0��ʧ��
u16 lcd_deal_key(u8 *data);

//���������ش����ַ������ݣ�1���ɹ���0��ʧ��
u16 lcd_deal_str(u8 *data,u8 *strout);


		//���Ϳ����豸����豸
void lcd_msg_to_rf (u8 devtype,u8 key_state);


#endif




