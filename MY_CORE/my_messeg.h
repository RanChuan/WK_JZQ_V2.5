#ifndef __MY_MESSEG_H
#define __MY_MESSEG_H
#include "sys.h"
#include "os_msg.h"


/****************************************************************************
									/																													/
									/			����ļ��ﶨ����Ϣ�ľ����������ĺ���							/
									/			���嵽��������֮���ͨ��														/
									/			����ʱ��2018.11.16																	/
									/			�����������ǰд�ĳ�����Ϣ����û��ʹ�����µĺ궨��	/
									/			���³���Ŀɶ��ԱȽϲ�Ժ�д�ĳ�����Ϣ���ݲ���ʹ	/
									/			�����µĺ궨�壬��߳���ɶ���											/
									/			ÿ���Զ������񶼰������ͷ�ļ������ͷ�ļ���������	/
									/			����Ϣ������Ҫ�õ��Ĳ���														/
									/			
*****************************************************************************/



#define  KEY_MESSEG  1
#define  LCD_MESSEG  2
#define  LIT_MESSEG  4
#define  RF_MESSEG   5
#define  WK_MESSEG   7//����
#define  CFG_MESSEG  9

//#define  ET_MESSEG   2




//RFͨ���еĴ������Ͷ���

#define ERR_SUCCESS         0x0000			//�ɹ�
#define ERR_FAIL						0x0001			//δ֪����
#define ERR_DATAVALE				0x0002			//���ݴ���
#define ERR_DATALENGTH			0x0003			//���ݳ��ȴ���
#define ERR_WANTLENGTH			0x0004			//��Ҫ��ȡ�����ݳ��ȴ���
#define ERR_NULLCMD					0x0005			//��֧�ֵ��������ͣ�
#define ERR_NOCONNECT				0x0006			//û�����ӣ�һ�в�����Ҫ��ͨ�ſ�ʼ֮��
#define ERR_NOCJADDR				0x0007			//�ɼ����ĵ�ַ���Ǳ����󶨵Ĳɼ�����ַ
#define ERR_CANNOTCFG				0x0008			//�ڲ��ǿ�����״̬���յ�����������
#define ERR_NOTMYTYPE				0x0009			//��Ҫ���Ƶ��豸�뱾������
#define ERR_TIMEOUT					0x000a   		//��ʱ
#define ERR_OFFLINE         0x000b   		//�豸������
#define ERR_NONEADDR				0x000c      //�豸��ַ����
#define ERR_ADDRCROSS				0x000d			//����豸��ʱ���ַ�����˵�ַ��





/*************************************************

			�����������Ϣ���壺
			meg0��1�ı䰴��״̬��
								ȡֵ��KEY_CHANGE_STATE
			meg1���ı�İ���״̬λ��
								ȡֵ��KEY_WENDU_UP,
											KEY_WENDU_DOWN,
											KEY_SHIDU_UP,
											KEY_SHIDU_DOWN,
											KEY_LCD_STATE,
											KEY_JINHUA
			meg2��״̬
								ȡֵ��0~255
			

*****************************************************/

#define  KEY_CHANGE_STATE  		 1

#define  KEY_WENDU_UP  			0x01
#define  KEY_WENDU_DOWN 		0x02
#define  KEY_SHIDU_UP  			0x03
#define  KEY_SHIDU_DOWN 		0x04
#define  KEY_LCD_STATE 			0x05
#define  KEY_JINHUA  				0x06

/********************��������End******************************/




/*****************************************

		�ƹ��������Ϣ���壺
		meg[0],1,�����ϵĵƣ�2���豸״ָ̬ʾ��3���ƴ���4�������ϵĵ���˸��5,���°�������ɫ��ʾ
						ȡֵ��LIGHT_DEVICE_STATE
									LIGHT_WARN_STATE
									LIGHT_ROUND_LIGHT
									LIGHT_UPDATE_KEY
									
		meg[1],������,�ƴ���˸�����ͣ�1��������ˮ��2��������ˮ��3������˫��ˮ��4������˫��ˮ��5����ɫ,
						���̵���˸��ʱ����˸��λ��
						ȡֵ����meg[0]=LIGHT_DEVICE_STATE��meg[0]=LIGHT_WARN_STATEʱ
											LIGHT_LIGHT_ON
											LIGHT_LIGHT_OFF
									��meg[0]=LIGHT_ROUND_LIGHTʱ
											LIGHT_LIGHT_R
											LIGHT_LIGHT_L
											LIGHT_LIGHT_DR
											LIGHT_LIGHT_DL
											LIGHT_LIGHT_COLOR
									��meg[0]=LIGHT_UPDATE_KEYʱ
											LIGHT_LIGHT_UPDATE
											LIGHT_LIGHT_UPDATEBY
											LIGHT_LIGHT_GETTO
		meg2��������ڼ���,�ƴ���˸1,��ʼ��0��ֹͣ��2���Զ���ɫ
						���̵���˸��ʱ����˸������
						ȡֵ����meg[0]=LIGHT_DEVICE_STATE��meg[0]=LIGHT_WARN_STATEʱ
											LIGHT_WENDU_UP
											LIGHT_WENDU_DOWN
											LIGHT_SHIDU_UP
											LIGHT_SHIDU_DOWN
											LIGHT_LCD_STATE
											LIGHT_JINHUA
									��meg[0]=LIGHT_ROUND_LIGHTʱ
											LIGHT_LIGHT_START
											LIGHT_LIGHT_STCOR
											LIGHT_LIGHT_STOP
									��meg[0]=LIGHT_UPDATE_KEYʱ
											����4���ֽڱ�ʾ�������ĵ�ַ
		meg3,�ƴ���ɫ
						���̵���˸��ʱ������ʱ��(��λ��10ms)
		meg4���ƴ���ɫ
						���̵���˸��ʱ�����ʱ��
		meg5���ƴ���ɫ
						���̵���˸��ʱ����������ֹͣ

***************************************/


#define LIGHT_DEVICE_STATE  		1//�����ϵĵ�,�豸����״ָ̬ʾ
#define LIGHT_WARN_STATE  			2//������Χ�ĵƣ�����״̬
#define LIGHT_ROUND_LIGHT   		3//��Χ�Ļ��Ƶ�
#define LIGHT_UPDATE_KEY	   		5//���°�������ɫ��ʾ

#define LIGHT_LIGHT_ON					1//�ƹ⣬��
#define LIGHT_LIGHT_OFF					0//�ƹ⣬��
#define LIGHT_LIGHT_R						1//���Ƶƹ⣬������ˮ
#define LIGHT_LIGHT_L						2//���ĵƹ⣬������ˮ
#define LIGHT_LIGHT_DR					3//���Ƶƹ⣬����˫��ˮ
#define LIGHT_LIGHT_DL					4//���Ƶƹ⣬����˫��ˮ
#define LIGHT_LIGHT_COLOR				5//���Ƶƹ⣬��ɫ
#define LIGHT_LIGHT_UPDATE			1//����Ϊ���������ɫ
#define LIGHT_LIGHT_UPDATEBY		2//����Ϊָ�������ַ����ɫ
#define LIGHT_LIGHT_GETTO				3//��ȡ�������ַ����ɫ��ָ�������ַ

#define  LIGHT_WENDU_UP  			0x01//λ�ã����°�ť��
#define  LIGHT_WENDU_DOWN 		0x02//λ�ã����°�ť��
#define  LIGHT_SHIDU_UP  			0x03//λ�ã���ʪ��ť��
#define  LIGHT_SHIDU_DOWN 		0x04//λ�ã���ʪ��ť��
#define  LIGHT_LCD_STATE 			0x05//λ�ã�LCD����ť��
#define  LIGHT_JINHUA  				0x06//λ�ã�����������ť��
#define LIGHT_LIGHT_START				 1//���Ƶƹ⣬��ʼ��̬����
#define LIGHT_LIGHT_STCOR				 2//���Ƶƹ⣬��ʼ�Զ���ɫ����
#define LIGHT_LIGHT_STOP				 0//���Ƶƹ⣬ֹͣ


/********************�ƹ�����End******************************/



/*****************************************

		��Ļ�������Ϣ���壺
		meg[0],1,��ʾ������Ϣ��2���豸״ָ̬ʾ3����Ļ��Դ���ƣ�4����ʾ��ʪ��5,�����ȱ���
						ȡֵ��LCD_SHOW_WARN
									LCD_DEVICE_STATE
									LCD_LCD_POWER
									LCD_DATAUP
									LCD_BEEP
		meg[1],����أ�������Ļ��Դʱ1�������ɿ�����Ļ���ȣ�0����Դ����,����������ʱ1���̣�2����
						ȡֵ��meg[0]=	LCD_DEVICE_STATEʱ
										LCD_STATE_ON
										LCD_STATE_OFF
									meg[0]=LCD_LCD_POWERʱ
										LCD_POWER_ON
										LCD_POWER_OFF
									meg[0]=LCD_BEEPʱ
										LCD_BEEP_SHORT
										LCD_BEEP_LONG
		meg2��������ڼ���,���Ƶ�Դ����ʱ����Ļ����
						ȡֵ��meg[0]=	LCD_DEVICE_STATEʱ
										LCD_WENDU_UP
										LCD_WENDU_DOWN
										LCD_SHIDU_UP
										LCD_SHIDU_DOWN
										LCD_JINHUA
										LCD_HAND
									meg[0]=LCD_LCD_POWER��meg[1]=LCD_POWER_ONʱ
										���ȣ�0~63
		meg3, �¿ظ�8λ
		meg4���¶ȵ�8λ
		meg5��ʪ�ȸ�8λ
		meg6��ʪ�ȵ�8λ
		meg7��tvoc��8λ
		meg8��tvoc��8λ

***************************************/


#define LCD_SHOW_WARN  				1//��ʾ������Ϣ
#define LCD_DEVICE_STATE     	2//�����豸״ָ̬ʾ
#define LCD_LCD_POWER					3//��Ļ��Դ����
#define LCD_DATAUP						4//�����ϴ�����ʪ��TVOC
#define LCD_BEEP							5//�����ȱ���

#define LCD_STATE_ON					1//״̬��
#define LCD_STATE_OFF					0//״̬��
#define LCD_POWER_ON					1//��Ļ��Դ��
#define LCD_POWER_OFF					0//��Ļ��Դ��
#define LCD_BEEP_SHORT				1//�����϶̵�������
#define LCD_BEEP_LONG					2//�����ϳ���������

#define  LCD_WENDU_UP  			0x01//λ�ã����°���
#define  LCD_WENDU_DOWN 		0x02//λ�ã����°���
#define  LCD_SHIDU_UP  			0x03//λ�ã���ʪ����
#define  LCD_SHIDU_DOWN 		0x04//λ�ã���ʪ����
#define  LCD_JINHUA  				0x05//λ�ã���������
#define  LCD_HAND  					0x06//λ�ã��ֶ��Զ��л�����

/********************LCD����End******************************/




/***********************************************************

		���߿����������Ϣ���壺
		meg[0],�豸����,����Ϊ0��0xff,����ָ����ַ���豸
				ȡֵ��RF_DEVICE_CJ
							RF_DEVICE_KT
							RF_DEVICE_CS
							RF_DEVICE_JH
							RF_DEVICE_JS
							RF_DEVICE_YT
							RF_DEVICE_ADDR
		meg[1],1��ȡ�豸״̬��2�������豸,3����������豸
				ȡֵ��RF_CTRL_DEVICE
							RF_GET_DEVICE
							RF_ADD_DEVICE
		meg[2],�豸��ַ�߰�λ
		meg[3],�豸��ַ�ڰ�λ
		meg[4],����ǿ��ƣ�����״̬��0���أ�1������2������
				ȡֵ��meg[1]=RF_CTRL_DEVICE ʱ
								RF_DEVICE_ON
								RF_DEVICE_OFF
								RF_DEVICE_CH
		meg[5],����ǿ��ƣ�����״̬������ʱ�İ�����1������2����
				ȡֵ��meg[1]=RF_CTRL_DEVICE ʱ
								RF_DEVICE_UP
								RF_DEVICE_DOWN

***********************************************************/

#define RF_DEVICE_CJ				1//�豸���Ͳɼ���
#define RF_DEVICE_KT				2//�豸���Ϳյ�������
#define RF_DEVICE_CS				3//�豸���ͳ�ʪ��������
#define RF_DEVICE_JH				4//�豸���;�����������
#define RF_DEVICE_JS				5//�豸���ͼ�ʪ��������
#define RF_DEVICE_YT				6//�豸����һ���������
#define RF_DEVICE_ADDR	 0xff//�������豸���ͣ�ͨ���豸��ַ��ȷ��

#define RF_CTRL_DEVICE			2//�����豸
#define RF_GET_DEVICE				1//��ȡ�豸״̬
#define RF_ADD_DEVICE				3//����������豸

#define RF_DEVICE_ON				1//�豸��
#define RF_DEVICE_OFF				0//�豸��
#define RF_DEVICE_CH				2//���򣬿�ת��Ϊ�أ���װ��Ϊ��

#define RF_DEVICE_UP				1//�豸״̬��
#define RF_DEVICE_DOWN			2//�豸״̬��

/********************RF����End******************************/




/*******************************************

			����W5500��Ϣ���壺
			msg[0],1,���жϣ�2������Ϣ����.3��������
				ȡֵ��WK_INTERRUPT
							WK_SEND_DATA
							WK_REINIT
			msg[1],������Ϣʱ�����͵��豸����
				ȡֵ��WK_DEVICE_CJ
							WK_DEVICE_KT
							WK_DEVICE_CS
							WK_DEVICE_JH
							WK_DEVICE_JS
							WK_DEVICE_YT
			msg[2]���豸״̬��0�����ߣ�1������
				ȡֵ��WK_DEVICE_ONLINE
							WK_DEVICE_OFFLINE
			������ʱ��msg[1]��=WK_DEVICE_CJ
			msg3,id,��8λ
			msg4,id,��8λ
			msg5���豸״̬������״̬��1����
				ȡֵ��WK_DEVICE_ON
							WK_DEVICE_OFF
			msg6������״̬��1������2����
				ȡֵ��WK_STATE_UP
							WK_STATE_DOWN
			�ɼ���ʱ��msg[1]=WK_DEVICE_CJ
			msg3~6���ɼ������ݻ�������ַ���ɸߵ���
			

*********************************************/


#define WK_INTERRUPT  1//���жϷ���
#define WK_SEND_DATA	2//ͨ�����ڷ�����Ϣ
#define WK_REINIT			3//�������³�ʼ��

#define WK_DEVICE_CJ				1//�豸���Ͳɼ���
#define WK_DEVICE_KT				2//�豸���Ϳյ�������
#define WK_DEVICE_CS				3//�豸���ͳ�ʪ��������
#define WK_DEVICE_JH				4//�豸���;�����������
#define WK_DEVICE_JS				5//�豸���ͼ�ʪ��������
#define WK_DEVICE_YT				6//�豸����һ���������

#define WK_DEVICE_ONLINE		0//�豸����
#define WK_DEVICE_OFFLINE		1//�豸������

#define WK_DEVICE_ON				1//�豸����״̬
#define WK_DEVICE_OFF				0//�豸�ر�״̬

#define WK_STATE_UP					1//�豸��
#define WK_STATE_DOWN				2//�豸��

/********************��������End******************************/





/*********************************************

		����������Ϣ����λ��
		msg0,1����������ģʽ��0���˳�����ģʽ,2��ת����ǰ��״̬
				ȡֵ��CFG_ENTER_CFGMODE
							CFG_EXIT_CFGMODE
							CFG_CHANGE_CFGMODE

***********************************************/


#define CFG_ENTER_CFGMODE 		1//��������ģʽ
#define CFG_EXIT_CFGMODE			0//�˳�����ģʽ
#define CFG_CHANGE_CFGMODE		2//ת����ǰ״̬��������ģʽʱ�˳�������ʱ����

/********************��������End******************************/




	//�Ѵ���ת��Ϊ�ַ���
char *err_to_str(u16 err);





#endif
















