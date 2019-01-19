
#include "my_debug.h"
#include "string.h"
#include "stdio.h"
#include "data_map.h"//��ȡ���ò���
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
	if (net_state_old&IR_RECV)//��������ݵȴ�����
	{
		net_state_old&=~IR_RECV;
		dbg_Interpreter();//���������
	}
	NET_S1_STATE=0;
}

/*****************************************************

						��������������ʹ����ڴ��������ִ��
						help����ʾ������Ϣ
						sysinfo����ʾϵͳ��Ϣ
						setip�������������


******************************************************/

u8 DBG_OCHE =0;//����
u8 DBG_IP[4]={255,255,255,255};//�����õ�Ŀ��ip��ַ
u16 DBG_PORT=7000;//�����õĶ˿ں�
void dbg_Interpreter(void)
{
	u8 *recvbuff=mymalloc(2048);
	memset(recvbuff,0,2048);//����ڴ��е�����
	Read_SOCK_Data_Buffer(1, recvbuff);
	
	//�������õ�������Ŀ���ַ
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


//�Ƚ����ַ���ֵ����ͬ����1����ͬ����0
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
		ptxt="�ѿ�������\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
		
	}
	else if (samestr((u8*)"off",databuff))
	{
		DBG_OCHE=0;
		ptxt="�ѹرջ���\r\n";
		udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	}
}




	//������Ϣ
void dbg_info (void)
{
	char * ptxt=0;
	char *txtbuff=mymalloc(512);
	extern u8 Gateway_IP[4];
	ptxt="�¿ؼ�������";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
  sprintf(txtbuff,"%.6s-%02X%02X%02X\r\n","WK_JZQ",Phy_Addr[3],Phy_Addr[4],Phy_Addr[5]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	
	ptxt="�����������·��汾��2018-10-22\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen(ptxt));
	
	sprintf(txtbuff,"����IP��ַ��%d.%d.%d.%d\r\n",IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"����MAC��ַ��%02X.%02X.%02X.%02X.%02X.%02X\r\n",Phy_Addr[0],Phy_Addr[1],Phy_Addr[2],Phy_Addr[3],Phy_Addr[4],Phy_Addr[5]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"������IP��ַ��%d.%d.%d.%d:%d\r\n",SERVER_IP[0],SERVER_IP[1],SERVER_IP[2],SERVER_IP[3],SERVER_PORT);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"����IP��ַ��%d.%d.%d.%d\r\n",Gateway_IP[0],Gateway_IP[1],Gateway_IP[2],Gateway_IP[3]);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	
	ptxt="��������״̬��";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	if (DBG_INTER_STATE==0) ptxt="û����������\r\n"; else if (DBG_INTER_STATE==1) ptxt="������������\r\n";
	else if (DBG_INTER_STATE==2) ptxt="�������Ϸ�����\r\n"; else ptxt="δ֪������״̬\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	sprintf (txtbuff,"�����ŵ���%d\r\n",Get_MyChanel());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

ptxt="�ļ�ϵͳ״̬��";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	if (DBG_FATS==0) ptxt="��֧���ļ�ϵͳ\r\n"; else if (DBG_FATS==1) ptxt="û��SD��\r\n"; 
	else if (DBG_FATS==2) ptxt="SD������ʧ��\r\n"; else if (DBG_FATS==3) ptxt="֧���ļ�ϵͳ\r\n";
	else ptxt="δ֪״̬\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));


	ptxt="�ⲿFLASH��";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	if (SPI_FLASH_TYPE==0XEF13) ptxt="1MB\r\n"; else if (SPI_FLASH_TYPE==0XEF14) ptxt="2MB\r\n"; 
	else if (SPI_FLASH_TYPE==0XEF15) ptxt="4MB\r\n"; else if (SPI_FLASH_TYPE==0XEF16) ptxt="8MB\r\n";
	else if (SPI_FLASH_TYPE==0XEF17) ptxt="16MB\r\n"; else ptxt="δ֪������\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));


	sprintf (txtbuff,"ϵͳ�ڴ�ʹ�������%dKB�ܹ���%dKB��ʹ�á�%dKBʣ�ࡢʹ����%d%%\r\n",
		memsize/1024,memsize*mem_perused()/100/1024,memsize*(100-mem_perused())/100/1024,mem_perused());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	
	sprintf(txtbuff,"������������ %d ��\r\n",getSysRunTime());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));
	
	sprintf(txtbuff,"����λ�ã�-- %#X -- \r\n",SCB->VTOR);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));

	sprintf(txtbuff,"����ʱ�䣺%s ---- %s\r\n",__DATE__,__TIME__);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen(txtbuff));


	myfree(txtbuff);
}


void dbg_err (u8 errtype)
{
	char * ptxt=0;
	ptxt="��֧�ֵ��������\"help\"��ȡ������\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
}


//�豸���Ͷ���
const objname devicename[]=
{
	[0].objtype=0,[0].name=(u8*)"������",
	[1].objtype=1,[1].name=(u8*)"�ɼ���",
	[2].objtype=2,[2].name=(u8*)"�յ�",
	[3].objtype=3,[3].name=(u8*)"��ʪ��",
	[4].objtype=4,[4].name=(u8*)"������",
	[5].objtype=5,[5].name=(u8*)"��ʪ��",
	[6].objtype=6,[6].name=(u8*)"һ���",
	[7].objtype=7,[7].name=(u8*)"��֧�ֵ�����",
};




//�����豸���ͻ�ȡ����
u8 *dbg_getdevname(u8 devtype)
{
	if (devtype<7)
		return devicename[devtype].name;
	else
		return devicename[7].name;
}

//��ȡ�豸״̬�ַ������λ��0����ʾ����״̬����1,2��λ��ʾ����״̬����3��λ��ʾ����״̬,
//��4��5,6���������Դ�������7��������
void dbg_getdevstate(u8 state,char *txtbuff)
{
	if (state&0x08)
	{
		memcpy(txtbuff,"���ߡ�",6);
		txtbuff+=6;
		txtbuff[0]=0;
		return ;//����״̬��������������
	}
	else
	{
		memcpy(txtbuff,"���ߡ�",6);
		txtbuff+=6;
	}
	if (state&0x01)
	{
		memcpy(txtbuff,"�豸������",10);
		txtbuff+=10;
	}
	else
	{
		memcpy(txtbuff,"�豸�رա�",10);
		txtbuff+=10;
		txtbuff[0]=0;
		return ;
	}
	
	if (state&0x02)
	{
		memcpy(txtbuff,"������",6);
		txtbuff+=6;
	}
	else if (state&0x04)
	{
		memcpy(txtbuff,"������",6);
		txtbuff+=6;
	}
	else if (state&(0x06))
	{
		memcpy(txtbuff,"�ǳ���״̬",10);
		txtbuff+=10;
	}
	else
	{
	}
	txtbuff[0]=0;
}

//��ȡ�豸����״̬
void dbg_devconfig(void)
{
	u16 i=0;
	char * ptxt=0;
	char *txtbuff=mymalloc(512);
	char *txttemp=mymalloc(512);
	ptxt="�豸������Ϣ���£�\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	
	for (i=0;EN_CONFIG[i*2];i++)
	{
		dbg_getdevstate(EN_CONFIG[i*2+1]>>8,txttemp);
		sprintf(txtbuff,"\t�豸��%s\t��ַ��%-8d\t�豸״̬��%s\r\n",dbg_getdevname(EN_CONFIG[i*2+1]&0x00ff),EN_CONFIG[i*2],txttemp);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	}
	myfree(txttemp);
	myfree(txtbuff);
}

//��ʾ������Ϣ
void dbg_help(void)
{
	char * ptxt=0;
	char *txtbuff=mymalloc(512);
	ptxt="������\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	ptxt="\t����\"sysinfo\"��ȡ������Ϣ\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	ptxt="\t����\"devconfig\"��ȡ�豸������Ϣ\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	ptxt="\t����\"reboot\"�豸����\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	ptxt="\t����\"copy host on\"��ʼ��������λ�������ݽ��������˿�\
			\n\t����\"copy host on [�˿ں�]\"��ʼ��������λ�������ݽ�����ָ���Ķ˿�\r\n\t����\"copy host off\"ֹͣ\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));
	ptxt="\t����\"oche on\"��������\r\n\t����\"oche off\"�رջ���\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t����\"getip [����]\"��ȡ������Ӧ��IP��ַ\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t����\"setchannel [�ŵ�]\"��������ͨ���ŵ�\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	ptxt="\t����\"mqtt\"���ӵ��ٶ���\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	myfree(txtbuff);
}


void dbg_reboot (void)
{
	char * ptxt=0;
	ptxt="�豸������������\r\n";
	udp_send(1,DBG_IP,DBG_PORT,(u8*)ptxt,strlen((const char *)ptxt));

	SysPowerOff();
}


void dbg_booting(void)
{
	char *txtbuff=mymalloc(512);
	sprintf (txtbuff,"���Ϊ %d ���¿ؼ���������������\r\n",Get_MyAddr());
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txtbuff,strlen((const char *)txtbuff));
	
	myfree(txtbuff);
	
}

//�������ݵ����Զ˿�
void dbg_copydata (u8 *buff)
{
	u16 port=0;
	char *txtbuff=mymalloc(512);
	if ( samestr((u8*)"host on",buff))
	{
		
		if (DBG_COPY_TO_S1CK)
		{
			sprintf (txtbuff,"�������ڽ����У���\r\n");
		}
		else
		{
			port=str2num(buff+8);
			if (port!=0)
			{
				if (tcp_connect(2,30,DBG_IP,port)==TRUE)
				{
					DBG_COPY_TO_S1CK=2;
					sprintf (txtbuff,"��ʼ�������������������ݵ��˿� %d����\r\n",port);
				}
				else
				{
					tcp_close(2);
					sprintf (txtbuff,"�뱣֤����Ķ˿ں��Ѵ�ΪTCP Server!!!\r\n");
				}
			}
			else
			{
				DBG_COPY_TO_S1CK=1;
				sprintf (txtbuff,"��ʼ�������������������ݵ����˿ڡ���\r\n");
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
		sprintf (txtbuff,"ֹͣ�������������������ݡ���\r\n");
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
					//ʧ��
		sprintf(txtbuff,"����ʧ�ܣ�����������ŵ�Ϊ %d������0~31��Χ�ڡ���\r\n",str2num(chars));
	}
	else
	{
					//���óɹ�
		Save_Config();
		sprintf(txtbuff,"�����������ŵ�Ϊ %d����������Ч����\r\n",Get_MyChanel());
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

			//��ȡ�ַ����е����֣��÷ָ���point�ֿ�,��length������
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
		udp_send(1,DBG_IP,DBG_PORT,(u8*)"���ȹر�\"copy\"�����\r\n",22);
		return;
	}
	if (dns_query(2,buff,getip))
	{
		char *txt=mymalloc(200);
		sprintf (txt,"������%s ��IP��ַ�ǣ�%d.%d.%d.%d\r\n",buff,getip[0],getip[1],getip[2],getip[3]);
		udp_send(1,DBG_IP,DBG_PORT,(u8*)txt,strlen((const char *)txt));
	}
	else
	{
		udp_send(1,DBG_IP,DBG_PORT,(u8*)"��ȡIP��ַʧ��T_T\r\n",17);
	}
	
}



void dbg_mqtt(u8 *buff)
{
	u8 ret=0;
	ret=mqtt_connect("rahher9.mqtt.iot.gz.baidubce.com",1883,"two","rahher9/two","KGa5JL87iGCheRFF");
	char *txt=mymalloc(512);
	sprintf(txt,"������Ϊ��%d\r\n",ret);
	udp_send(1,DBG_IP,DBG_PORT,(u8*)txt,strlen(txt));
	myfree(txt);
}



