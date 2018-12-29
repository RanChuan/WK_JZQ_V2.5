
#include "enternet.h"
#include "dhcp.h"
#include "w5500.h"


/***********************************************
*
*				�ļ�˵�������ļ���װ���������غ���
*				��ϸ˵�������ļ���װ�ĺ������ܰ�����ȡ����״̬
*									�򿪹ر�����ӿ�
*									���ͽ����������ݰ�
*									��Ҫ��������ĳ�ʼ��
*									һЩ���Ժ���
*				����ʱ�䣺2018.12.8
*
***********************************************/




			//�¿������ϴ���������ַ
u8 SERVER_IP[4]={0};
u16 SERVER_PORT=6000;







//ͨ��udp���緢�����ݣ����ṩĿ�ĵ�ip��ַ�Ͷ˿�
u8 udp_send(SOCKET s,u8 *ip,u16 com,u8 *databuff,u16 size)
{
		OS_ENTER_ONLYME();

	if((Read_W5500_SOCK_1Byte(s,Sn_MR)&0x0f) != SOCK_UDP)//���Socket��ʧ��
	{		
		Write_W5500_SOCK_4Byte(s, Sn_DIPR, ip);//����Ŀ������IP  		
		Write_W5500_SOCK_2Byte(s, Sn_DPORTR, com);//����Ŀ�������˿ں�				
	}
	
	Write_SOCK_Data_Buffer(s, databuff,size);		
		OS_EXIT_ONLYME();
}


u8 udp_init(SOCKET s,u16 mycom)
{
	u8 ret=FALSE;
	OS_ENTER_ONLYME();

	//���ö˿�0�Ķ˿ں�
	Write_W5500_SOCK_2Byte(s, Sn_PORT,mycom);

	Write_W5500_SOCK_1Byte(s,Sn_MR,MR_UDP);//����SocketΪUDPģʽ*/
	Write_W5500_SOCK_1Byte(s,Sn_CR,SN_OPEN);//��Socket*/
	Delay(5);//��ʱ5ms
	if(Read_W5500_SOCK_1Byte(s,Sn_SR)!=SOCK_UDP)//���Socket��ʧ��
	{
		Write_W5500_SOCK_1Byte(s,Sn_CR,SN_CLOSE);//�򿪲��ɹ�,�ر�Socket
		ret= FALSE;//����FALSE(0x00)
	}
	else
		ret= TRUE;
	OS_EXIT_ONLYME();
	return ret;
}


//ͨ��tcp����
u8 tcp_connect(SOCKET s,u16 mycom,u8 *ip,u16 com)
{ 
	
	if (!(ip[0]||ip[1]||ip[2]||ip[3]))
	{
		return FALSE;//ip��ַ���Ϸ���ʧ��
	}
	
	OS_ENTER_ONLYME();
	
	//���ö˿�0�Ķ˿ں�
	Write_W5500_SOCK_2Byte(s, Sn_PORT,mycom);
	//���ö˿�0Ŀ��(Զ��)�˿ں�
	Write_W5500_SOCK_2Byte(s, Sn_DPORTR, com);
	//���ö˿�0Ŀ��(Զ��)IP��ַ
	Write_W5500_SOCK_4Byte(s, Sn_DIPR, ip);			
 
	Write_W5500_SOCK_1Byte(s,Sn_MR,MR_TCP);//����socketΪTCPģʽ
	Write_W5500_SOCK_1Byte(s,Sn_CR,SN_OPEN);//��Socket
	u16 i=20;
	do
	{
		Delay(5);//��ʱ5ms
		if(Read_W5500_SOCK_1Byte(s,Sn_SR)==SOCK_INIT)//���socket�򿪳ɹ�
		{  
			break;
		}
	}while(--i);
	if (i==0)
	{
		OS_EXIT_ONLYME();
		return FALSE;
	}
	Write_W5500_SOCK_1Byte(s,Sn_CR,SN_CONNECT);//����SocketΪConnectģʽ
	i=2000;
	do
	{
		delay_us(500);//��ʱ5ms
		if(Read_W5500_SOCK_1Byte(s,Sn_SR)==SOCK_ESTABLISHED)//���socket���ӳɹ�
		{  
			break;
		}
	}while(--i);
	OS_EXIT_ONLYME();
	if (i==0)
	{
		return FALSE;
	}
	return TRUE;//����TRUE,���óɹ�
	
}


//ͨ��tcp��������
u8 tcp_send(SOCKET s,u8 *databuff,u16 size)
{
	OS_ENTER_ONLYME();
	Write_SOCK_Data_Buffer(s, databuff,size);
	OS_EXIT_ONLYME();
}

//�Ͽ�tcp����
u8 tcp_close(SOCKET s)
{
	OS_ENTER_ONLYME();
	Write_W5500_SOCK_1Byte(s,Sn_CR,SN_DISCON);
	OS_EXIT_ONLYME();
	return TRUE;//����TRUE,���óɹ�

}

u8 udp_close(SOCKET s)
{
	OS_ENTER_ONLYME();
	
	Write_W5500_SOCK_1Byte(s,Sn_CR,SN_CLOSE);//����SocketΪConnectģʽ
	u8 i=20;
	do
	{
		Delay(5);//��ʱ5ms
		if(Read_W5500_SOCK_1Byte(s,Sn_SR)==SOCK_CLOSED)//���socket�򿪳ɹ�
		{  
			break;
		}
	}while(--i);
	OS_EXIT_ONLYME();
	
	if (i==0)
	{
		return FALSE;
	}
	return TRUE;//����TRUE,���óɹ�

}



		//��������
u8 socket_recv(SOCKET s,u8 *databuff)
{
	OS_ENTER_ONLYME();
	Read_SOCK_Data_Buffer(0,databuff);
	OS_EXIT_ONLYME();
	return 0;
}




			//�ȴ�������� 
u8 net_wailt_sended(u8 socket)
{ 
	extern u8 NET_S0_STATE;
	extern u8 NET_S1_STATE;
	if (socket==0)
	{
		while(!(NET_S0_STATE&IR_RECV))
		{
			delay_ms(100);
			
		}
		return TRUE;
	}
	else if (socket==1)
	{
		while(!(NET_S1_STATE&IR_RECV))
		{
			delay_ms(100);
		}
		return TRUE;
	}
	return FALSE;
}






//��ȡ�˿���������״̬
u8 net_get_comstate(u8 socket)
{
	return Read_W5500_SOCK_1Byte(socket,Sn_SR);
}



		//��������ip
void net_set_getway(u8 *getwayip)
{
	Write_W5500_nByte(GAR, getwayip, 4);
}

		//������������
void net_set_sub_mask(u8 *mask)
{
	Write_W5500_nByte(SUBR,mask,4);	
}

		//���ñ���IP��ַ
void net_set_myip(u8 *myip)
{
	Write_W5500_nByte(SIPR,myip,4);	
}

	//����Ŀ�ĵ�IP��ַ���˿�
void net_set_disip(u8 socket,u8 *disip,u16 discom,u16 mycom)
{
	
	Write_W5500_SOCK_2Byte(socket, Sn_MSSR, 1460);//����Ƭ�ֽ���=1460(0x5b4)

	//���ö˿�0�Ķ˿ں�
	Write_W5500_SOCK_2Byte(socket, Sn_PORT, mycom);
	//���ö˿�0Ŀ��(Զ��)�˿ں�
	Write_W5500_SOCK_2Byte(socket, Sn_DPORTR, discom);
	//���ö˿�0Ŀ��(Զ��)IP��ַ
	Write_W5500_SOCK_4Byte(socket, Sn_DIPR, disip);			
	
}



void net_init(net_parameters *t)
{
	net_set_getway(t->getwayip);
	net_set_sub_mask(t->submask);
	net_set_myip(t->myip);
	
}

void net_sysboot_init(void)
{	
	net_parameters t;
	extern u8 Sub_Mask[4];
	extern u8 IP_Addr[4];
	extern u8 Gateway_IP[4];
	t.getwayip=Gateway_IP;
	t.myip=IP_Addr;
	t.submask=Sub_Mask;
	net_init(&t);
}

			//����1�������ӣ�0��δ����
u8 net_get_phycstate(void)
{
//	while((Read_W5500_1Byte(PHYCFGR)&LINK)==0)
//	{
//		Delay(200);
//		
//	}//�ȴ���̫���������
//	return 1;
	return Read_W5500_1Byte(PHYCFGR)&LINK;
}



			//��������Ƿ����
u8 net_check_gateway(void)
{
	unsigned char ip_adde[4];
	extern u8 IP_Addr[4];
	ip_adde[0]=IP_Addr[0]+1;
	ip_adde[1]=IP_Addr[1]+1;
	ip_adde[2]=IP_Addr[2]+1;
	ip_adde[3]=IP_Addr[3]+1;

	//������ؼ���ȡ���ص������ַ
	Write_W5500_SOCK_4Byte(0,Sn_DIPR,ip_adde);//��Ŀ�ĵ�ַ�Ĵ���д���뱾��IP��ͬ��IPֵ
	Write_W5500_SOCK_1Byte(0,Sn_MR,MR_TCP);//����socketΪTCPģʽ
	Write_W5500_SOCK_1Byte(0,Sn_CR,SN_OPEN);//��Socket	 
	Delay(5);//��ʱ5ms 	
	
	if(Read_W5500_SOCK_1Byte(0,Sn_SR) != SOCK_INIT)//���socket��ʧ��
	{
		Write_W5500_SOCK_1Byte(0,Sn_CR,SN_CLOSE);//�򿪲��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}

	Write_W5500_SOCK_1Byte(0,Sn_CR,SN_CONNECT);//����SocketΪConnectģʽ						
	u8 i=255;
	do
	{
		u8 j=0;
		j=Read_W5500_SOCK_1Byte(0,Sn_IR);//��ȡSocket0�жϱ�־�Ĵ���
		if(j!=0)
		Write_W5500_SOCK_1Byte(0,Sn_IR,j);
		Delay(5);//��ʱ5ms 
		if((j&IR_TIMEOUT) == IR_TIMEOUT)
		{
			return FALSE;	
		}
		else if(Read_W5500_SOCK_1Byte(0,Sn_DHAR) != 0xff)
		{
			Write_W5500_SOCK_1Byte(0,Sn_CR,SN_CLOSE);//�ر�Socket
			return TRUE;							
		}
	}while(i--);
	return FALSE;
}




//0û�г�ʼ���ã�1��ʼ������
u8 net_check_parameters(void)
{
	extern u8 Sub_Mask[4];
	extern u8 IP_Addr[4];
	extern u8 Gateway_IP[4];
	
	if (!(Sub_Mask[0]||Sub_Mask[1]||Sub_Mask[2]||Sub_Mask[3]) )
	{
		return 0;
	}
	if (!(IP_Addr[0]||IP_Addr[1]||IP_Addr[2]||IP_Addr[3]) )
	{
		return 0;
	}
	if (!(Gateway_IP[0]||Gateway_IP[1]||Gateway_IP[2]||Gateway_IP[3]) )
	{
		return 0;
	}
	
	return 1;
}









		//���ڲ��Եģ�����������ڸı�����������
		//�����������֮���ٵ���net_sysboot_init()
void net_test(void)
{
	extern u8 Sub_Mask[4];
	extern u8 IP_Addr[4];
	extern u8 Gateway_IP[4];
	extern u8 SERVER_IP[4];
	u16 *Get_MyIP(void);
	u16 *ipconfig=Get_MyIP();
	Sub_Mask[0]=255;
	Sub_Mask[1]=255;
	Sub_Mask[2]=255;
	Sub_Mask[3]=0;
	
	IP_Addr[0]=*ipconfig++;
	IP_Addr[1]=*ipconfig++;
	IP_Addr[2]=*ipconfig++;
	IP_Addr[3]=*ipconfig++;
	
	ipconfig++;		//�����˿ں�
	ipconfig++;
	
	
	SERVER_IP[0]=*ipconfig++;
	SERVER_IP[1]=*ipconfig++;
	SERVER_IP[2]=*ipconfig++;
	SERVER_IP[3]=*ipconfig++;
	
	SERVER_PORT=0;
	SERVER_PORT=(*ipconfig++)<<8;
	SERVER_PORT|=(*ipconfig++)&0x00ff;
	
	Gateway_IP[0]=*ipconfig++;
	Gateway_IP[1]=*ipconfig++;
	Gateway_IP[2]=*ipconfig++;
	Gateway_IP[3]=*ipconfig++;
}












