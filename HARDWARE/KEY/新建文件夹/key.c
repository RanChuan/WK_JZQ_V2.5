#include "key.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"




//1�����ⲿ�жϣ�0ʹ�ö�ʱ��ɨ��
#define USE_EXIT 0

//�������Ŷ���
#define KEY1  PCin(7)
#define KEY2  PAin(0)
#define KEY3  PCin(8)
#define KEY4  PBin(9)
#define KEY5  PCin(9)
#define KEY6  PBin(8)


//�������飬֧�ֶ������ͬʱ����
static u8 KEYS[6];

void Key_Init (void)
{
#if USE_EXIT==1
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
#endif

	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʼ��KEY0-->GPIOA.13,KEY1-->GPIOA.15  ��������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTEʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTEʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;//KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;//KEY3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;//KEY4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;//KEY5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;//KEY6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
#if USE_EXIT==1	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);//KEY1
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);//KEY2
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);//KEY3
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);//KEY4
	EXTI_InitStructure.EXTI_Line=EXTI_Line9;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource9);//KEY5
	EXTI_InitStructure.EXTI_Line=EXTI_Line9;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);//KEY6
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 
#else
	TIME4_Init( );

#endif
}

void TIME4_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 100-1; //10ms
	TIM_TimeBaseStructure.TIM_Prescaler =7200-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx		
}


//���ذ���״̬
u8 Get_Key(u8 keynum)
{
	u8 key_ret;
	key_ret=KEYS[keynum-1];
	KEYS[keynum-1]=PRESS_NONE;
	return key_ret;
}





//�жϷ�����														
#if USE_EXIT==1

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line7)!=RESET)//KEY1
	{
		KEYS[0]=PRESS_SHORT;
		EXTI_ClearFlag(EXTI_Line7);
		
	}

	if(EXTI_GetITStatus(EXTI_Line8)!=RESET)//KEY3,KEY6
	{
		KEYS[2]=PRESS_SHORT;
		KEYS[5]=PRESS_SHORT;
		EXTI_ClearFlag(EXTI_Line8);
		
	}
	if(EXTI_GetITStatus(EXTI_Line9)!=RESET)//KEY4,KEY5
	{
		KEYS[3]=PRESS_SHORT;
		KEYS[4]=PRESS_SHORT;
		EXTI_ClearFlag(EXTI_Line9);
		
	}
	
}
 
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)//KEY2
	{
		KEYS[0]=PRESS_SHORT;
		EXTI_ClearFlag(EXTI_Line0);
		
	}
}
#else


//�����жϷ������
void KEY_IRQHandler(void)   //
{
	static u8 key_time[6];
	static u8 key_valid[6];
//KEY1																																				
		if (KEY1==0)
		{
			if (key_valid[0]==0)//������Ч
			{
				key_time[0]++;
				if (key_time[0]>=80)
				{
					KEYS[0]=PRESS_LONG;
					key_valid[0]=1;//����������������Ч
				}
			}
		}
		else
		{
		if (key_time[0]>1&&key_time[0]<80)
			{
				KEYS[0]=PRESS_SHORT;
			}
			key_time[0]=0;
			key_valid[0]=0;//������Ϊ��Ч
		}
//KEY2																																				
		if (KEY2==0)
		{
			if (key_valid[1]==0)//������Ч
			{
				key_time[1]++;
				if (key_time[1]>=80)
				{
					KEYS[1]=PRESS_LONG;
					key_valid[1]=1;//����������������Ч
				}
			}
		}
		else
		{
		if (key_time[1]>1&&key_time[1]<80)
			{
				KEYS[1]=PRESS_SHORT;
			}
			key_time[1]=0;
			key_valid[1]=0;
		}
//KEY3																																				
		if (KEY3==0)
		{
			if (key_valid[2]==0)//������Ч
			{
				key_time[2]++;
				if (key_time[2]>=80)
				{
					KEYS[2]=PRESS_LONG;
					key_valid[2]=1;//����������������Ч
				}
			}
		}
		else
		{
		if (key_time[2]>1&&key_time[2]<80)
			{
				KEYS[2]=PRESS_SHORT;
			}
			key_time[2]=0;
			key_valid[2]=0;
		}
//KEY4																																				
		if (KEY4==0)
		{
			if (key_valid[3]==0)//������Ч
			{
				key_time[3]++;
				if (key_time[3]>=80)
				{
					KEYS[3]=PRESS_LONG;
					key_valid[3]=1;//����������������Ч
				}
			}
		}
		else
		{
			if (key_time[3]>1&&key_time[3]<80)
			{
				KEYS[3]=PRESS_SHORT;
			}
			key_time[3]=0;
			key_valid[3]=0;
		}
//KEY5																																				
		if (KEY5==0)
		{
			if (key_valid[4]==0)//������Ч
			{
				key_time[4]++;
				if (key_time[4]>=80)
				{
					KEYS[4]=PRESS_LONG;
					key_valid[4]=1;//����������������Ч
				}
			}
		}
		else
		{
			if (key_time[4]>1&&key_time[4]<80)
			{
				KEYS[4]=PRESS_SHORT;
			}
			key_time[4]=0;
			key_valid[4]=0;
		}
//KEY6																																				
		if (KEY6==0)
		{
			if (key_valid[5]==0)//������Ч
			{
				key_time[5]++;
				if (key_time[5]>=80)
				{
					KEYS[5]=PRESS_LONG;
					key_valid[5]=1;//����������������Ч
				}
			}
		}
		else
		{
			if (key_time[5]>1&&key_time[5]<80)
			{
				KEYS[5]=PRESS_SHORT;
			}
			key_time[5]=0;
			key_valid[5]=0;//������Ϊ��Ч
		}

}

void TIM4_IRQHandler(void)   //TIM4�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		KEY_IRQHandler( );   //ojjm�ж�

	}
}
#endif 




