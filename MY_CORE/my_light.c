
#include "my_light.h"
#include "includes.h"
#include "delay.h"
/*****************************************

		meg的位定义
		meg[0],1,键盘上的灯，2，设备状态指示灯3，灯带，4，键盘上的灯闪烁
		meg[1],亮或灭,灯带闪烁的类型，1，正向流水，2，反向流水，3，正向双流水，4，反向双流水，5，纯色，6，渐变至纯色
						键盘灯闪烁的时候闪烁的位置
		meg2，亮或灭第几个,灯带闪烁1,开始，0，停止，2，自动换色
						键盘灯闪烁的时候闪烁次数，
		meg3,灯带红色
						键盘灯闪烁的时候亮灯时间(单位，10ms)
		meg4，灯带绿色
						键盘灯闪烁的时候灭灯时间
		meg5，灯带蓝色
						键盘灯闪烁的时候启动还是停止

		详细的消息定义参见my_messeg.h文件

***************************************/



		//灯光任务函数
void my_light (void * t)//这个任务出现了独占CPU的情况2018.12.20
{
	u8 messeg[MESSEG_DATA]={0};
	u8 mode=0;//纯色还是流水
	Light_init();
	while (1)
	{
		delay_ms(40);
		if (get_messeg (LIT_MESSEG,messeg))
		{
			if (mode<=4)//流水灯模式
			{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
				led_run (messeg);
			}
			else if (mode==6)//渐变至纯色
			{
				light_runto(messeg);  
			}
		}
		else 
		{
			if (messeg[0]==LIGHT_WARN_STATE)
			{
				key_light (messeg[2],messeg[1]);
			}
			else if (messeg[0]==LIGHT_DEVICE_STATE)
			{
				key_around (messeg[2],messeg[1]);
			}
			else if (messeg[0]==LIGHT_ROUND_LIGHT)
			{
				mode=messeg[1];
				if (mode<=4)
				{
					led_run (messeg);
				}
				else if (mode==LIGHT_LIGHT_COLOR)
				{
					led_lightall (messeg);
				}
				else if (mode==6)
				{
					light_runto(messeg);  
				}
			}
		}
	}
}


//灯带的实时颜色
static u8 red,green,blue;



					//键盘灯闪烁
void light_run (u8 *msg)
{
	static u8 light_state[6]={0};//关灯
	static u8 times[6]={0};//闪烁次数
	static u8 light[6]={0};
	static u8 nolight[6]={0};
	u8 i=0;
	while (1)
	{
		if (msg[0]==4)//键盘灯闪烁,更新命令
		{
			times[msg[1]]=msg[2];//闪烁次数
			light[msg[1]]=msg[3];//亮灯时间
			nolight[msg[1]]=msg[4];//灭灯时间
			light_state[msg[1]]=msg[5];
		}
		if (times[0]>0)							//由于没有想到好的算法，暂时6个灯同一使用第一个灯的参数
		{
			times[0]--;				
			for (i=0;i<6;i++)
			{
				if (light_state[i]==1)
					key_around (i+1,1);
			}
			delay_ms(light[0]*10);
			for (i=0;i<6;i++)
			{
				if (light_state[i]==1)
					key_around (i+1,0);
			}
			delay_ms(nolight[0]*10);
			if (times[0]==0) 
			for (i=0;i<6;i++)
			{
				light_state[i]=0;//闪烁次数到，关
			}
		}
		
	}
	
}


		//渐变步长
#define RUNTO_SKIP 5

//周围灯光渐变至指定颜色
void light_runto (u8 *msg)
{
	static u8 red_=0,green_=0,blue_=0;
	static u8 red_old=0,green_old=0,blue_old=0;
	static u8 mode=0;//闪烁模式
	static u8 first=0;
	if (msg[0]==LIGHT_ROUND_LIGHT)
	{
		if (msg[1]==6)
		{
			mode=msg[1];
			if (msg[2]==LIGHT_LIGHT_STCOR)
			{
				red_=msg[3];
				green_=msg[4];
				blue_=msg[5];
				if (first==0)
				{red_old=red;green_old=green;blue_old=blue;first=1;}
			}
			else
			{
				if (first==1)
				{
					red_=red_old;
					green_=green_old;
					blue_=blue_old;
					red=red_old;green=green_old;blue=blue_old;
					first=0;
				}
			}
		}
		else
		{
			mode=0;
		}
	}
	
	if (mode==LIGHT_LIGHT_RUNTO)
	{
		if (red_>red+RUNTO_SKIP){
			red+=RUNTO_SKIP;
		}else if (red_+RUNTO_SKIP<red){
			red-=RUNTO_SKIP;
		}else{
			red=red_;
		}
		if (green_>green+RUNTO_SKIP){
			green+=RUNTO_SKIP;
		}else if (green_+RUNTO_SKIP<green){
			green-=RUNTO_SKIP;
		}else{
			green=green_;
		}
		if (blue_>blue+RUNTO_SKIP){
			blue+=RUNTO_SKIP;
		}else if (blue_+RUNTO_SKIP<blue){
			blue-=RUNTO_SKIP;
		}else{
			blue=blue_;
		}
		led_setall (red,green,blue);
		led_senddata();
		light_setcolor(red,green,blue);//设置新版灯光带的颜色2019.1.18
		if ((red_==red)&&(green_==green)&&(blue_==blue))
		{
			mode=0;
		}
	}
}









//state 1,正向递减渐变，2，反向递减渐变，3，正向双，4，反向双
void led_light (u16 num,u8 state,u8 red,u8 green,u8 blue)
{
	u16 led_num[20]={0};
	u16 led_num1[20]={0};
	u16 num1=(num+LED_NUM/2)%LED_NUM;
	u16 led_li_num=7;//流水时亮着的灯数目
	u8 red1=255-red,green1=255-green,blue1=255-blue;
	u16 i=0;
	if (LED_NUM>80)
	{
		led_li_num=20;
	}
	else
	{
		led_li_num=LED_NUM/4;
	}
	for (i=0;i<led_li_num;i++)
	{
		if (num>=i)
		{
			if ((state==1)||(state==3))
				led_num[i]=num-i;
			else if ((state==2)||(state==4))
				led_num[led_li_num-1-i]=num-i;
		}
		else
		{
			if ((state==1)||(state==3))
				led_num[i]=LED_NUM+num-i;
			else if ((state==2)||(state==4))
				led_num[led_li_num-1-i]=LED_NUM+num-i;
		}
	}
	for (i=0;i<led_li_num;i++)
	{
		if (num1>=i)
		{
			if (state==3)
				led_num1[i]=num1-i;
			else if (state==4)
				led_num1[led_li_num-1-i]=num1-i;
		}
		else
		{
			if (state==3)
				led_num1[i]=LED_NUM+num1-i;
			else if (state==4)
				led_num1[led_li_num-1-i]=LED_NUM+num1-i;
		}
	}
	for (i=0;i<led_li_num;i++)
	{
		led_setcolor(led_num[i],red,green,blue);
	}
	if (state==3||state==4)
	{
		for (i=0;i<led_li_num;i++)
		{
			led_setcolor(led_num1[i],red1,green1,blue1);
		}
	}
	led_senddata();
}

			//灯带亮纯色
void led_lightall (u8 *meg)
{
	static u8 mode;
	if (meg[0]==3)
	{
		if (meg[1]==5)
		{
			red=meg[3];green=meg[4];blue=meg[5];mode=meg[1];
			led_setall (red,green,blue);
			led_senddata();
			light_setcolor(red,green,blue);//设置新版灯光带的颜色2019.1.18
		}
	}
	else if (mode==5)
	{
			led_setall (red,green,blue);
			led_senddata();
	}
}


			//流水灯
void led_run (u8 *meg)
{
	static u8 led_state=0;//关灯带
	static u16 run_num=0;
	static u8 tt[1]={0};
	static u8 color_state[3]={0};//自动换色标志
	if (meg[0]==3)
	{
		led_state=meg[2];
		
		if (led_state>=1)//开始
		{
			tt[0]=meg[1];
			red=meg[3];//颜色
			green=meg[4];
			blue=meg[5];
		}
		else if (led_state==0)//停止
		{
			led_resetall ( );
			tt[0]=0;
			red=0;
			green=0;
			blue=0;
			led_senddata();
			light_setcolor(0,0,0);
		}
	}
	
	if (led_state>=1)
	{
		led_resetall ( );
		led_light (run_num,tt[0],red,green,blue);
		light_setcolor(red,green,blue);//新版灯光带2019.1.18
		if (tt[0]&0x01) 
		{
			run_num++;
			if (run_num>=LED_NUM)
				run_num=0;
		}
		else 
		{
			if (run_num==0)
				run_num=LED_NUM-1;
			run_num--;
		}
		if (led_state==2)//自动换色
		{
		
			if (color_state[0]==0)
			{
				if (red<255)
				{
					red++;
				}
				else
				{
					color_state[0]=1;
				}
			}
			else
			{
				if (red>0)
				{
					red-=1;
				}
				else
				{
					color_state[0]=0;
				}
			}
			//--------------------------
			if (color_state[1]==0)
			{
				if (green<251)
				{
					green+=2;
				}
				else
				{
					color_state[1]=1;
				}
			}
			else
			{
				if (green>3)
				{
					green-=2;
				}
				else
				{
					color_state[1]=0;
				}
			}
			//--------------------------
			if (color_state[2]==0)
			{
				if (blue<250)
				{
					blue+=3;
				}
				else
				{
					color_state[2]=1;
				}
			}
			else
			{
				if (blue>4)
				{
					blue-=3;
				}
				else
				{
					color_state[2]=0;
				}
			}	
		}
	}
}


/*****************************************************

			新版集中器key_light函数在light.c文件下
			新版集中器key_around函数在light.c文件下

					//亮按键灯
void key_light (u8 key_num,u8 state)
{
	u8 t;
	switch (key_num)
	{
		case 1:
			t=LED1;break;
		case 2:
			t=LED2;break;
		case 3:
			t=LED3;break;
		case 4:
			t=LED4;break;
		case 5:
			t=LED5;break;
		case 6:
			t=LED6;break;
		default :
			break;
	}
	if (state)
	{
		on_light(t,LIGHT_LED);
	}
	else
	{
		off_light(t,LIGHT_LED);
	}
}
			//按键周围的灯
void key_around (u8 key_num,u8 state)
{
	u8 data[6]={0};
	u8 tt=0;
			//hoqu亮灯位
	Get_lightdata (data);

	if (state==1)
	{
		tt=0xff;
	}
	else if (state==2)
	{
		tt=0x55;
	}
	else if (state==3)
	{
		tt=0xaa;
	}
	else if (state==4)
	{
		tt=0x3c;
	}
	switch (key_num)
	{
		case 1:
			data[0]&=0x80;
			data[1]&=0x01;
			data[0]|=tt>>1;
			data[1]|=tt<<7;
			break;
		case 2:
			data[0]&=0xfe;
			data[1]&=0x01;
			data[0]|=tt<<7;
			data[1]|=tt>>1;
			break;
		case 3:
			data[2]&=0xc0;
			data[3]&=0x03;
			data[2]|=tt>>2;
			data[3]|=tt<<6;
			break;
		case 4:
			data[2]&=0x03;
			data[3]&=0xc0;
			data[2]|=tt<<6;
			data[3]|=tt>>2;
			break;
		case 5:
			data[4]=0;
			data[4]=tt;
			break;
		case 6:
			data[5]=0;
			data[5]=tt;
			break;
		default:
			break;
			
	}
	
	Set_lightdata(data);
	HC595_SendData( );
}
*********************************************/
