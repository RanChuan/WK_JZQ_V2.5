

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


//通过udp网络发送数据，需提供目的地ip地址和端口
u8 udp_send(SOCKET s,u8 *ip,u16 com,u8 *databuff,u16 size);


u8 udp_init(SOCKET s,u16 mycom);


//通过tcp连接
u8 tcp_connect(SOCKET s,u16 mycom,u8 *ip,u16 com);


//通过tcp发送数据
u8 tcp_send(SOCKET s,u8 *databuff,u16 size);

		//接收数据
u8 socket_recv(SOCKET s,u8 *databuff);


u8 tcp_close(SOCKET s);

u8 udp_close(SOCKET s);

void net_set_getway(u8 *getwayip);
void net_set_sub_mask(u8 *mask);
void net_set_myip(u8 *myip);
void net_set_disip(u8 socket,u8 *disip,u16 discom,u16 mycom);

				//网络参数初始化
void net_init(net_parameters *t);

				//网络，设备启动时的初始化
void net_sysboot_init(void);

				//获取物理连接状态
u8 net_get_phycstate(void);

//获取端口网络连接状态
u8 net_get_comstate(u8 socket);


			//检查网关是否存在
u8 net_check_gateway(void);

//0没有初始化好，1初始化好了
u8 net_check_parameters(void);


					//用于测试的，在这个函数内改变各项网络参数
void net_test(void);


#endif















