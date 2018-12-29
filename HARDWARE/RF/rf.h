#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#include "data_map.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define RF_AUX PAin(8)
#define RF_M0 PAout(12)
#define RF_M1 PAout(11)
#define USART_REC_LEN  			50  	//�����������ֽ��� 200






				//��ʱʱ��
#define TIME_OUT  10
					//��ʱ���鳤��
#define BUFF_LENGTH 40

#define DEVICE_DATA_NUM 20


					//�豸״̬
#define DEVICEON 				0x0100
#define DEVICEUP 				0x0200
#define DEVICEDOWN 			0x0400
#define DEVICEOFFLINE 	0x0800


						//�豸�������������������͵ĸ�ʽ
						//�������ͨ����ʼ���������
extern u16 DEVICE_NUM[DEVICE_DATA_NUM];


extern u8 EN_DATA[25];




//------------------------���ⲿ���õĺ���---------------------------


					//��ȡ�ɼ��������׵�ַ
u8 *GetCJDataAddr (void);

					//��ȡ������ַ
u16 Get_MyAddr(void);

					//���ñ�����ַ
void Set_MyAddr(u16 addr);

				//�����豸��������ʽ�ǰ����豸��ַ��8λ����8λ������,len,�豸����
void Set_Device(u8 * data,u16 len);

				//����ip��ַ
void Set_MyIP (u8 *data);
					//��ȡip��ַ
u16 * Get_MyIP (void);





//------------------------���ⲿ���õĺ���End---------------------------







				//�������߷��ͽ���
void RF_SetFocus(u8 focus);




void RF_Init(u32 bound);//��ʼ��
u16 RF1_tx_bytes( uint8_t* TxBuffer, uint8_t Length );
u16 RF_GetCmd (uint8_t *buff,u16 len);


				//�����������õ�ʱ����
u16 RF_Get (u8 *buff,u16 len);

u16  Get_Mycmd(u8 *data);
void Clear_Mycmd(void);

	//�����ŵ�
u8 RF_SetChannel(u8 channel);



				//��������
void copy_data (u8 *from,u8 *to,u16 length);



#endif
