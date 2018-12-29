

#ifndef __ENTERNET_H
#define __ENTERNET_H

#include "includes.h"
#include "dhcp.h"
#include "w5500.h"


typedef struct 
{
	u8 *myip;
	u8 *submask;
	u8 *getwayip;
	
}net_parameters;


u8 net_wailt_sended(u8 socket);


//ͨ��udp���緢�����ݣ����ṩĿ�ĵ�ip��ַ�Ͷ˿�
u8 udp_send(SOCKET s,u8 *ip,u16 com,u8 *databuff,u16 size);


u8 udp_init(SOCKET s,u16 mycom);


//ͨ��tcp����
u8 tcp_connect(SOCKET s,u16 mycom,u8 *ip,u16 com);


//ͨ��tcp��������
u8 tcp_send(SOCKET s,u8 *databuff,u16 size);

		//��������
u8 socket_recv(SOCKET s,u8 *databuff);


u8 tcp_close(SOCKET s);

u8 udp_close(SOCKET s);

void net_set_getway(u8 *getwayip);
void net_set_sub_mask(u8 *mask);
void net_set_myip(u8 *myip);
void net_set_disip(u8 socket,u8 *disip,u16 discom,u16 mycom);

				//���������ʼ��
void net_init(net_parameters *t);

				//���磬�豸����ʱ�ĳ�ʼ��
void net_sysboot_init(void);

				//��ȡ��������״̬
u8 net_get_phycstate(void);

//��ȡ�˿���������״̬
u8 net_get_comstate(u8 socket);


			//��������Ƿ����
u8 net_check_gateway(void);

//0û�г�ʼ���ã�1��ʼ������
u8 net_check_parameters(void);


					//���ڲ��Եģ�����������ڸı�����������
void net_test(void);


#endif















