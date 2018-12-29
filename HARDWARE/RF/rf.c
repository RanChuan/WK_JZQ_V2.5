#include "rf.h"
#include "stdio.h"
#include "delay.h"
#include "stmflash.h"
#include "my_rf.h"
#include "includes.h"

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








						//�豸�������������������͵ĸ�ʽ
						//�������ͨ����ʼ���������
u16 DEVICE_NUM[DEVICE_DATA_NUM]={1,1,1,3,1,2,1,4,1,5};

 

					//�ɼ���������
u8 EN_DATA[25]={0};



		//���ߵ����뽹��
u8 RF_FOCUS=TASK_MAX_NUM;





#define AUX PAin(8)

#define USART1_RX_ENABLE  *((volatile unsigned long  *)(0x42000000 + (((u32)(&USART1->CR1))*32) + (5*4)))=1
#define USART1_RX_DISABLE  *((volatile unsigned long  *)(0x42000000 + (((u32)(&USART1->CR1))*32) + (5*4)))=0


u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//u8 USART_RX_BUF2[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

static u16 RX_START=0;		//��¼��ȡ������λ�ã�ʵ�����ݶ���
static u16 RET_START=0;	//���ս�����λ��



//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13��	���ճ������������
u16 USART_RX_STA=0;       //����״̬���
static u16 RET_NUM=0;//��	���յ�����Ч�ֽ���Ŀ


static u8 USART1_IDE=1;//����



#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
int _sys_exit(int x) 
{ 
	x = x; 
	return 0;
} 

void _ttywrch(int ch)
{
}


//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0){}//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 





				//��������
void copy_data (u8 *from,u8 *to,u16 length)
{
	while(length--)
	{
		*to++=*from++;
	}
}


void RF_SetFocus(u8 focus)
{
	if (focus<TASK_MAX_NUM)
		RF_FOCUS=focus;
}




void RF_Init(u32 bound)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART1);  //��λ����1

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PB.5 �����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);						 //PB.5 �����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);						 //PB.5 �����
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9

	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Cmd( USART1, DISABLE );		
	USART_DeInit(USART1);	
	USART_Init(USART1, &USART_InitStructure); //��ʼ������
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���������ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//���������ж�
	//		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//���������ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
}



u16 USART1_GetFlag (void)
{
			return RET_NUM;

}


//��ȡBuff�ĵ�һ������
//�Զ���Buff��ѭ��
//����֮��������ǰλ���Զ���һ
static u8 Get_BuffData(void)
{
	u8 temp;
	if (RX_START<USART_REC_LEN)
	{
		temp=USART_RX_BUF[RX_START];
	}
	else
	{
		RX_START=0;
		temp=USART_RX_BUF[RX_START];
	}

	USART1_RX_DISABLE;
	USART_RX_BUF[RX_START]=0;
	if (RET_NUM)RET_NUM--;//ÿ��ȡһ�ξͼ���һ��������
	USART1_RX_ENABLE;
	RX_START++;
	
	return temp;
}

//�鿴Buff��ǰλ������
//���������ǰλ�������Զ���һ
static u8 See_BuffData(void)
{
	u8 temp;
	if (RX_START<USART_REC_LEN)
	{
		temp=USART_RX_BUF[RX_START];
	}
	else
	{
		RX_START=0;
		temp=USART_RX_BUF[RX_START];
	}
	return temp;
	
}
static u8 See_BuffNextData(void)
{
	u8 temp;
	u8 i=RX_START+1;
	if (i<USART_REC_LEN)
	{
		temp=USART_RX_BUF[i];
	}
	else
	{
		i=0;
		temp=USART_RX_BUF[i];
	}
	return temp;
	
}



				//�����������õ�ʱ����
u16 RF_Get (u8 *buff,u16 len)
{
	u16 getlen=0;//��¼�Ѿ���ȡ�����ֽڸ���
	if (RET_NUM)
	{
		for (getlen=0;getlen<len;getlen++)
			buff[getlen]=0;
	}
	
	getlen=0;
	while (RET_NUM)
	{
		buff[getlen]=Get_BuffData();
		getlen++;
	}
	return getlen;
}



u8 MY_CMD[58]={0};
u16  Get_Mycmd(u8 *data) 
{
	u16 i=0;
	for (i=0;i<58;i++)
	{
		data[i]=MY_CMD[i];
	}
	Clear_Mycmd();
	return i;
}

void Clear_Mycmd(void)
{
	u16 i=0;
	for (i=0;i<58;i++)
	{
		MY_CMD[i]=0;
	}
}






u16 RF_GetCmd (uint8_t *buff,u16 len)
{
	u16 getlen=0;//��¼�Ѿ���ȡ�����ֽڸ���
	u16 i;//��ʱѭ������
//	A:
	for (getlen=0;getlen<len;getlen++)
			buff[getlen]=0;

	getlen=0;
	u16 iii=10000;
	while(USART1_IDE==0)//�ȴ��ڰ����ݽ�����
	{
		iii--;
		if (iii==0) return 0;
	}
	if (AUX)
	{
		getlen=0;
		for (i=0;i<USART_REC_LEN;i++)//��ѯ��������������û������
		{
			if (See_BuffData()==0xff&&See_BuffNextData()==0xff)
				break;
			else
			{
				Get_BuffData();
			}
			
		}
		getlen=0;
		while(RET_NUM)
		{
			buff[getlen] =Get_BuffData();
			getlen++;
			if (getlen==len)
				break;
			if (See_BuffData()==0xff&&See_BuffNextData()==0xff)
				break;
		}
								//��ΰѽ��յ��������ڱ������������ר��buff����
//		if ((((buff[2]<<8)|buff[3])==0x0000)||(((buff[2]<<8)|buff[3])==Get_MyAddr()))
//		{
//			if (getlen)
//			{
//				Clear_Mycmd();
//				for (i=0;i<getlen;i++)
//				{
//					MY_CMD[i]=buff[i];
//				}
//				goto A;
//			}
//		}
		
		
		if (RET_NUM==0) 
		{
			RX_START=0;							//���ȫ����ȡ�ˣ����¿�ʼ����
			USART1_RX_DISABLE;
			RET_START=0;
			USART1_RX_ENABLE;
		}
	}
		return getlen;
	
}





	//�����ŵ�
u8 RF_SetChannel(u8 channel)
{
	RF_Init(9600);
	RF_M0=1;
	RF_M1=1;
	delay_ms(100);
	u8 setdata[]={0xc2,0x00,0x00,0x3a,0x14,0x40};
	setdata[4]=channel;
	while(1)
	{
		RF1_tx_bytes(setdata,6);//��������
		delay_ms(500);
		u8 recv[6]={0};
		RF_Get(recv,6);
		if (samestr("OK",recv)) break;
	}
	RF_M0=0;
	RF_M1=0;
	delay_ms(100);
	RF_Init(115200);
	return 0;//���óɹ�
}







void USART1_IRQHandler(void)                	//����1�жϷ������
{
	static u8 t=0;
	u8 temp=0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		
		USART1_IDE=0;
		
		temp= (uint8_t)USART1->DR;//ִ�ж������Զ������жϱ��
		if (t<58) MY_CMD[t++]=temp;
		if (RET_NUM<USART_REC_LEN)//��������û���Ž���
		{
			USART_RX_BUF[RET_START]=temp;
			RET_NUM++;
			RET_START++;
			if (RET_START>=USART_REC_LEN) RET_START=0;
		}else		//����ز�Ϊ�գ����Ϊ����
		{
			USART_ClearFlag(USART1, USART_IT_RXNE);//û��ִ�ж���������Ҫ�����������־λ
		}
	}
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //�����ж�
	{
		USART1_IDE=1;
		t=USART1->SR;
		t=USART1->DR;
		t=0;
		TaskIntSendMsg(RF_FOCUS,1);//���͸����߽������
	}
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)  //�����ж�
	{
		USART_ClearFlag(USART1, USART_IT_TXE);
		
	}
	
} 


u16 RF1_tx_bytes( uint8_t* TxBuffer, uint8_t Length )
{
	
	if (RF_FOCUS!=OSPrioHighRdy) return 0;//���ǽ�����̵�����Ч
	if (!Length) return 0;
	while( Length-- )
	{
		while( RESET == USART_GetFlagStatus( USART1, USART_FLAG_TXE ));
		USART1->DR = *TxBuffer;
		TxBuffer++;
	}
	return 0;
}


//////////���ⲿ���õĺ���/////////////////////////////


					//��ȡ������ַ
u16 Get_MyAddr(void)
{
	return EN_CONFIG[0];
}


void Set_MyAddr(u16 addr)
{
	EN_CONFIG[0]=addr;
}



//------------------------���ⲿ���õĺ���---------------------------


					//��ȡ�ɼ��������׵�ַ
u8 *GetCJDataAddr (void)
{
	return EN_DATA;
}




				//�����豸����,
void Set_Device(u8 * data,u16 len)
{
	u16 i=0;
	for (i=1;i<100;i++) {EN_CONFIG[i]=0;}
	for (i=0;i<len;i++)
	{
		EN_CONFIG[2+i*2]=(data[i*3]<<8)|data[i*3+1];
		EN_CONFIG[3+i*2]=data[i*3+2];
	}
}

					//����ip��ַ
void Set_MyIP (u8 *data)
{
	u16 i=0;
	for (i=0;i<16;i++)
	{
		EN_CONFIG[CONFIG_DATA_NUM-20+i]=data[i];
	}
} 

					//��ȡip��ַ
u16 * Get_MyIP (void)
{
		return &EN_CONFIG[CONFIG_DATA_NUM-20];
}







 
