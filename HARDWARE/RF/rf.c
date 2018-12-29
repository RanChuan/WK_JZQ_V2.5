#include "rf.h"
#include "stdio.h"
#include "delay.h"
#include "stmflash.h"
#include "my_rf.h"
#include "includes.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   








						//设备数量，按照数量，类型的格式
						//这个数组通过初始化函数填充
u16 DEVICE_NUM[DEVICE_DATA_NUM]={1,1,1,3,1,2,1,4,1,5};

 

					//采集器的数据
u8 EN_DATA[25]={0};



		//无线的输入焦点
u8 RF_FOCUS=TASK_MAX_NUM;





#define AUX PAin(8)

#define USART1_RX_ENABLE  *((volatile unsigned long  *)(0x42000000 + (((u32)(&USART1->CR1))*32) + (5*4)))=1
#define USART1_RX_DISABLE  *((volatile unsigned long  *)(0x42000000 + (((u32)(&USART1->CR1))*32) + (5*4)))=0


u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//u8 USART_RX_BUF2[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

static u16 RX_START=0;		//记录读取结束的位置，实现数据队列
static u16 RET_START=0;	//接收结束的位置



//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13，	接收出错，缓冲池已满
u16 USART_RX_STA=0;       //接收状态标记
static u16 RET_NUM=0;//，	接收到的有效字节数目


static u8 USART1_IDE=1;//空闲



#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
int _sys_exit(int x) 
{ 
	x = x; 
	return 0;
} 

void _ttywrch(int ch)
{
}


//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0){}//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 





				//复制数据
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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PB.5 输出高
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);						 //PB.5 输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);						 //PB.5 输出高
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9

	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Cmd( USART1, DISABLE );		
	USART_DeInit(USART1);	
	USART_Init(USART1, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启接收中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//开启空闲中断
	//		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//开启发送中断
	USART_Cmd(USART1, ENABLE);                    //使能串口 
}



u16 USART1_GetFlag (void)
{
			return RET_NUM;

}


//获取Buff的第一个数据
//自动在Buff中循环
//调用之后会清除当前位并自动加一
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
	if (RET_NUM)RET_NUM--;//每读取一次就减少一个接收数
	USART1_RX_ENABLE;
	RX_START++;
	
	return temp;
}

//查看Buff当前位的数据
//不会清除当前位，不会自动加一
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



				//设置无线配置的时候用
u16 RF_Get (u8 *buff,u16 len)
{
	u16 getlen=0;//记录已经读取到的字节个数
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
	u16 getlen=0;//记录已经读取到的字节个数
	u16 i;//临时循环变量
//	A:
	for (getlen=0;getlen<len;getlen++)
			buff[getlen]=0;

	getlen=0;
	u16 iii=10000;
	while(USART1_IDE==0)//等串口把数据接收完
	{
		iii--;
		if (iii==0) return 0;
	}
	if (AUX)
	{
		getlen=0;
		for (i=0;i<USART_REC_LEN;i++)//查询整个缓冲区，有没有命令
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
								//这段把接收到的作用于本机的命令保存在专用buff里面
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
			RX_START=0;							//如果全部读取了，重新开始接收
			USART1_RX_DISABLE;
			RET_START=0;
			USART1_RX_ENABLE;
		}
	}
		return getlen;
	
}





	//设置信道
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
		RF1_tx_bytes(setdata,6);//发送配置
		delay_ms(500);
		u8 recv[6]={0};
		RF_Get(recv,6);
		if (samestr("OK",recv)) break;
	}
	RF_M0=0;
	RF_M1=0;
	delay_ms(100);
	RF_Init(115200);
	return 0;//设置成功
}







void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	static u8 t=0;
	u8 temp=0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		
		USART1_IDE=0;
		
		temp= (uint8_t)USART1->DR;//执行读操作自动清零中断标记
		if (t<58) MY_CMD[t++]=temp;
		if (RET_NUM<USART_REC_LEN)//如果缓冲池没满才接收
		{
			USART_RX_BUF[RET_START]=temp;
			RET_NUM++;
			RET_START++;
			if (RET_START>=USART_REC_LEN) RET_START=0;
		}else		//缓冲池不为空，标记为错误
		{
			USART_ClearFlag(USART1, USART_IT_RXNE);//没有执行读操作，需要程序中清除标志位
		}
	}
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //空闲中断
	{
		USART1_IDE=1;
		t=USART1->SR;
		t=USART1->DR;
		t=0;
		TaskIntSendMsg(RF_FOCUS,1);//发送给无线焦点进程
	}
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)  //发送中断
	{
		USART_ClearFlag(USART1, USART_IT_TXE);
		
	}
	
} 


u16 RF1_tx_bytes( uint8_t* TxBuffer, uint8_t Length )
{
	
	if (RF_FOCUS!=OSPrioHighRdy) return 0;//不是焦点进程调用无效
	if (!Length) return 0;
	while( Length-- )
	{
		while( RESET == USART_GetFlagStatus( USART1, USART_FLAG_TXE ));
		USART1->DR = *TxBuffer;
		TxBuffer++;
	}
	return 0;
}


//////////供外部调用的函数/////////////////////////////


					//获取本机地址
u16 Get_MyAddr(void)
{
	return EN_CONFIG[0];
}


void Set_MyAddr(u16 addr)
{
	EN_CONFIG[0]=addr;
}



//------------------------供外部调用的函数---------------------------


					//获取采集器数据首地址
u8 *GetCJDataAddr (void)
{
	return EN_DATA;
}




				//设置设备参数,
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

					//设置ip地址
void Set_MyIP (u8 *data)
{
	u16 i=0;
	for (i=0;i<16;i++)
	{
		EN_CONFIG[CONFIG_DATA_NUM-20+i]=data[i];
	}
} 

					//获取ip地址
u16 * Get_MyIP (void)
{
		return &EN_CONFIG[CONFIG_DATA_NUM-20];
}







 
