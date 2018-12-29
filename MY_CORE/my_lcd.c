#include "my_lcd.h"
#include "includes.h"
#include "stmflash.h"
#include "delay.h"
#include "relay.h"
#include "my_key.h"


#include "includes.h"//调用进程挂起函数




							//进入一个页面，默认主页面
uint8_t LCD_PAGE[6]				={0x5A,0xA5,0x03,0x80,0x4F,0x01};//主页面

							//改变按钮开关状态
uint8_t LCD_STATE[8]				={0x5A,0xA5,0x05,0x82,0x10,0x01,0x00,0x00};//升温按钮  00关  01开
#define  WENDU_UP  0x01
#define  WENDU_DOWN 0x02
#define  SHIDU_UP  0x03
#define  SHIDU_DOWN 0x04
#define  JINHUA  0x05
#define  HAND  0x06
			
						//设置屏幕上的数据
uint8_t LCD_SET_DATA[8]      ={0x5A ,0xA5 ,0x05 ,0x82 ,0x00 ,0x14 ,0x00 ,0x00};
#define  WENDU_UP_ADDR  0x14
#define  WENDU_DOWN_ADDR  0x15
#define  SHIDU_UP_ADDR  0x16
#define  SHIDU_DOWN_ADDR  0x17
#define  PM2_5_UP_ADDR  0x18
#define  LCD_POWER_HS_ADDR  0x19
#define  LCD_POWER_SS_ADDR  0x1a
#define  LCD_POWER_HE_ADDR  0x1b
#define  LCD_POWER_SE_ADDR  0x1c


						//获取屏幕上的数据
uint8_t LCD_GET_DATA[7]				={0x5A,0xA5,0x04,0x83,0x00,0x00,0x01};//通过发送获取数据


						//按键和按键灯的
#define  WENDU_UP_  0x01
#define  WENDU_DOWN_ 0x02
#define  SHIDU_UP_  0x03
#define  SHIDU_DOWN_ 0x04
#define  LCD_STATE_ 0x05
#define  JINHUA_  0x06


/*****************************************

		meg的位定义
		meg[0],1,显示报警信息，2，设备状态指示3，屏幕电源控制，4，显示温湿度5,发出比比声
		meg[1],开或关，控制屏幕电源时1，开，可控制屏幕亮度，0，电源控制,发出哔哔声时1，短，2，长
		meg2，亮或灭第几个,控制电源开的时候屏幕亮度，电源时开或关
		meg3, 温控高8位
		meg4，温度低8位
		meg5，湿度高8位
		meg6，湿度低8位
		meg7，tvoc高8位
		meg8，tvoc低8位
		
		详细的消息定义参见my_messeg.h文件

***************************************/







			//屏幕任务函数
void my_lcd (void * t)
{
	u8 buf[30]={0};u16 reclen;
	u16 i=0;
	u8 meg[MESSEG_DATA]={0};
	Relay_Init();
	LCD_Init(9600);
	my_lcd_init();
	LCD_SetFocus(OSPrioHighRdy); 
	while (1)
	{
		delay_ms(30);
		reclen=0;
		if (get_messeg (LCD_MESSEG,meg)==0)//如果接收到了消息
		{
			if (meg[0]==2)//设备状态
			{
				lcd_messeg_02 (meg);
			}
			else if (meg[0]==3)//pingmu power
			{
				lcd_messeg_03 (meg);
			}
			else if (meg[0]==4)
			{
				lcd_dataUp ( meg);
			}
			else if (meg[0]==5)
			{
				lcd_beep(meg[1]);
			}
		}
		lcd_timetosleep();
		LCD_Receive_Data(buf,&reclen);

		for(i=0;i<reclen;i++)//判断数据内容
		{
			if(buf[i]==0x5A)
			{
				switch(buf[i+2])
				{
					case 0x06:
						switch (buf[i+8])
						{
								case 0xA1:Set_Init();break;//进入设置页面
								case 0xA2:Hand_Set_Init();break;//进入手动页面
								case 0xA3:		lcd_sleep( );break;//睡眠
	//						case 0xA4:break;//关机
								case 0xA5:
									TaskPend(7);//挂起网口任务
									Set_Save();
									TaskRepend(7);
									break;//设置确定
	//						case 0xA6:FLAG_MAIN=1;break;//设置返回
	//						case 0xA7:Button_Judge_ShowText();break;//设置勾选
								case 0xA8:Auto_sleep();break;//设置定时开关
	//						case 0xA9:break;//进入更多设置
	//						case 0xAA:Senior_Set_Save();break;//高级设置确定
	//						case 0xAB:break;//高级设置返回
								case 0xAC:Save_LCD();break;//手动设置确定
	//						case 0xAD:AUTO=1;break;//手动设置返回
								case 0xB1:BUT_UP_TEMP();break;//手动升温
								case 0xB2:BUT_DOWN_TEMP();break;//降温
								case 0xB3:BUT_UP_HUMI();break;//加湿
								case 0xB4:BUT_DOWN_HUMI();break;//降湿
								case 0xB5:BUT_PM();break;//空气净化
								case 0xB6:
									TaskPend(7);//挂起网口任务
									Time_Set();
									TaskRepend(7);
									break;//时间确定按钮
								case 0xB8:
									AutoCtr();break;
							default:break;
						}break;
						case 0x04:if(buf[i+6]==0x03)lcd_sleep( );break;
						case 0x08:
							Into_Senior(buf);
							break;
						default:break;
				}
			}
			

		}
	}

}

			//进入高级设置
void Into_Senior(u8 *buf) 
{
	u8 data[]={0x5A ,0xA5 ,0x03 ,0x80 ,0x4F ,0x02};
	if ((buf[7]==00)&&(buf[8]==0x05)&&(buf[9]==0x0d)&&(buf[10]==0xc0))//331200
	{
		LCD_Send_Data(data,6);
	}
}



				//手动自动转换
void AutoCtr(void)
{
	if (HAND_STATE==1)
	{
		HAND_STATE=0;
	}
	else
	{
		HAND_STATE=1;
	}
	lcd_chan_state (HAND,HAND_STATE);
}











						//屏幕睡眠按键
void lcd_sleep(void)
{
	u8 meg[MESSEG_DATA]={0};
	u8 light[MESSEG_DATA]={0};
	static u8 lcd_light=0x3f;//屏幕的亮度
	if (get_keystate(LCD_STATE_))
	{
		meg[0]=3;meg[1]=1;
		meg[2]=0;meg[3]=0;
		meg[4]=1;meg[5]=0;
		light[0]=1;light[1]=LCD_STATE_;
		light[2]=0;//亮度改为0
		send_messeg(KEY_MESSEG,light);
		light[0]=2;light[1]=1;
		light[2]=LCD_STATE_;
		send_messeg(LIT_MESSEG,light);
		send_messeg(LCD_MESSEG,meg);//
		lcd_light=get_keystate(LCD_STATE_);
	}
	else
	{
		meg[0]=3;meg[1]=1;
		meg[2]=lcd_light;meg[3]=0;
		meg[4]=1;meg[5]=0;
		light[0]=1;light[1]=LCD_STATE_;
		light[2]=lcd_light;
		send_messeg(KEY_MESSEG,light);
		light[0]=2;light[1]=0;
		light[2]=LCD_STATE_;
		send_messeg(LIT_MESSEG,light);
		send_messeg(LCD_MESSEG,meg);//
	}
	
}




				//屏幕上传温湿度数据
void lcd_dataUp (u8* meg)
{
	u8 data[8]={0x5A, 0xA5 ,0x05 ,0x82 ,0x00 ,0x11 ,0x00 ,0x00};
	data[6]=meg[3];data[7]=meg[4];//原数据*10
	LCD_Send_Data(data,8);//发送温度
	delay_us(100);
	data[5]=0x12;data[6]=meg[5];data[7]=meg[6];//原数据
	LCD_Send_Data(data,8);//发送湿度
	delay_us(100);
	data[5]=0x13;data[6]=meg[7];data[7]=meg[8];//原数据*10
	LCD_Send_Data(data,8);//发送tvoc
	
}







			//发出比比声
void lcd_beep(u8 l)
{
	u8 dd[]={0x5a,0xa5,0x03,0x80,0x02,0x0a};
	if (l==1)
	{
		dd[5]=0x05;
	}
	else if (l==2)
	{
		dd[5]=0x0a;
	}
		LCD_Send_Data(dd,6);

}






void Time_Set()//休眠时间设置保存
{
	u8 R_buff[20];u16 R_len;
	u8 gettime[7]={0x5A,0xA5,0x04,0x83,0x00,0x00,0x01};//获取时间

	u16 time_year; 
	u16 time_month; 
	u16 time_day; 
	u16 time_hour; 
	u16 time_min; 
	u16 time_sec; 
	
	
			gettime[5]=0x24;
			LCD_Send_Data(gettime,7);delay_ms(20);
			LCD_Receive_Data(R_buff,&R_len);
			if(R_buff[5]==0x24&&R_len==9)
			{
				time_year=(R_buff[7]<<8)|R_buff[8];
			}
		
			gettime[5]=0x25;
			LCD_Send_Data(gettime,7);delay_ms(20);
			LCD_Receive_Data(R_buff,&R_len);
			if(R_buff[5]==0x25&&R_len==9)
			{
				time_month=(R_buff[7]<<8)|R_buff[8];
			}


			gettime[5]=0x26;
			LCD_Send_Data(gettime,7);delay_ms(20);
			LCD_Receive_Data(R_buff,&R_len);
			if(R_buff[5]==0x26&&R_len==9)
			{
				time_day=(R_buff[7]<<8)|R_buff[8];
			}


			gettime[5]=0x27;
			LCD_Send_Data(gettime,7);delay_ms(20);
			LCD_Receive_Data(R_buff,&R_len);
			if(R_buff[5]==0x27&&R_len==9)
			{
				time_hour=(R_buff[7]<<8)|R_buff[8];
			}


			gettime[5]=0x28;
			LCD_Send_Data(gettime,7);delay_ms(20);
			LCD_Receive_Data(R_buff,&R_len);
			if(R_buff[5]==0x28&&R_len==9)
			{
				time_min=(R_buff[7]<<8)|R_buff[8];

			}


			gettime[5]=0x29;
			LCD_Send_Data(gettime,7);delay_ms(20);
			LCD_Receive_Data(R_buff,&R_len);
			if(R_buff[5]==0x29&&R_len==9)
			{
				time_sec=(R_buff[7]<<8)|R_buff[8];
			}

	u16 year,month,day,hour,min,sec;
	year=(((time_year%100)/10)<<4)|(((time_year%100)%10));
	month=(((time_month%100)/10)<<4)|(((time_month%100)%10));
	day=(((time_day%100)/10)<<4)|(((time_day%100)%10));
	hour=(((time_hour%100)/10)<<4)|(((time_hour%100)%10));
	min=(((time_min%100)/10)<<4)|(((time_min%100)%10));
	sec=(((time_sec%100)/10)<<4)|(((time_sec%100)%10));
	u8 time_buff[13]				={0x5A,0xA5,0x0A,0x80,0x1F,0x5A,0x18,0x1,0x22,0x00,0x14,0x40,0x00}; 	
	time_buff[6]=year;
	time_buff[7]=month;
	time_buff[8]=day;
	time_buff[10]=hour;
	time_buff[11]=min;
	time_buff[12]=sec;
	
	delay_ms(1);
	LCD_Send_Data(time_buff,13);
	delay_ms(1);
}
	

						//节能时间段
void Auto_sleep()
{
	u8 auto_sleep[8]={0x5A,0xA5,0x05,0x82,0x10,0x09,0x00,0x00};//节能开关 00关 01开

	if(LCD_AUTOSLEEP==1)
	{
		auto_sleep[7]=0x00;
		LCD_Send_Data(auto_sleep,8);
		LCD_AUTOSLEEP=0;
	}
	else 
	{
		LCD_AUTOSLEEP=1;
		auto_sleep[7]=0x01;
		LCD_Send_Data(auto_sleep,8);;
	}
}



		
			//屏幕初始化
void my_lcd_init (void)
{
	RELAY=0;
	delay_ms(1000);
	Load_LCD();
//	lcd_page(PAGE_MAIN);
}


void Set_Init(void)//设置初始化
{
	Load_LCD( );
	lcd_set_data (WENDU_UP_ADDR,WENDU_UPLIMIT);
	delay_us(100);
	lcd_set_data (WENDU_DOWN_ADDR,WENDU_DOWNLIMIT);
	delay_us(100);
	lcd_set_data (SHIDU_UP_ADDR,SHIDU_UPLIMIT);
	delay_us(100);
	lcd_set_data (SHIDU_DOWN_ADDR,SHIDU_DOWNLIMIT);
	delay_us(100);
	lcd_set_data (PM2_5_UP_ADDR,JINHUA_UPLIMIT);
	delay_us(100);
	lcd_set_data (LCD_POWER_HS_ADDR,LCD_POWER_HS);
	delay_us(100);
	lcd_set_data (LCD_POWER_SS_ADDR,LCD_POWER_SS);
	delay_us(100);
	lcd_set_data (LCD_POWER_HE_ADDR,LCD_POWER_HE);
	delay_us(100);
	lcd_set_data (LCD_POWER_SE_ADDR,LCD_POWER_SE);
	delay_us(100);
	u8 auto_sleep[8]={0x5A,0xA5,0x05,0x82,0x10,0x09,0x00,0x00};//节能开关 00关 01开
	auto_sleep[7]=LCD_AUTOSLEEP;
	LCD_Send_Data(auto_sleep,8);
	delay_us(100);
	
	
}


void Hand_Set_Init(void)//手动设置初始化按键
{
	if (KONGTIAO_STATE==1)
	{
		lcd_chan_state (WENDU_UP,1);
		delay_us(100);
		lcd_chan_state (WENDU_DOWN,0);
		delay_us(100);
	}
	else if (KONGTIAO_STATE==2)
	{
		lcd_chan_state (WENDU_UP,0);
		delay_us(100);
		lcd_chan_state (WENDU_DOWN,1);
		delay_us(100);
	}
	else
	{
		lcd_chan_state (WENDU_UP,0);
		delay_us(100);
		lcd_chan_state (WENDU_DOWN,0);
		delay_us(100);
	}

	if (HAND_STATE==1)//更新手动自动状态
	{
		lcd_chan_state (HAND,1);
		delay_us(100);
	}
	else
	{
		lcd_chan_state (HAND,0);
		delay_us(100);
	}
	
	
}




void BUT_UP_TEMP(void)//手动设置升温按键
{
	u8 meg[MESSEG_DATA]={0};
	meg[0]=2;
	meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
	meg[5]=1;
	send_messeg(RF_MESSEG,meg);
}


void BUT_DOWN_TEMP(void)//手动设置降温按键
{
	u8 meg[MESSEG_DATA]={0};
	meg[0]=2;
	meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
	meg[5]=2;
	send_messeg(RF_MESSEG,meg);
}




void BUT_UP_HUMI(void)
{
	u8 meg[MESSEG_DATA]={0};
	meg[0]=5;
	meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
	meg[5]=1;
	send_messeg(RF_MESSEG,meg);
}





void BUT_DOWN_HUMI(void)
{
	u8 meg[MESSEG_DATA]={0};
	meg[0]=3;
	meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
	meg[5]=2;
	send_messeg(RF_MESSEG,meg);
}


					
						//空气净化机
void BUT_PM(void)
{
	u8 meg[MESSEG_DATA]={0};
	meg[0]=4;
	meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
	meg[5]=2;
	send_messeg(RF_MESSEG,meg);

}




			//进入一个页面
void lcd_page (u8 page)
{
	LCD_PAGE[5]=page;
	LCD_Send_Data(LCD_PAGE,6);
}

				//改变一个开关的状态
void lcd_chan_state (u8 type,u8 state)
{
	LCD_STATE[5]=type;
	LCD_STATE[7]=state;
	LCD_Send_Data(LCD_STATE,8);
	
}

					//屏幕消息处理，02
void lcd_messeg_02 (u8 *meg)
{
	if (meg[2]==WENDU_UP)
	{
		if (meg[1]==1)
		{
			KONGTIAO_STATE=1;
		}
		else
		{
			KONGTIAO_STATE=0;
		}
	}
	else if (meg[2]==WENDU_DOWN)
	{
		if (meg[1]==1)
		{
			KONGTIAO_STATE=2;
		}
		else
		{
			KONGTIAO_STATE=0;
		}
		
	}
	else if (meg[2]==SHIDU_UP)
	{
		if (meg[1]==1)
		{
			CHUSHI_STATE=1;
		}
		else
		{
			CHUSHI_STATE=0;
		}
		
	}
	else if (meg[2]==SHIDU_DOWN)
	{
		if (meg[1]==1)
		{
			CHUSHI_STATE=2;
		}
		else
		{
			CHUSHI_STATE=0;
		}
		
	}
	else if (meg[2]==JINHUA_)
	{
		if (meg[1]==1)
		{
			JINHUA_STATE=1;
		}
		else
		{
			JINHUA_STATE=0;
		}
		
	}
	
	if (KONGTIAO_STATE==1)
	{
		lcd_chan_state (WENDU_UP,1);
		delay_us(100);
		lcd_chan_state (WENDU_DOWN,0);
		delay_us(100);
	}
	else if (KONGTIAO_STATE==2)
	{
		lcd_chan_state (WENDU_UP,0);
		delay_us(100);
		lcd_chan_state (WENDU_DOWN,1);
		delay_us(100);
	}
	else
	{
		lcd_chan_state (WENDU_UP,0);
		delay_us(100);
		lcd_chan_state (WENDU_DOWN,0);
		delay_us(100);
	}
	
	if (CHUSHI_STATE==1)
	{
		lcd_chan_state (SHIDU_UP,1);
		delay_us(100);
		lcd_chan_state (SHIDU_DOWN,0);
		delay_us(100);
	}
	else if (CHUSHI_STATE==2)
	{
		lcd_chan_state (SHIDU_UP,0);
		delay_us(100);
		lcd_chan_state (SHIDU_DOWN,1);
		delay_us(100);
	}
	else
	{
		lcd_chan_state (SHIDU_UP,0);
		delay_us(100);
		lcd_chan_state (SHIDU_DOWN,0);
		delay_us(100);
	}
	
	if (JINHUA_STATE==1)
	{
		lcd_chan_state (JINHUA,1);
		delay_us(100);
	}
	else
	{
		lcd_chan_state (JINHUA,0);
		delay_us(100);
	}
}

#define  LCD_POWER PBout(2)





				//立即睡眠
void LCD_tosleep (void)
{
	u8 meg[MESSEG_DATA]={0};
	u8 light[MESSEG_DATA]={0};
		meg[0]=3;meg[1]=1;
		meg[2]=0;meg[3]=0;
		meg[4]=1;meg[5]=0;
		light[0]=1;light[1]=LCD_STATE_;
		light[2]=0;//亮度改为0
		send_messeg(KEY_MESSEG,light);
		light[0]=2;light[1]=1;
		light[2]=LCD_STATE_;
		send_messeg(LIT_MESSEG,light);
		send_messeg(LCD_MESSEG,meg);//
	
}


				//立即亮屏
void LCD_tolight (void)
{
	u8 meg[MESSEG_DATA]={0};
	u8 light[MESSEG_DATA]={0};
		meg[0]=3;meg[1]=1;
		meg[2]=0x3f;meg[3]=0;
		meg[4]=1;meg[5]=0;
		light[0]=1;light[1]=LCD_STATE_;
		light[2]=0x3f;//亮度改为3f
		send_messeg(KEY_MESSEG,light);
		light[0]=2;light[1]=0;
		light[2]=LCD_STATE_;
		send_messeg(LIT_MESSEG,light);
		send_messeg(LCD_MESSEG,meg);//
	
}









				//屏幕电源控制
void	lcd_messeg_03 (u8 *meg)
{
	u8 dd[6]={0x5A, 0xA5, 0x03, 0x80, 0x01,0};
	static u8 power=0;
	static u8 lcd_light=0x3f;
	if (meg[0]==0x03)
	{
		if (meg[1]==1)//屏幕是开着的，亮度控制
		{
			dd[5]=meg[2];//屏幕亮度
			LCD_Send_Data(dd,6);
			if (meg[2]!=0)
			{
				lcd_light=meg[2];
			}
		}
		else	if (meg[1]==0)//屏幕关，关继电器
		{
			if (meg[2]==1)
			{
				LCD_POWER=0;
				power=1;
			}
			else 
			{
				LCD_POWER=1;
			}
		}
	}
	if (power>0)
	{
		delay_ms(1000);
		lcd_page (PAGE_MAIN);
		delay_ms(30);
		dd[5]=lcd_light;//屏幕亮度
		LCD_Send_Data(dd,6);
		power=0;
	}
}

void lcd_timetosleep(void)
{
	if (LCD_AUTOSLEEP)
	{
		static u16 i=0;
		i++;
		if (i>=100)
		{
			i=0;
			u8 gettime[6]={0x5A,0xA5,0x03,0x81,0x24,0x03};//获取时间
			u8 R_buff[10]={0};u16 R_len=0;
			u16 time_hour,time_min,time_sec;
			LCD_Send_Data(gettime,6);delay_ms(20);
			LCD_Receive_Data(R_buff,&R_len);
			if(R_buff[4]==0x24&&R_len==9)
			{
				time_hour=(R_buff[6]>>4)*10+(R_buff[6]&0x0f);
				time_min=(R_buff[7]>>4)*10+(R_buff[7]&0x0f);
				time_sec=(R_buff[8]>>4)*10+(R_buff[8]&0x0f);
			}


			if ((time_hour==LCD_POWER_HS)&&(time_min==LCD_POWER_SS)&&(time_sec<=7)) 
			{
					LCD_tosleep();
			}
			else if ((time_hour==LCD_POWER_HE)&&(time_min==LCD_POWER_SE)&&(time_sec<=7)) 
			{
				LCD_tolight();
			}

			
		}
	}

	
}





void lcd_set_data (u8 type,u16 data)
{
	LCD_SET_DATA[5]=type;
	LCD_SET_DATA[6]=data>>8;
	LCD_SET_DATA[7]=data;
	LCD_Send_Data(LCD_SET_DATA,8);

}












void Set_Save(void)//设置页面保存
{
	u8 buf[20]={0};
	u16 reclen=0;
	LCD_GET_DATA[5]=WENDU_UP_ADDR;
	LCD_Send_Data(LCD_GET_DATA,7);
	delay_ms(20);
	LCD_Receive_Data(buf,&reclen);
	if(buf[5]==0x14&&reclen==9)
	{
		WENDU_UPLIMIT=(buf[7]<<8)|buf[8];
	}
	
	reclen=0;
	LCD_GET_DATA[5]=WENDU_DOWN_ADDR;
	LCD_Send_Data(LCD_GET_DATA,7);
	delay_ms(20);
	LCD_Receive_Data(buf,&reclen);
	if(buf[5]==WENDU_DOWN_ADDR&&reclen==9)
	{
		WENDU_DOWNLIMIT=(buf[7]<<8)|buf[8];
	}

	reclen=0;
	LCD_GET_DATA[5]=SHIDU_UP_ADDR;
	LCD_Send_Data(LCD_GET_DATA,7);
	delay_ms(20);
	LCD_Receive_Data(buf,&reclen);
	if(buf[5]==SHIDU_UP_ADDR&&reclen==9)
	{
		SHIDU_UPLIMIT=(buf[7]<<8)|buf[8];
	}
	
	reclen=0;
	LCD_GET_DATA[5]=SHIDU_DOWN_ADDR;
	LCD_Send_Data(LCD_GET_DATA,7);
	delay_ms(20);
	LCD_Receive_Data(buf,&reclen);
	if(buf[5]==SHIDU_DOWN_ADDR&&reclen==9)
	{
		SHIDU_DOWNLIMIT=(buf[7]<<8)|buf[8];
	}

	reclen=0;
	LCD_GET_DATA[5]=PM2_5_UP_ADDR;
	LCD_Send_Data(LCD_GET_DATA,7);
	delay_ms(20);
	LCD_Receive_Data(buf,&reclen);
	if(buf[5]==PM2_5_UP_ADDR&&reclen==9)
	{
		JINHUA_UPLIMIT=(buf[7]<<8)|buf[8];
	}
	
					//节能模式时间区间
	reclen=0;
	LCD_GET_DATA[5]=LCD_POWER_HS_ADDR;
	LCD_Send_Data(LCD_GET_DATA,7);
	delay_ms(20);
	LCD_Receive_Data(buf,&reclen);
	if(buf[5]==LCD_POWER_HS_ADDR&&reclen==9)
	{
		LCD_POWER_HS=(buf[7]<<8)|buf[8];
	}
	
	reclen=0;
	LCD_GET_DATA[5]=LCD_POWER_SS_ADDR;
	LCD_Send_Data(LCD_GET_DATA,7);
	delay_ms(20);
	LCD_Receive_Data(buf,&reclen);
	if(buf[5]==LCD_POWER_SS_ADDR&&reclen==9)
	{
		LCD_POWER_SS=(buf[7]<<8)|buf[8];
	}

	reclen=0;
	LCD_GET_DATA[5]=LCD_POWER_HE_ADDR;
	LCD_Send_Data(LCD_GET_DATA,7);
	delay_ms(20);
	LCD_Receive_Data(buf,&reclen);
	if(buf[5]==LCD_POWER_HE_ADDR&&reclen==9)
	{
		LCD_POWER_HE=(buf[7]<<8)|buf[8];
	}

	reclen=0;
	LCD_GET_DATA[5]=LCD_POWER_SE_ADDR;
	LCD_Send_Data(LCD_GET_DATA,7);
	delay_ms(20);
	LCD_Receive_Data(buf,&reclen);
	if(buf[5]==LCD_POWER_SE_ADDR&&reclen==9)
	{
		LCD_POWER_SE=(buf[7]<<8)|buf[8];
	}
	
	Save_LCD();
	u8 m_send[MESSEG_DATA]={0};
	m_send[0]=6;
	send_messeg (WK_MESSEG,m_send);//发送设置变更到上位机
	
}















