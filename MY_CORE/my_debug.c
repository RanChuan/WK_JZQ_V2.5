
#include "my_debug.h"
#include "string.h"
#include "stdio.h"
#include "data_map.h"//获取配置参数
#include "enternet.h"
#include "dns.h"
#include "baidu_iot.h"


extern u8 NET_S1_STATE;

void my_debug (void)
{
	static u8 net_state_old=0;
	if (net_get_comstate(1)==SOCK_CLOSED)
	{
		if (udp_init(1,12))
		{
			dbg_booting();
		}
	}
	net_state_old|=NET_S1_STATE;
	if (net_state_old&IR_RECV)//如果有数据等待接收
	{
		net_state_old&=~IR_RECV;
		dbg_Interpreter();//命令解释器
	}
	NET_S1_STATE=0;
}

/*****************************************************

						命令解释器：解释从网口传来的命令并执行
						help：显示帮助信息
						sysinfo：显示系统信息
						setip：设置网络参数


******************************************************/

u8 DBG_OCHE =0;//回显
u8 DBG_IP[4]={255,255,255,255};//调试用的目标ip地址
u16 DBG_PORT=7000;//调试用的端口号
void dbg_Interpreter(void)
{
	u8 *recvbuff=mymalloc(2048);
	memset(recvbuff,0,2048);//清空内存中的数据
	Read_SOCK_Data_Buffer(1, recvbuff);
	
	//重新设置调试主机目标地址
	mymemcpy(DBG_IP,recvbuff,4);
	DBG_PORT=(recvbuff[4]<<8)|recvbuff[5];

	if (DBG_OCHE)
	{
		udp_send(1,DBG_IP,DBG_PORT,(u8*)recvbuff+8,strlen((const char *)recvbuff+8));
		udp_send(1,DBG_IP,DBG_PORT,(u8*)"\r\n",2);
	}
	
	
	if (samestr((u8*)"sysinfo",recvbuff+8))
	{
		dbg_info();
	}
	else if (samestr((u8*)"help",recvbuff+8))
	{
		dbg_help();
	}
	else if (samestr((u8*)"devconfig",recvbuff+8))
	{
		dbg_devconfig();
	}
	else if (samestr((u8*)"reboot",recvbuff+8))
	{
		dbg_reboot(); 
	}
	else if (samestr((u8*)"oche ",recvbuff+8))
	{
		dbg_oche(recvbuff+8+5); 
	}
	else if (samestr((u8*)"copy ",recvbuff+8))
	{
		dbg_copydata(recvbuff+8+5); 
	}
	else if (samestr((u8*)"getip ",recvbuff+8))
	{
		dbg_getip(recvbuff+8+6); 
	}
	else if (samestr((u8*)"setchannel ",recvbuff+8))
	{
		dbg_setchanel(recvbuff+8+11); 
	}
	else if (samestr((u8*)"mqtt",recvbuff+8))
	{
		dbg_mqtt(recvbuff+8+6); 
	}
	else
	{
		dbg_err(1);
	}

	
	myfree(recvbuff);
}


//比较两字符串值，相同返回1，不同返回0
u8 samestr(u8 *str1,u8 *str2)
{
	u16 i=strlen((const char *)str1);
	while(i--)
	{
		if ((*str1)!=(*str2))
			return 0;
		str1++;str2++;
	}
	return 1;
}



void dbg_oche (u8 *databuff)
{
	char * ptxt=0;

	if (samestr((u8*)"on",databuff))
	{
		DBG_OCHE=1;
		ptxt="已开启回显\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
	}
	else if (samestr((u8*)"off",databuff))
	{
		DBG_OCHE=0;
		ptxt="已关闭回显\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	}
}




	//板子信息
void dbg_info (void)
{
	char * ptxt=0;
	char *txtbuff=mymalloc(512);
	extern u8 Gateway_IP[4];
	ptxt="温控集中器：";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
  sprintf(txtbuff,"%.6s-%02X%02X%02X\r\n","WK_JZQ",Phy_Addr[3],Phy_Addr[4],Phy_Addr[5]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	
	ptxt="本程序适配电路板版本：2018-10-22\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen(ptxt));
	
	sprintf(txtbuff,"本机IP地址：%d.%d.%d.%d\r\n",IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"本机MAC地址：%02X.%02X.%02X.%02X.%02X.%02X\r\n",Phy_Addr[0],Phy_Addr[1],Phy_Addr[2],Phy_Addr[3],Phy_Addr[4],Phy_Addr[5]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"服务器IP地址：%d.%d.%d.%d:%d\r\n",SERVER_IP[0],SERVER_IP[1],SERVER_IP[2],SERVER_IP[3],SERVER_PORT);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"网关IP地址：%d.%d.%d.%d\r\n",Gateway_IP[0],Gateway_IP[1],Gateway_IP[2],Gateway_IP[3]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	
	ptxt="网络连接状态：";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	if (DBG_INTER_STATE==0) ptxt="没有网络连接\r\n"; else if (DBG_INTER_STATE==1) ptxt="已连接上网关\r\n";
	else if (DBG_INTER_STATE==2) ptxt="已连接上服务器\r\n"; else ptxt="未知的网络状态\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	sprintf (txtbuff,"无线信道：%d\r\n",Get_MyChanel());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

ptxt="文件系统状态：";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	if (DBG_FATS==0) ptxt="不支持文件系统\r\n"; else if (DBG_FATS==1) ptxt="没有SD卡\r\n"; 
	else if (DBG_FATS==2) ptxt="SD卡挂载失败\r\n"; else if (DBG_FATS==3) ptxt="支持文件系统\r\n";
	else ptxt="未知状态\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));


	ptxt="外部FLASH：";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	if (SPI_FLASH_TYPE==0XEF13) ptxt="1MB\r\n"; else if (SPI_FLASH_TYPE==0XEF14) ptxt="2MB\r\n"; 
	else if (SPI_FLASH_TYPE==0XEF15) ptxt="4MB\r\n"; else if (SPI_FLASH_TYPE==0XEF16) ptxt="8MB\r\n";
	else if (SPI_FLASH_TYPE==0XEF17) ptxt="16MB\r\n"; else ptxt="未知的类型\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));


	sprintf (txtbuff,"系统内存使用情况：%dKB总共、%dKB已使用、%dKB剩余、使用了%d%%\r\n",
		memsize/1024,memsize*mem_perused()/100/1024,memsize*(100-mem_perused())/100/1024,mem_perused());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	
	sprintf(txtbuff,"集中器已运行 %d 秒\r\n",getSysRunTime());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	
	sprintf(txtbuff,"程序位置：-- %#X -- \r\n",SCB->VTOR);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"编译时间：%s ---- %s\r\n",__DATE__,__TIME__);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));


	myfree(txtbuff);
}


void dbg_err (u8 errtype)
{
	char * ptxt=0;
	ptxt="不支持的命令，输入\"help\"获取帮助。\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
}


//设备类型定义
const objname devicename[]=
{
	[0].objtype=0,[0].name=(u8*)"集中器",
	[1].objtype=1,[1].name=(u8*)"采集器",
	[2].objtype=2,[2].name=(u8*)"空调",
	[3].objtype=3,[3].name=(u8*)"除湿机",
	[4].objtype=4,[4].name=(u8*)"净化机",
	[5].objtype=5,[5].name=(u8*)"加湿机",
	[6].objtype=6,[6].name=(u8*)"一体机",
	[7].objtype=7,[7].name=(u8*)"不支持的类型",
};




//根据设备类型获取名称
u8 *dbg_getdevname(u8 devtype)
{
	if (devtype<7)
		return devicename[devtype].name;
	else
		return devicename[7].name;
}

//获取设备状态字符串最高位【0】表示开关状态，【1,2】位表示升降状态，【3】位表示离线状态,
//【4，5,6】代表重试次数，【7】，保留
void dbg_getdevstate(u8 state,char *txtbuff)
{
	if (state&0x08)
	{
		memcpy(txtbuff,"离线、",6);
		txtbuff+=6;
		txtbuff[0]=0;
		return ;//离线状态其他参数无意义
	}
	else
	{
		memcpy(txtbuff,"在线、",6);
		txtbuff+=6;
	}
	if (state&0x01)
	{
		memcpy(txtbuff,"设备开启、",10);
		txtbuff+=10;
	}
	else
	{
		memcpy(txtbuff,"设备关闭、",10);
		txtbuff+=10;
		txtbuff[0]=0;
		return ;
	}
	
	if (state&0x02)
	{
		memcpy(txtbuff,"参数升",6);
		txtbuff+=6;
	}
	else if (state&0x04)
	{
		memcpy(txtbuff,"参数减",6);
		txtbuff+=6;
	}
	else if (state&(0x06))
	{
		memcpy(txtbuff,"非常规状态",10);
		txtbuff+=10;
	}
	else
	{
	}
	txtbuff[0]=0;
}

//获取设备配置状态
void dbg_devconfig(void)
{
	u16 i=0;
	char * ptxt=0;
	char *txtbuff=mymalloc(512);
	char *txttemp=mymalloc(512);
	ptxt="设备配置信息如下：\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	for (i=0;EN_CONFIG[i*2];i++)
	{
		dbg_getdevstate(EN_CONFIG[i*2+1]>>8,txttemp);
		sprintf(txtbuff,"\t设备：%s\t地址：%-8d\t设备状态：%s\r\n",dbg_getdevname(EN_CONFIG[i*2+1]&0x00ff),EN_CONFIG[i*2],txttemp);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	}
	myfree(txttemp);
	myfree(txtbuff);
}

//显示帮助信息
void dbg_help(void)
{
	char * ptxt=0;
	char *txtbuff=mymalloc(512);
	ptxt="帮助：\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	ptxt="\t输入\"sysinfo\"获取板子信息\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	ptxt="\t输入\"devconfig\"获取设备配置信息\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	ptxt="\t输入\"reboot\"设备重启\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	ptxt="\t输入\"copy host on\"开始复制与上位机的数据交换到本端口\
			\n\t输入\"copy host on [端口号]\"开始复制与上位机的数据交换到指定的端口\r\n\t输入\"copy host off\"停止\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	ptxt="\t输入\"oche on\"开启回显\r\n\t输入\"oche off\"关闭回显\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t输入\"getip [域名]\"获取域名对应的IP地址\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t输入\"setchannel [信道]\"设置无线通信信道\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t输入\"mqtt\"连接到百度云\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	myfree(txtbuff);
}


void dbg_reboot (void)
{
	char * ptxt=0;
	ptxt="设备即将重启……\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	SysPowerOff();
}


void dbg_booting(void)
{
	char *txtbuff=mymalloc(512);
	sprintf (txtbuff,"编号为 %d 的温控集中器已启动……\r\n",Get_MyAddr());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	
	myfree(txtbuff);
	
}

//复制数据到调试端口
void dbg_copydata (u8 *buff)
{
	u16 port=0;
	char *txtbuff=mymalloc(512);
	if ( samestr((u8*)"host on",buff))
	{
		
		if (DBG_COPY_TO_S1CK)
		{
			sprintf (txtbuff,"复制已在进行中！！\r\n");
		}
		else
		{
			port=str2num(buff+8);
			if (port!=0)
			{
				if (tcp_connect(2,30,DBG_IP,port)==TRUE)
				{
					DBG_COPY_TO_S1CK=2;
					sprintf (txtbuff,"开始复制与主机交换的数据到端口 %d……\r\n",port);
				}
				else
				{
					tcp_close(2);
					sprintf (txtbuff,"请保证输入的端口号已打开为TCP Server!!!\r\n");
				}
			}
			else
			{
				DBG_COPY_TO_S1CK=1;
				sprintf (txtbuff,"开始复制与主机交换的数据到本端口……\r\n");
			}
		}
	}
	else if ( samestr((u8*)"host off",buff))
	{
		if (DBG_COPY_TO_S1CK==2)
		{
			tcp_close(2);
		}
		DBG_COPY_TO_S1CK=0;
		sprintf (txtbuff,"停止复制与主机交换的数据……\r\n");
	}
	else
	{
	}
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	myfree(txtbuff);
}



void dbg_setchanel (u8 *chars)
{ 
	char *txtbuff=mymalloc(512);
	if (Set_MyChanel(str2num(chars)))
	{
					//失败
		sprintf(txtbuff,"设置失败：输入的无线信道为 %d，不在0~31范围内……\r\n",str2num(chars));
	}
	else
	{
					//设置成功
		Save_Config();
		sprintf(txtbuff,"已设置无线信道为 %d，重启后生效……\r\n",Get_MyChanel());
	}
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	myfree(txtbuff);
}







u16 str2num(u8 *str)
{
	u16 ret=0;
	while(*str)
	{
		if((*str>='0')&&(*str<='9'))
		{
			ret*=10;
			ret+=*str-'0';
		}
		str++;
	}
	return ret;
}

			//提取字符串中的数字，用分隔符point分开,有length个数字
u8 getnumfstr(u8 *out,u8 *strin,u8 point,u8 length)
{
	u8 buff[10]={0};
	u8 i=0;
	while (length--)
	{
		if ((*strin!=point)&&(*strin!=0))
		{
			buff[i++]=*strin++;
		}
		else
		{
			buff[i]=0;
			*out++=str2num(buff);
			i=0;strin++;
			
		}
	}
}




void dbg_getip(u8 *buff)
{
	u8 getip[4]={0};
	if (DBG_COPY_TO_S1CK) 
	{
		udp_send(1,DBG_IP,DBG_PORT,(u8*)"请先关闭\"copy\"命令！！\r\n",22);
		return;
	}
	if (dns_query(2,buff,getip))
	{
		char *txt=mymalloc(200);
		sprintf (txt,"域名：%s 的IP地址是：%d.%d.%d.%d\r\n",buff,getip[0],getip[1],getip[2],getip[3]);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txt,strlen((const char *)txt));
	}
	else
	{
		udp_send(1,DBG_IP,DBG_PORT,(u8*)"获取IP地址失败T_T\r\n",17);
	}
	
}



void dbg_mqtt(u8 *buff)
{
	u8 ret=0;
	ret=mqtt_connect("rahher9.mqtt.iot.gz.baidubce.com",1883,"two","rahher9/two","KGa5JL87iGCheRFF");
	char *txt=mymalloc(512);
	sprintf(txt,"错误码为：%d\r\n",ret);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txt,strlen(txt));
	myfree(txt);
}



