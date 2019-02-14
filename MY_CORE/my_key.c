#include "my_key.h"
#include "my_light.h"
#include "includes.h"
#include "beep.h"
#include "my_lcd.h"
#include "my_messeg.h"


/************************************

			meg位定义
			meg0，1改变按键状态，
			meg1，改变的按键状态位置
			meg2新状态

		详细的消息定义参见my_messeg.h文件

**********************************/



	static u8 key[6]={0};
	static u8 state[6]={0,0,0,0,0x3f,0};//短按状态
	static u8 statel[6]={0,0,0,0,1,0};//长安状态

u8 key_color[3][6]={0};
	
					//获取按键状态
u8 get_keystate(u8 key_num)
{
	return state[key_num-1];
}
	


			//按键任务函数
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
		if (get_messeg(KEY_MESSEG,recv)==0)//在屏幕上关屏之后开不了bug，2018.11.5
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
				light[0]=5;//屏幕蜂鸣器叫
				light[1]=1;
				send_messeg(LCD_MESSEG,light);
				
				light[0]=LIGHT_ROUND_LIGHT;
				light[2]=LIGHT_LIGHT_STOP;
				light[1]=LIGHT_LIGHT_RUNTO;
				send_messeg(LIT_MESSEG,light);
			}
			else if (key[i]==PRESS_LONG)
			{
				light[0]=5;//屏幕蜂鸣器叫
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
		
		if (key[0]==PRESS_SHORT) //按键1短按
		{
			meg[0]=2;//空调
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=1;//升按键
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[0]==PRESS_LONG)//按键1长安
		{
			if (statel[0]==0)
			{
				TaskPend(8);//挂起自动控制
				Load_down();
			}
			else
			{
				TaskRepend(8);//自动控制
				Load_up();
			}
			statel[0]=!statel[0];
		}

		if (key[1]==PRESS_SHORT) //按键2短按
		{
			meg[0]=2;//空调
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=2;//降按键
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[1]==PRESS_LONG)//按键2长安
		{
			if (statel[1]==0)
			{
				meg[0]=3;meg[1]=4;//彩灯
				meg[2]=2;//闪烁
				meg[3]=200;meg[4]=100;meg[5]=50;//色彩值
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=1;
				Lcd_SetHandstate(1);
			}
			else if (statel[1]==1)
			{
				meg[0]=3;meg[1]=3;//彩灯
				meg[2]=2;//闪烁
				meg[3]=200;meg[4]=100;meg[5]=50;//色彩值
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=2;
				Lcd_SetHandstate(1);
			}
			else if (statel[1]==2)
			{
				meg[0]=3;meg[1]=4;//彩灯
				meg[2]=0;//停止
				meg[3]=200;meg[4]=100;meg[5]=50;//色彩值
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=0;
				Lcd_SetHandstate(0);
			}
		}


		if (key[2]==PRESS_SHORT) //按键3短按
		{
			meg[0]=5;//加湿机
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=1;//升按键
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[2]==PRESS_LONG)//按键3长安
		{
			if (statel[2]==0)
			{
				Load_song3();//一首歌会占用很大的存储空间
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

		if (key[3]==PRESS_SHORT) //按键4短按
		{
			meg[0]=3;
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=2;
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[3]==PRESS_LONG)//按键4长安
		{
			meg[0]=2;
			send_messeg(CFG_MESSEG,meg);//切换配置模式
		}


		if (key[4]==PRESS_SHORT) //按键5短按，屏幕控制
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
			else   //背光为0时亮灯
			{
				meg[0]=3;meg[1]=1;
				meg[2]=state[4];meg[3]=0;
				meg[4]=1;meg[5]=0;
				light[0]=2;light[1]=1;
				light[2]=LIGHT_LCD_STATE;
			}
			send_messeg(LIT_MESSEG,light);//空调升温按键
			send_messeg(LCD_MESSEG,meg);//
		}
		else if (key[4]==PRESS_LONG)//按键5长安
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
				send_messeg(LIT_MESSEG,light);//空调升温按键
				send_messeg(LCD_MESSEG,meg);//
			}
			light[0]=2;light[1]=0;
			light[2]=LIGHT_LCD_STATE;
			send_messeg(LIT_MESSEG,light);
		}



		
		if (key[5]==PRESS_SHORT) //按键6短按
		{
			meg[0]=4;
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=2;
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[5]==PRESS_LONG)//按键6长安
		{
		}
//		delay_ms(100);
	}
}





