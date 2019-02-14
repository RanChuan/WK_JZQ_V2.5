#include "my_key.h"
#include "my_light.h"
#include "includes.h"
#include "beep.h"
#include "my_lcd.h"
#include "my_messeg.h"


/************************************

			megλ����
			meg0��1�ı䰴��״̬��
			meg1���ı�İ���״̬λ��
			meg2��״̬

		��ϸ����Ϣ����μ�my_messeg.h�ļ�

**********************************/



	static u8 key[6]={0};
	static u8 state[6]={0,0,0,0,0x3f,0};//�̰�״̬
	static u8 statel[6]={0,0,0,0,1,0};//����״̬

u8 key_color[3][6]={0};
	
					//��ȡ����״̬
u8 get_keystate(u8 key_num)
{
	return state[key_num-1];
}
	


			//����������
void my_key (void *t)
{
	u8 meg[MESSEG_DATA]={0};
	u8 light[MESSEG_DATA]={0};
	u8 i=0;
	
	u8 recv[MESSEG_DATA]={0};
	Key_Init();
	for (i=0;i<6;i++)
	{
		key[i]=Get_Key (i+1);
		key[i]=PRESS_NONE;
	}
	while(1)
	{
		TaskGetMsg();
		if (get_messeg(KEY_MESSEG,recv)==0)//����Ļ�Ϲ���֮�󿪲���bug��2018.11.5
		{
			if (recv[0]==1)
			{
				state[recv[1]-1]=recv[2];
			}
		}
		for (i=0;i<6;i++)
		{
			key[i]=PRESS_NONE;
			key[i]=Get_Key (i+1);
			if (key[i]==PRESS_SHORT)
			{
				light[0]=5;//��Ļ��������
				light[1]=1;
				send_messeg(LCD_MESSEG,light);
				
				light[0]=LIGHT_ROUND_LIGHT;
				light[2]=LIGHT_LIGHT_STOP;
				light[1]=LIGHT_LIGHT_RUNTO;
				send_messeg(LIT_MESSEG,light);
			}
			else if (key[i]==PRESS_LONG)
			{
				light[0]=5;//��Ļ��������
				light[1]=2;
				send_messeg(LCD_MESSEG,light);
				
				light[0]=LIGHT_ROUND_LIGHT;
				light[2]=LIGHT_LIGHT_STOP;
				light[1]=LIGHT_LIGHT_RUNTO;
				send_messeg(LIT_MESSEG,light);
			}
			else if (key[i]==PRESS_DOWN)
			{
				light[0]=LIGHT_ROUND_LIGHT;
				light[2]=LIGHT_LIGHT_STCOR;
				light[1]=LIGHT_LIGHT_RUNTO;
				light[3]=255;
				light[4]=255;
				light[5]=255;
				send_messeg(LIT_MESSEG,light);
			}
			else
			{
			}
		}
		
		if (key[0]==PRESS_SHORT) //����1�̰�
		{
			meg[0]=2;//�յ�
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//����ͨ�ò���
			meg[5]=1;//������
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[0]==PRESS_LONG)//����1����
		{
			if (statel[0]==0)
			{
				TaskPend(8);//�����Զ�����
				Load_down();
			}
			else
			{
				TaskRepend(8);//�Զ�����
				Load_up();
			}
			statel[0]=!statel[0];
		}

		if (key[1]==PRESS_SHORT) //����2�̰�
		{
			meg[0]=2;//�յ�
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//����ͨ�ò���
			meg[5]=2;//������
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[1]==PRESS_LONG)//����2����
		{
			if (statel[1]==0)
			{
				meg[0]=3;meg[1]=4;//�ʵ�
				meg[2]=2;//��˸
				meg[3]=200;meg[4]=100;meg[5]=50;//ɫ��ֵ
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=1;
				Lcd_SetHandstate(1);
			}
			else if (statel[1]==1)
			{
				meg[0]=3;meg[1]=3;//�ʵ�
				meg[2]=2;//��˸
				meg[3]=200;meg[4]=100;meg[5]=50;//ɫ��ֵ
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=2;
				Lcd_SetHandstate(1);
			}
			else if (statel[1]==2)
			{
				meg[0]=3;meg[1]=4;//�ʵ�
				meg[2]=0;//ֹͣ
				meg[3]=200;meg[4]=100;meg[5]=50;//ɫ��ֵ
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=0;
				Lcd_SetHandstate(0);
			}
		}


		if (key[2]==PRESS_SHORT) //����3�̰�
		{
			meg[0]=5;//��ʪ��
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//����ͨ�ò���
			meg[5]=1;//������
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[2]==PRESS_LONG)//����3����
		{
			if (statel[2]==0)
			{
				Load_song3();//һ�׸��ռ�úܴ�Ĵ洢�ռ�
			}
			else if (statel[2]==1)
			{
				Load_song1();
			}
			else if (statel[2]==2)
			{
				Load_song();
			}
			else if (statel[2]==3)
			{
				Load_song2();
			}
			statel[2]++;
			if (statel[2]>4) statel[2]=0;
		}

		if (key[3]==PRESS_SHORT) //����4�̰�
		{
			meg[0]=3;
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//����ͨ�ò���
			meg[5]=2;
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[3]==PRESS_LONG)//����4����
		{
			meg[0]=2;
			send_messeg(CFG_MESSEG,meg);//�л�����ģʽ
		}


		if (key[4]==PRESS_SHORT) //����5�̰�����Ļ����
		{
			
			if (state[4]>0x30)
			{
				state[4]-=0x30;
			}
			else if (state[4]!=0)
			{
				state[4]=0;
			}
			else
			{
				state[4]=0x3f;
			}
			if (state[4]!=0)
			{
				meg[0]=3;meg[1]=1;
				meg[2]=state[4];meg[3]=0;
				meg[4]=1;meg[5]=0;
				light[0]=2;light[1]=0;
				light[2]=LIGHT_LCD_STATE;
			}
			else   //����Ϊ0ʱ����
			{
				meg[0]=3;meg[1]=1;
				meg[2]=state[4];meg[3]=0;
				meg[4]=1;meg[5]=0;
				light[0]=2;light[1]=1;
				light[2]=LIGHT_LCD_STATE;
			}
			send_messeg(LIT_MESSEG,light);//�յ����°���
			send_messeg(LCD_MESSEG,meg);//
		}
		else if (key[4]==PRESS_LONG)//����5����
		{
			if (statel[4]==0)
			{
				meg[0]=3;meg[1]=0;
				meg[2]=1;meg[3]=0;
				meg[4]=1;meg[5]=0;
				light[0]=1;light[1]=0;
				light[2]=LIGHT_LCD_STATE;
				statel[4]=1;
				send_messeg(LIT_MESSEG,light);
				send_messeg(LCD_MESSEG,meg);//

				
			}
			else
			{
				meg[0]=3;meg[1]=0;
				meg[2]=0;meg[3]=0;
				meg[4]=1;meg[5]=0;
				light[0]=1;light[1]=1;
				light[2]=LIGHT_LCD_STATE;
				statel[4]=0;
				send_messeg(LIT_MESSEG,light);//�յ����°���
				send_messeg(LCD_MESSEG,meg);//
			}
			light[0]=2;light[1]=0;
			light[2]=LIGHT_LCD_STATE;
			send_messeg(LIT_MESSEG,light);
		}



		
		if (key[5]==PRESS_SHORT) //����6�̰�
		{
			meg[0]=4;
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//����ͨ�ò���
			meg[5]=2;
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[5]==PRESS_LONG)//����6����
		{
		}
//		delay_ms(100);
	}
}





