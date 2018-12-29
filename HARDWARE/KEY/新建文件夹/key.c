#include "key.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"




//1，用外部中断，0使用定时器扫描
#define USE_EXIT 0

//按键引脚定义
#define KEY1  PCin(7)
#define KEY2  PAin(0)
#define KEY3  PCin(8)
#define KEY4  PBin(9)
#define KEY5  PCin(9)
#define KEY6  PBin(8)


//按键数组，支持多个按键同时按下
static u8 KEYS[6];

void Key_Init (void)
{
#if USE_EXIT==1
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
#endif

	GPIO_InitTypeDef GPIO_InitStructure;
	//初始化KEY0-->GPIOA.13,KEY1-->GPIOA.15  上拉输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTE时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;//KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE2,3,4

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;//KEY3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;//KEY4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOE2,3,4

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;//KEY5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE2,3,4

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;//KEY6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOE2,3,4

RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
#if USE_EXIT==1	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);//KEY1
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);//KEY2
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);//KEY3
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);//KEY4
	EXTI_InitStructure.EXTI_Line=EXTI_Line9;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource9);//KEY5
	EXTI_InitStructure.EXTI_Line=EXTI_Line9;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);//KEY6
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
#else
	TIME4_Init( );

#endif
}

void TIME4_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 100-1; //10ms
	TIM_TimeBaseStructure.TIM_Prescaler =7200-1; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx		
}


//返回按键状态
u8 Get_Key(u8 keynum)
{
	u8 key_ret;
	key_ret=KEYS[keynum-1];
	KEYS[keynum-1]=PRESS_NONE;
	return key_ret;
}





//中断服务函数														
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


//按键中断服务程序
void KEY_IRQHandler(void)   //
{
	static u8 key_time[6];
	static u8 key_valid[6];
//KEY1																																				
		if (KEY1==0)
		{
			if (key_valid[0]==0)//按键有效
			{
				key_time[0]++;
				if (key_time[0]>=80)
				{
					KEYS[0]=PRESS_LONG;
					key_valid[0]=1;//触发长按，按键无效
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
			key_valid[0]=0;//按键设为有效
		}
//KEY2																																				
		if (KEY2==0)
		{
			if (key_valid[1]==0)//按键有效
			{
				key_time[1]++;
				if (key_time[1]>=80)
				{
					KEYS[1]=PRESS_LONG;
					key_valid[1]=1;//触发长按，按键无效
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
			if (key_valid[2]==0)//按键有效
			{
				key_time[2]++;
				if (key_time[2]>=80)
				{
					KEYS[2]=PRESS_LONG;
					key_valid[2]=1;//触发长按，按键无效
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
			if (key_valid[3]==0)//按键有效
			{
				key_time[3]++;
				if (key_time[3]>=80)
				{
					KEYS[3]=PRESS_LONG;
					key_valid[3]=1;//触发长按，按键无效
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
			if (key_valid[4]==0)//按键有效
			{
				key_time[4]++;
				if (key_time[4]>=80)
				{
					KEYS[4]=PRESS_LONG;
					key_valid[4]=1;//触发长按，按键无效
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
			if (key_valid[5]==0)//按键有效
			{
				key_time[5]++;
				if (key_time[5]>=80)
				{
					KEYS[5]=PRESS_LONG;
					key_valid[5]=1;//触发长按，按键无效
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
			key_valid[5]=0;//按键设为有效
		}

}

void TIM4_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志 
		KEY_IRQHandler( );   //ojjm中断

	}
}
#endif 




