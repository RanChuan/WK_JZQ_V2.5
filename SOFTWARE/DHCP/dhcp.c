

#include "dhcp.h"
#include "w5500.h"
#include "includes.h"
#include "enternet.h"
#include "my_debug.h"


/***********************************************
*
*				�ļ�˵�������ļ���װDHCP��غ���
*				��ϸ˵����ͨ������dhcp_retry()������ȡ�������
*									��dhcp_retry()��������ִ�еĹ����в�Ҫ��������������صĺ���
*									�����ȡip��ַʧ��
*									��ȡ�������������䵽ȫ�ֱ���Sub_Mask��IP_Addr��Gateway_IP��
*									��������1�ɹ�,����0ʧ��
*				����ʱ�䣺2018.12.8
*
***********************************************/









#define ZERO (*(u32*)0x20000000)

u32 Transaction=4561235;



#define CMD_NUM (MAC+Transaction++)

			//��������
u8 Sub_Mask[4]={0};
u8 IP_Addr[4]={0};
u8 Gateway_IP[4]={0};
extern u8 DNS_SERVER[4];












//����ip��ַ
void dhcp_discover(dhcp_pack * discover)
{ 
	Transaction+=*((u32*)MAC+2);//��MAC��֤ÿ���������ı���Ψһ
	u8 serverIP[]={255,255,255,255};
	u8 noneIP[]={0,0,0,0};
	mymemset(discover,0,sizeof(dhcp_pack)+320);
	discover->OP=1;
	discover->Htype=1;
	discover->Hlen=6;
	discover->Hops=0; 
	discover->Transaction=ZERO+Transaction++;ZERO=discover->Transaction;
	discover->Seconds=0;
	discover->Flags=0x0080;//��С��ת��
	mymemcpy(discover->ClientMAC,Phy_Addr,6);
	discover->MagicCookie=0x63538263;//���˴�С��ת��


	u16 i=0;
  /* Option Request Param. */
  discover->Options[i++] = dhcpMessageType; 
  discover->Options[i++] = 0x01; 
  discover->Options[i++] = DHCP_DISCOVER;
  
  // Client identifier
  discover->Options[i++] = dhcpClientIdentifier;
  discover->Options[i++] = 0x07;
  discover->Options[i++] = 0x01;
  discover->Options[i++] = Phy_Addr[0];
  discover->Options[i++] = Phy_Addr[1];
  discover->Options[i++] = Phy_Addr[2];
  discover->Options[i++] = Phy_Addr[3];
  discover->Options[i++] = Phy_Addr[4];
  discover->Options[i++] = Phy_Addr[5];
  
  // host name
  discover->Options[i++] = hostName;
	
  // set the host name
	char *host_name=mymalloc(64);
  sprintf((char*)host_name,"%.6s-%02X%02X%02X","WK_JZQ",Phy_Addr[3],Phy_Addr[4],Phy_Addr[5]);
  discover->Options[i++] = (u8)strlen((char*)host_name);
	strcpy((char*)(&(discover->Options[i])),(char*)host_name);
	myfree(host_name);
  
  i+=(u16)strlen((char*)host_name);	
 
  discover->Options[i++] = dhcpParamRequest;
  discover->Options[i++] = 0x06;
  discover->Options[i++] = subnetMask;
  discover->Options[i++] = routersOnSubnet;
  discover->Options[i++] = dns;
  discover->Options[i++] = domainName;
  discover->Options[i++] = dhcpT1value;
  discover->Options[i++] = dhcpT2value;
  discover->Options[i++] = endOption;






	
	//���������ַ
	Write_W5500_nByte(SHAR,Phy_Addr,6);	
	//���ñ���IP��ַ
	Write_W5500_nByte(SIPR,noneIP,4);		

	//���ö˿�0�Ķ˿ں�
	Write_W5500_SOCK_2Byte(0, Sn_PORT, 68);
	//���ö˿�0Ŀ��(Զ��)�˿ں�
	Write_W5500_SOCK_2Byte(0, Sn_DPORTR, 67);
	//���ö˿�0Ŀ��(Զ��)IP��ַΪ255
	Write_W5500_SOCK_4Byte(0, Sn_DIPR, serverIP);		
		//�������ص�ַΪ0
	Write_W5500_nByte(GAR, noneIP, 4);
	//������������Ϊ0
	Write_W5500_nByte(SUBR,Sub_Mask,4);		

	//����ΪUDPģʽ
	if (TRUE==Socket_UDP(0))
	{
		Write_SOCK_Data_Buffer(0,(u8*)discover,sizeof(dhcp_pack)+320);
	}
	
}



u8 dhcp_request(dhcp_pack * discover,dhcp_pack * recvdata)
{
	if (recvdata->Transaction!=discover->Transaction)
	{
		return 0;
	}
	
	mymemcpy(IP_Addr,recvdata->YourIP,4);//��ȡ�Լ���ip��ַ
	
	u8 * options=recvdata->Options;
	while(options)
	{
		if (options[0]==subnetMask)
		{
			mymemcpy(Sub_Mask,&options[2],4);//��ȡ��������
			break;
		}
		options=nest_Options(options);
	}
	
	options=recvdata->Options;
	while(options)
	{
		if (options[0]==dns)
		{
			mymemcpy(DNS_SERVER,&options[2],4);//��ȡDNS������ip
			break;
		}
		options=nest_Options(options);
	}

	options=recvdata->Options;
	while(options)
	{
		if (options[0]==routersOnSubnet)
		{
			mymemcpy(Gateway_IP,&options[2],4);//��ȡ����ip
			break;
		}
		options=nest_Options(options);
	}


	
	u16 i=0;
  /* Option Request Param. */
  discover->Options[i++] = dhcpMessageType; 
  discover->Options[i++] = 0x01; 
  discover->Options[i++] = DHCP_REQUEST;
  
  // Client identifier
  discover->Options[i++] = dhcpClientIdentifier;
  discover->Options[i++] = 0x07;
  discover->Options[i++] = 0x01;
  discover->Options[i++] = Phy_Addr[0];
  discover->Options[i++] = Phy_Addr[1];
  discover->Options[i++] = Phy_Addr[2];
  discover->Options[i++] = Phy_Addr[3];
  discover->Options[i++] = Phy_Addr[4];
  discover->Options[i++] = Phy_Addr[5];
 

  // Client �����õ�IP��ַ
  discover->Options[i++] = dhcpRequestedIPaddr;
  discover->Options[i++] = 0x04;
  discover->Options[i++] = IP_Addr[0];
  discover->Options[i++] = IP_Addr[1];
  discover->Options[i++] = IP_Addr[2];
  discover->Options[i++] = IP_Addr[3];

  // Client DHCP��������IP��ַ
  discover->Options[i++] = dhcpServerIdentifier;
  discover->Options[i++] = 0x04;
  discover->Options[i++] = Gateway_IP[0];
  discover->Options[i++] = Gateway_IP[1];
  discover->Options[i++] = Gateway_IP[2];
  discover->Options[i++] = Gateway_IP[3];


  // host name
  discover->Options[i++] = hostName;
	
  // set the host name
	char *host_name=mymalloc(64);
  sprintf((char*)host_name,"%.6s-%02X%02X%02X","WK_JZQ",Phy_Addr[3],Phy_Addr[4],Phy_Addr[5]);
  discover->Options[i++] = (u8)strlen((char*)host_name);
	strcpy((char*)(&(discover->Options[i])),(char*)host_name);
	myfree(host_name);
  
  i+=(u16)strlen((char*)host_name);	
 
  discover->Options[i++] = dhcpParamRequest;
  discover->Options[i++] = 0x08;
  discover->Options[i++] = subnetMask;
  discover->Options[i++] = routersOnSubnet;
  discover->Options[i++] = dns;
  discover->Options[i++] = domainName;
  discover->Options[i++] = dhcpT1value;
  discover->Options[i++] = dhcpT2value;
  discover->Options[i++] = performRouterDiscovery;
  discover->Options[i++] = staticRoute;
  discover->Options[i++] = endOption;


	Write_SOCK_Data_Buffer(0,(u8*)discover,sizeof(dhcp_pack)+320);
	
	return 1;
}



//����յ���dhcp��������
u8 dhcp_ask(dhcp_pack * discover,dhcp_pack * recvdata)
{
	u8 ret=0;
	if (recvdata->Transaction!=discover->Transaction)
	{
		return 0;
	}
	u8 * options=recvdata->Options;
	while(options)
	{
		if (options[0]==dhcpMessageType)
		{
			if (options[2]==DHCP_ACK)//ip��ַ����ɹ�
			{
				net_sysboot_init();
				ret=udp_close(0);
				Write_SOCK_Data_Buffer(1,(u8*)"�������ӳɹ�����\r\n",18);
				return ret;
			}
			else if (options[2]==DHCP_NAK)//ip��ַ����ʧ��
			{
				udp_init(1,55);
				Write_SOCK_Data_Buffer(1,(u8*)"��������ʧ�ܡ���\r\n",18);
				return 0;
			}
			break;
		}
		options=nest_Options(options);
	}
	return 0;
}



		//dhcp����
u8 dhcp_retry(void)
{
	extern u8 NET_S0_STATE;
	dhcp_pack * discover=0;
	discover=mymalloc(sizeof(dhcp_pack)+320);
	if (discover==0) return 0;
	u8 * recvdata=0;
	recvdata=mymalloc(2048);//���ڽ�������ֻ����2048
	if (recvdata==0) 
	{
		myfree(discover);
		return 0;
	} 
	
	u8 ret=0;
	u8 sir=0;
	u8 i=200;
				//����IP��ַ��ȡ����
	dhcp_discover(discover);
	do
	{
		delay_ms(4); 
		if (NET_S0_STATE&IR_RECV)//��������ݵȴ�����
		{
			NET_S0_STATE&=~IR_RECV;
			Read_SOCK_Data_Buffer(0,(u8*)recvdata);
			ret=dhcp_request(discover,(dhcp_pack *)(recvdata+8));
			if (ret)
				break;
		}
	}
	while(--i);
	if (i==0) 
	{
		myfree(discover);
		myfree(recvdata);
		return 0;
	}
				
	i=200;
	while(--i)
	{
		delay_ms(4); 
		if (NET_S0_STATE&IR_RECV)//��������ݵȴ�����
		{
			NET_S0_STATE&=~IR_RECV;
			mymemset(recvdata,0,sizeof(dhcp_pack)+320);
			Read_SOCK_Data_Buffer(0,(u8*)recvdata);
			ret=dhcp_ask(discover,(dhcp_pack *)(recvdata+8));
			if (ret)
			  break;
		}
	}
	if (i==0) 
	{
		myfree(discover);
		myfree(recvdata);
		return 0;
	}
	
	
	
	myfree(discover);
	myfree(recvdata);
	return ret;
}

//��ȡoption����
u8 * nest_Options(u8 *options)
{
	if (options[0]==0xff) return 0;
	u8 len=options[1];//��ȡ����
	options+=len+2;
	return options;
}








