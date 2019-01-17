#include "light.h"
#include "delay.h"
#include "misc.h"

#define LED_R PCout(7)
#define LED_B PCout(8)
//#define LIGHT_1 PBout(4)
#define LIGHT_2 PBout(5)
#define LIGHT_3 PBout(6)
#define LIGHT_4 PBout(7)
#define LIGHT_1 PBout(5)

//-------------------------------

#define KEY_LIGHT PAout(1)


#define DELAY_300NS  {__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();}
#define DELAY_900NS  {__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
											__NOP();__NOP();__NOP();__NOP();__NOP();}

											//�����Աߵĵƻ����
static u8 data[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static u8 KEY_COLOR[3][6]={{0},{0},{20,20,20,20,20,20}};//�°漯������������ɫ2018.11.5

static u8 LED_COLOR[3][LED_NUM]={0};

void Light_init (void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	
				//�����ϵĵ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	
		//��ʱʹ���������ŵ���2018.11.23
	GPIO_Init(GPIOB, &GPIO_InitStructure); //�°��Ӳ�ʹ����ǰ�İ����ƹ�2018.11.5
	LED_R=0;
	LED_B=0;
	
	
	/********************PWM����************************/
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
		//��ʱʹ���������ŵ���2018.11.23
	GPIO_Init(GPIOB, &GPIO_InitStructure); //�°��Ӳ�ʹ����ǰ�İ����ƹ�2018.11.5
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
		//��ʱʹ���������ŵ���2018.11.23
	GPIO_Init(GPIOD, &GPIO_InitStructure); //�°��Ӳ�ʹ����ǰ�İ����ƹ�2018.11.5
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
		//��ʱʹ���������ŵ���2018.11.23
	GPIO_Init(GPIOC, &GPIO_InitStructure); //�°��Ӳ�ʹ����ǰ�İ����ƹ�2018.11.5
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
		//��ʱʹ���������ŵ���2018.11.23
	GPIO_Init(GPIOA, &GPIO_InitStructure); //�°��Ӳ�ʹ����ǰ�İ����ƹ�2018.11.5
	
	/********************PWM���Խ���********************/
				//���ܵĵƴ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);


//	LIGHT_4=0;
//	LIGHT_3=0;
//	LIGHT_2=1;
	LIGHT_1=0;

//	HC595_SendData( );�°��Ӳ�ʹ����ǰ�İ����ƹ�2018.11.5
//	LINGT_EN =0;
	led_senddata();
	key_senddata();
}



/************************************************************

		�����İ����ʵƵĿ��ƺ���2018.11.5

			����key_reset()
					key_send()
					key_senddata()
					key_setcolor()
					led_resetall()
					key_around()
*************************************************************/


void key_reset (void)
{
	KEY_LIGHT=0;
	delay_us(800);
//	LIGHT_1=1;
}


void key_send (u8 red ,u8 green,u8 blue)
{
	u8 i=0;
	
	
//	__disable_irq() ;   //�ر����ж�
	for (i=0;i<8;i++)
	{
		if (green&0x80)//����1
		{
			__disable_irq() ;   //�ر����ж�
			KEY_LIGHT=1;
			DELAY_900NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //�ر����ж�
			DELAY_300NS;
		}
		else//����0
		{
			__disable_irq() ;   //�ر����ж�
			KEY_LIGHT=1;
			DELAY_300NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //�ر����ж�
			DELAY_900NS;
		}
		green<<=1;
	}

	
	for (i=0;i<8;i++)
	{
		if (red&0x80)//����1
		{
			__disable_irq() ;   //�ر����ж�
			KEY_LIGHT=1;
			DELAY_900NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //�ر����ж�
			DELAY_300NS;
		}
		else//����0
		{
			__disable_irq() ;   //�ر����ж�
			KEY_LIGHT=1;
			DELAY_300NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //�ر����ж�
			DELAY_900NS;
		}
		red<<=1;
	}
	
	
	
	for (i=0;i<8;i++)
	{
		if (blue&0x80)//����1
		{
			__disable_irq() ;   //�ر����ж�
			KEY_LIGHT=1;
			DELAY_900NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //�ر����ж�
			DELAY_300NS;
		}
		else//����0
		{
			__disable_irq() ;   //�ر����ж�
			KEY_LIGHT=1;
			DELAY_300NS;
			KEY_LIGHT=0;
			__enable_irq() ;   //�ر����ж�
			DELAY_900NS;
		}
		blue<<=1;
	}
//	__enable_irq() ;   //�ر����ж�
	KEY_LIGHT=0;
}

			//ˢ��Ĭ����ɫ
void key_senddata (void)
{
	u16 i=0;
	key_reset();
	for (i=0;i<6;i++)
	{
		key_send (KEY_COLOR[0][i] ,KEY_COLOR[1][i] ,KEY_COLOR[2][i] );
	}
	KEY_LIGHT=0;
}

			//ˢ��ˢ��ָ����������ɫ
void key_senddataby (u8 **colors)
{
	u16 i=0;
	key_reset();
	for (i=0;i<6;i++)
	{
		key_send (colors[0][i] ,colors[1][i] ,colors[2][i] );
	}
	KEY_LIGHT=0;
}



u8 key_setcolor (u16 led_num,u8 red ,u8 green,u8 blue)
{
	if (6<=led_num) return 1;
	KEY_COLOR[0][led_num]=red;
	KEY_COLOR[1][led_num]=green;
	KEY_COLOR[2][led_num]=blue;
	return 0;
}

void key_resetall (void)
{
	u16 i=0;
	for (i=0;i<LED_NUM;i++)
	{
		KEY_COLOR[0][i]=0;
		KEY_COLOR[1][i]=0;
		KEY_COLOR[2][i]=0;
	}
}
			//����ָʾ�ƣ�ָʾ�豸״̬����ɫ
void key_light (u8 key_num,u8 state)
{
	if (key_num>6) return;
	if(state)
	{
		KEY_COLOR[1][key_num-1]=200;
	}
	else
	{
		KEY_COLOR[1][key_num-1]=0;
	}
	key_senddata();
}

			//���������ƣ���ɫ
void key_around (u8 key_num,u8 state)
{
	if (key_num>6) return;
	if(state)
	{
		KEY_COLOR[0][key_num-1]=200;
	}
	else
	{
		KEY_COLOR[0][key_num-1]=0;
	}
	key_senddata();
	
}

/***************************************************
������������ӵļ��̵ƵĿ��ƺ�������Ϊֹ����������
***************************************************/


void sm16703_reset (void)
{
	LIGHT_1=0;
	delay_us(800);
//	LIGHT_1=1;
}



//�ʵ����ݷ��ͣ��Ե�ƽʱ��Ҫ���ϸ�Ҫ�ر������жϣ���������쳣������
//����һ��������ʱ��Լ28.8us
void sm16703_send (u8 red ,u8 green,u8 blue)
{
	u8 i=0;
	
	
//	__disable_irq() ;   //�ر����ж�
	for (i=0;i<8;i++)
	{
		if (green&0x80)//����1
		{
			__disable_irq() ;   //�ر����ж�
			LIGHT_1=1;
			DELAY_900NS;
			LIGHT_1=0;
			__enable_irq() ;   //�ر����ж�
			DELAY_300NS;
		}
		else//����0
		{
			__disable_irq() ;   //�ر����ж�
			LIGHT_1=1;
			DELAY_300NS;
			LIGHT_1=0;
			__enable_irq() ;   //�ر����ж�
			DELAY_900NS;
		}
		green<<=1;
	}

	
	for (i=0;i<8;i++)
	{
		if (red&0x80)//����1
		{
			__disable_irq() ;   //�ر����ж�
			LIGHT_1=1;
			DELAY_900NS;
			LIGHT_1=0;
			__enable_irq() ;   //�ر����ж�
			DELAY_300NS;
		}
		else//����0
		{
			__disable_irq() ;   //�ر����ж�
			LIGHT_1=1;
			DELAY_300NS;
			LIGHT_1=0;
			__enable_irq() ;   //�ر����ж�
			DELAY_900NS;
		}
		red<<=1;
	}
	
	
	
	for (i=0;i<8;i++)
	{
		if (blue&0x80)//����1
		{
			__disable_irq() ;   //�ر����ж�
			LIGHT_1=1;
			DELAY_900NS;
			LIGHT_1=0;
			__enable_irq() ;   //�ر����ж�
			DELAY_300NS;
		}
		else//����0
		{
			__disable_irq() ;   //�ر����ж�
			LIGHT_1=1;
			DELAY_300NS;
			LIGHT_1=0;
			__enable_irq() ;   //�ر����ж�
			DELAY_900NS;
		}
		blue<<=1;
	}
//	__enable_irq() ;   //�ر����ж�
	LIGHT_1=0;
}


//���õƹ����ɫ,0,�ɹ���1��ʧ��
u8 led_setcolor (u16 led_num,u8 red ,u8 green,u8 blue)
{
	if (LED_NUM<=led_num) return 1;
	LED_COLOR[0][led_num]=red;
	LED_COLOR[1][led_num]=green;
	LED_COLOR[2][led_num]=blue;
	return 0;
}

//��ȡ�ƹ����ɫ,0,�ɹ���1��ʧ��
u8 led_getcolor (u16 led_num,u8 *red ,u8 *green,u8 *blue)
{
	if (LED_NUM<=led_num) return 1;
	*red=LED_COLOR[0][led_num];
	*green=LED_COLOR[1][led_num];
	*blue=LED_COLOR[2][led_num];
	return 0;
}

void led_resetall (void)
{
	u16 i=0;
	for (i=0;i<LED_NUM;i++)
	{
		LED_COLOR[0][i]=0;
		LED_COLOR[1][i]=0;
		LED_COLOR[2][i]=0;
	}
}

void led_setall (u8 red,u8 green,u8 blue)
{
	u16 i=0;
	for (i=0;i<LED_NUM;i++)
	{
		LED_COLOR[0][i]=red;
		LED_COLOR[1][i]=green;
		LED_COLOR[2][i]=blue;
	}
}


void led_senddata (void)
{
	u16 i=0;
	sm16703_reset();
	for (i=0;i<LED_NUM;i++)
	{
		sm16703_send (LED_COLOR[0][i] ,LED_COLOR[1][i] ,LED_COLOR[2][i] );
	}
	LIGHT_1=0;
}



//�°��Ӳ�ʹ����ǰ�İ����ƹ�2018.11.5 ��װΪ�պ���
void HC595_SendData(void)
{}

/*
//��������
void HC595_SendData(void)
{
	//���еĵƶ���  1����
//	u8 data=0xff;
	int i,j;
	u8 temp;
	LIGHT_OUT=0;
	for(j=0;j<7;j++)
	{
		if (j==0)
			temp=~data[6-j];//����
		else
			temp=data[6-j];//����
		for(i=0;i<8;i++)
		{
			LIGHT_SCK=0;
			LIGHT_DATA=temp>>(7-i);
			LIGHT_SCK=1;
		}
	}
	LIGHT_OUT=1;

}
*/

//a��ʾ�ĸ�����b��ʾ���ĸ�оƬ
void on_light(u8 a,u8 b)
{


		data[b-1]|=a;//�����
		HC595_SendData();

}

//ѡ�����ô��a�ǰ���״̬b���ĸ�оƬ
void off_light(u8 a,u8 b)
{


		data[b-1]&=(~a);//�����
		HC595_SendData();


}
//����0���أ���0����
u8 GetLight_State(u8 lednum,u8 ic)
{
	u8 ret;
	ret=data[ic-1]&lednum;
	return ret;
}

		//��������λ
void Set_lightdata (u8 *data1)
{
	u16 i=0;
	for (i=0;i<6;i++)
	{
		data[i]=data1[i];
	}
}

		//hoqu����λ
void Get_lightdata (u8 *data1)
{
	u16 i=0;
	for (i=0;i<6;i++)
	{
		data1[i]=data[i];
	}
}



void Run_Light (void)
{
	static u8 t=0;
	static u16 len=0;
	static u8 red;
	static u8 green;
	static u8 blue ;
	static u8 led_t=0;
	u8 temp=0xff;
	u8 e=data[6];
	u8 i=0;
	for (i=0;i<7;i++)
	{
		data[i]=0;
	}
	
	if (t>=8) data[(t-8)/8]=(temp<<(7-t%8)); 
		data[t/8]=(temp>>(t%8));
	data[t/8+1]=temp<<(7-t%8);
	data[t/8+2]=temp>>(t%8);
	data[t/8+3]=temp<<(7-t%8);
	data[t/8+4]=temp>>(t%8);
	data[t/8+5]=temp<<(7-t%8);
	data[6]=e;
	
	t++;
	if (t>15) t=0;
	HC595_SendData();
	
	
		led_resetall ();
		led_setcolor (len-1,red*1/10,green*2/10,blue*2/10);
		led_setcolor (len-2,red*2/10,green*5/10,blue*5/10);
		led_setcolor (len-3,red*10/10,green*10/10,blue*10/10);
		led_setcolor (len-4,red*2/10,green*5/10,blue*5/10);
		led_setcolor (len-5,red*1/10,green*2/10,blue*2/10);
		led_senddata();
		len++;
		if (len>30)
		{
			len%=30;
			led_t++;
			if (led_t>=10) led_t=0;
		}
		switch (led_t)
		{
			case 0:
				red=255;
				green =255;
				blue =255;
				break;
			case 1:
				red=255;
				green =255;
				blue =0;
				break;
			case 2:
				red=0;
				green =255;
				blue =255;
				break;
			case 3:
				red=255;
				green =0;
				blue =255;
				break;
			case 4:
				red=255;
				green =0;
				blue =0;
				break;
			case 5:
				red=0;
				green =255;
				blue =0;
				break;
			case 6:
				red=0;
				green =0;
				blue =255;
				break;
			case 7:
				red=128;
				green =30;
				blue =255;
				break;
			case 8:
				red=255;
				green =30;
				blue =128;
				break;
			case 9:
				red=128;
				green =255;
				blue =30;
				break;
			default:
				break;
		}
	
}




#define LED111 PBout(0)
#define LED222 PBout(1)




#define LED_TEST1 PAout(15)
#define LED_TEST2 PBout(3)
#define LED_TEST3 PDout(2)
#define LED_TEST4 PCout(10)




void LIGHT_RUN_IRQ(void)
{
	static u32 a=0;
	static u32 b=0;
	static u32 c=0;
	static u32 d=0;
	a++;
	if (a==100)//Ƶ�ʿ���
	{
		a=0;
//		LED111=1;
		LED222=1;
		LED_TEST1=1;
		LED_TEST2=1;
		LED_TEST3=1;
		LED_TEST4=1;
	}
	else if (a==20)
	{
		LED111=0;
		LED222=0;
		LED_TEST1=0;
		LED_TEST2=0;
		LED_TEST3=0;
		LED_TEST4=0;
	}
	
	d++;
	static u8 t=0;
	if (d>=1000)//�������
	{
		d=0;
		if (t==0)
		{
			b++;
			if (b>=40)
			{
				t=1;
			}
		}
		if (t==1)
		{
			if (b>0)
				b--;
			else
			{
				
				t=0;
			}
		}
	}
}








