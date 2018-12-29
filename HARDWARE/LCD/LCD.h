#ifndef __LCD_H
#define __LCD_H


#include "sys.h"
#include "data_map.h"






				//��������ҳ�������λ
#define WENDU_UPLIMIT  		LCD_DATA[0]
#define WENDU_DOWNLIMIT  	LCD_DATA[1]
#define SHIDU_UPLIMIT  		LCD_DATA[2]
#define SHIDU_DOWNLIMIT  	LCD_DATA[3]
#define JINHUA_UPLIMIT  	LCD_DATA[4]
#define QUHAO  						LCD_DATA[5]
#define LCD_POWER_HS    	LCD_DATA[6]
#define LCD_POWER_SS    	LCD_DATA[7]
#define LCD_POWER_HE    	LCD_DATA[8]
#define LCD_POWER_SE    	LCD_DATA[9]
#define LCD_AUTOSLEEP   	LCD_DATA[10]

				//����߼����õ�����λ
#define CHUSHI_ADDR   		LCD_DATA[10]
#define KONGTIAO_ADDR 		LCD_DATA[11]
#define JINGHUA_ADDR  		LCD_DATA[12]
#define MY_ADDR						LCD_DATA[13]


				//�����ֶ���������λ
#define CHUSHI_STATE 			LCD_DATA[14]
#define KONGTIAO_STATE 		LCD_DATA[15]
#define JINHUA_STATE  		LCD_DATA[16]
#define HAND_STATE   			LCD_DATA[17]




//----------------------���ⲿ���õĺ���-------------------

					//��ȡ�Ƿ��ֶ���״̬
u16 Lcd_GetHandstate (void);
												//��ȡ����ֵ���޶ȣ�0���¶����ޣ�1���¶����ޣ�2��ʪ�����ޣ�3��ʪ�����ޣ�4��tvoc����
u16 Lcd_GetLimitData (u16 type);
u16 Lcd_SetLimitData (u16 type,u16 newlimit);

					//�ı��ֶ���״̬
void Lcd_SetHandstate (u16 handstate);


//---------------------���ⲿ���õĺ���End-------------------






void LCD_Init(u32 bound);

void LCD_Send_Data(u8 *buf,u8 len);

void LCD_Receive_Data(u8 *buf,u16 *len);


void LCD_SetFocus(u8 focus);








#endif









