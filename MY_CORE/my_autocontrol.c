

#include "my_autocontrol.h"
#include "my_messeg.h"
#include "delay.h"
#include "my_lcd.h"
#include "my_rf.h"
//自动控制实现文件

/*************************************************************


2018.8.2 加入自动控制，
2018.8.3 加入环境值异常灯光效果
					正常时灯带显示绿色，异常时灯带显示红色，同时异常的环境值对应的按键周围的灯圈会警示
					处于手动时常亮，处于自动时闪烁，直至恢复正常值后熄灭




************************************************************/


//超调值
#define OVERSHOOT_RANGE 3
//1使用旧版，0使用新版，旧版串口屏设置是以1mg/m3为单位，新版以0.1mg/m3为单位
#define __USE_OLD   1

						//自动控制循环
void my_autocontrol (void * t)
{
	u8 m_send[MESSEG_DATA]={0};
	u8 *cj_data;
//	u8 auto_state[3]={0};//新增自动状态标志位，用于判断自动控制停止
	float now_temp;//环境值变量改为浮点型，比较时比较小数，2018.11.19
	float now_humi;
	float now_tvoc;
	u8 en_warning=0;
	cj_data=GetCJDataAddr ( );
	delay_ms(3000);//系统运行3秒钟之后开始执行
	while (1)
	{
		delay_ms(30000);//每30秒执行一次自动判断
		{
			if (cj_data[1]!=0xff) continue;
			now_temp=cj_data[17]+cj_data[18]/10.;//传入小数，2018.11.19
			now_humi=cj_data[19]+cj_data[20]/10.;
			now_tvoc=cj_data[21]+cj_data[22]/10.;
			
									/*空调的自动控制*/
			if (now_temp>=Lcd_GetLimitData (0))
			{
				m_send[0]=2;
				m_send[1]=2;m_send[2]=0;m_send[3]=0;
				m_send[4]=1;
				m_send[5]=2;
				if (Lcd_GetHandstate()==0)//自动模式
				{
					send_messeg (RF_MESSEG,m_send);
				}
				
				m_send[2]=1;//闪烁的位置
				m_send[0]=1;m_send[1]=1;
				send_messeg (LIT_MESSEG,m_send);
					
				en_warning=1;
				send_warn(Get_MyAddr(),0,1,cj_data[17]);//发送报警到网口
				
			}
			else if (now_temp<=Lcd_GetLimitData (1))
			{
				m_send[0]=2;
				m_send[1]=2;m_send[2]=0;m_send[3]=0;
				m_send[4]=1;
				m_send[5]=1;
				if (Lcd_GetHandstate()==0)//自动模式
				{
					send_messeg (RF_MESSEG,m_send);
				}
				m_send[2]=2;//闪烁的位置
				m_send[0]=1;m_send[1]=1;
				send_messeg (LIT_MESSEG,m_send);
					
				en_warning=2;
				
				send_warn(Get_MyAddr(),0,2,cj_data[17]);//发送报警到网口
			}
			else if ((now_temp+OVERSHOOT_RANGE<Lcd_GetLimitData (0))&&(now_temp>Lcd_GetLimitData (1)+OVERSHOOT_RANGE))
			{
				m_send[0]=2;
				m_send[1]=2;m_send[2]=0;m_send[3]=0;
				m_send[4]=0;
				m_send[5]=0;
				if (Lcd_GetHandstate()==0)//自动模式
				{
					send_messeg (RF_MESSEG,m_send);
				}
				m_send[2]=1;//闪烁的位置
				m_send[0]=1;m_send[1]=0;
				send_messeg (LIT_MESSEG,m_send);
				m_send[2]=2;//闪烁的位置
				m_send[0]=1;m_send[1]=0;
				send_messeg (LIT_MESSEG,m_send);
			}
			else					//在超调区间，改报警为不报警，但是设备继续运行
			{
				m_send[2]=1;//闪烁的位置
				m_send[0]=1;m_send[1]=0;
				send_messeg (LIT_MESSEG,m_send);
				m_send[2]=2;//闪烁的位置
				m_send[0]=1;m_send[1]=0;
				send_messeg (LIT_MESSEG,m_send);
			}
									/*除湿加湿的自动控制*/
			if (now_humi>=Lcd_GetLimitData (2))
			{
				m_send[0]=3;
				m_send[1]=2;m_send[2]=0;m_send[3]=0;
				m_send[4]=1;
				m_send[5]=2;
				if (Lcd_GetHandstate()==0)//自动模式
				{
					send_messeg (RF_MESSEG,m_send);
				}
				m_send[2]=3;//闪烁的位置
				m_send[0]=1;m_send[1]=1;
				send_messeg (LIT_MESSEG,m_send);
					
				en_warning=3;
				send_warn(Get_MyAddr(),0,3,cj_data[19]);//发送报警到网口
			}
			else if (now_humi<=Lcd_GetLimitData (3))
			{
				m_send[0]=5;
				m_send[1]=2;m_send[2]=0;m_send[3]=0;
				m_send[4]=1;
				m_send[5]=1;
				if (Lcd_GetHandstate()==0)//自动模式
				{
					send_messeg (RF_MESSEG,m_send);
				}
				m_send[2]=4;//闪烁的位置
				m_send[0]=1;m_send[1]=1;
				send_messeg (LIT_MESSEG,m_send);
				en_warning=4;
				send_warn(Get_MyAddr(),0,4,cj_data[19]);//发送报警到网口
			}
			else if ((now_humi+OVERSHOOT_RANGE<Lcd_GetLimitData (2))&&(now_humi>Lcd_GetLimitData (3)+OVERSHOOT_RANGE))
			{
				m_send[0]=3;
				m_send[1]=2;m_send[2]=0;m_send[3]=0;
				m_send[4]=0;
				m_send[5]=0;
				if (Lcd_GetHandstate()==0)//自动模式
				{
					send_messeg (RF_MESSEG,m_send);
					m_send[0]=5;
					send_messeg (RF_MESSEG,m_send);
				}
				m_send[2]=3;//闪烁的位置
				m_send[0]=1;m_send[1]=0;
				send_messeg (LIT_MESSEG,m_send);
				m_send[2]=4;//闪烁的位置
				m_send[0]=1;m_send[1]=0;
				send_messeg (LIT_MESSEG,m_send);				
			}
			else
			{
				m_send[2]=3;//闪烁的位置
				m_send[0]=1;m_send[1]=0;
				send_messeg (LIT_MESSEG,m_send);
				m_send[2]=4;//闪烁的位置
				m_send[0]=1;m_send[1]=0;
				send_messeg (LIT_MESSEG,m_send);
			}
									/*空气净化机的自动控制*/
#if __USE_OLD==0
			if (now_tvoc*10>=Lcd_GetLimitData (4))//TVOC上限是乘以10之后的数据，在这里把tvoc数据*10后比较,2018.11.19
			{
#else
			if (now_tvoc>=Lcd_GetLimitData (4))//TVOC上限是乘以10之后的数据，在这里把tvoc数据*10后比较,2018.11.19
			{
#endif
				m_send[0]=4;
				m_send[1]=2;m_send[2]=0;m_send[3]=0;
				m_send[4]=1;
				m_send[5]=2;
				if (Lcd_GetHandstate()==0)//自动模式
				{
					send_messeg (RF_MESSEG,m_send);
				}
				m_send[2]=6;//闪烁的位置
				m_send[0]=1;m_send[1]=1;
				send_messeg (LIT_MESSEG,m_send);
					
				en_warning=6;
				send_warn(Get_MyAddr(),0,5,cj_data[21]);//发送报警到网口
			}
			else
			{
				m_send[0]=4;
				m_send[1]=2;m_send[2]=0;m_send[3]=0;
				m_send[4]=0;
				m_send[5]=0;
				if (Lcd_GetHandstate()==0)//自动模式
				{
					send_messeg (RF_MESSEG,m_send);
				}
				m_send[2]=6;//闪烁的位置
				m_send[0]=1;m_send[1]=0;
				send_messeg (LIT_MESSEG,m_send);
			}
			if (en_warning)		//灯带异常指示
			{
				m_send[0]=3;
				m_send[1]=5;
				m_send[3]=200;m_send[4]=0;m_send[5]=0;
				send_messeg (LIT_MESSEG,m_send);
				en_warning=0;
			}
			else
			{
				m_send[0]=3;
				m_send[1]=5;
				m_send[3]=0;m_send[4]=200;m_send[5]=0;
				send_messeg (LIT_MESSEG,m_send);
			}
		}
	}
}


//报警设备地址，设备类型，报警类型，报警时的值
void send_warn(u16 addr,u8 devtype,u8 warntype,u8 ddd)
{
	u8 send[MESSEG_DATA]={0};
	send[0]=7;
	send[1]=warntype;
	send[2]=devtype;
	send[3]=addr>>8;
	send[4]=addr;
	send[5]=ddd;
		if (find_messeg(WK_MESSEG)==0)
	send_messeg(WK_MESSEG,send);
}






