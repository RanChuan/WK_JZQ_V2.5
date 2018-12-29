
#ifndef __DHCP_H
#define __DHCP_H

#include "includes.h"



//dhcp使用的报文结构体，
typedef __packed struct
{
	u8  OP;//方向，1从机到主机，2主机到从机
	u8  Htype;//硬件类型1
	u8  Hlen;//硬件长度6
	u8  Hops;//每经过一个中转加一
	u32 Transaction;//一个事物随机数
	u16 Seconds;//：由用户指定的时间，指开始地址获取和更新进行后的时间；
	u16 Flags;//：从0-15bits，最左一bit为1时表示server将以广播方式传送封包给 client，其余尚未使用；
	u8  ClientIP[4];//：用户IP地址；
	u8  YourIP[4];//：服务器分配给客户的IP地址；
	u8  ServerIP[4];//：用于bootstrap过程中的IP地址；（服务器的IP地址）
	u8  GetwayIP[4];//：转发代理（网关）IP地址；
	u8  ClientMAC[16];//：client的硬件地址；使用前6个字节
	u8  ServerName[64];//：可选server的名称，以0x00结尾；
	u8  BootFile[128];//：启动文件名；
	u32 MagicCookie;//0x63825363    DHCP
	u8  Options[];//：，厂商标识，可选的参数字段	
}dhcp_pack;




/* DHCP message type */
#define  DHCP_DISCOVER           1
#define  DHCP_OFFER              2
#define  DHCP_REQUEST            3
#define  DHCP_DECLINE            4
#define  DHCP_ACK                5
#define  DHCP_NAK                6
#define  DHCP_RELEASE            7
#define  DHCP_INFORM             8

#define DHCP_HTYPE10MB           1
#define DHCP_HTYPE100MB          2

#define DHCP_HLENETHERNET        6
#define DHCP_HOPS                0
#define DHCP_SECS                0

#define MAGIC_COOKIE		         0x63825363
#define DEFAULT_XID              0x12345678

#define DEFAULT_LEASETIME        0xffffffff	/* infinite lease time */










/* DHCP option and value (cf. RFC1533) */
enum
{
   padOption               = 0,
   subnetMask              = 1,
   timerOffset             = 2,
   routersOnSubnet         = 3,
   timeServer              = 4,
   nameServer              = 5,
   dns                     = 6,
   logServer               = 7,
   cookieServer            = 8,
   lprServer               = 9,
   impressServer           = 10,
   resourceLocationServer  = 11,
   hostName                = 12,
   bootFileSize            = 13,
   meritDumpFile           = 14,
   domainName              = 15,
   swapServer              = 16,
   rootPath                = 17,
   extentionsPath          = 18,
   IPforwarding            = 19,
   nonLocalSourceRouting   = 20,
   policyFilter            = 21,
   maxDgramReasmSize       = 22,
   defaultIPTTL            = 23,
   pathMTUagingTimeout     = 24,
   pathMTUplateauTable     = 25,
   ifMTU                   = 26,
   allSubnetsLocal         = 27,
   broadcastAddr           = 28,
   performMaskDiscovery    = 29,
   maskSupplier            = 30,
   performRouterDiscovery  = 31,
   routerSolicitationAddr  = 32,
   staticRoute             = 33,
   trailerEncapsulation    = 34,
   arpCacheTimeout         = 35,
   ethernetEncapsulation   = 36,
   tcpDefaultTTL           = 37,
   tcpKeepaliveInterval    = 38,
   tcpKeepaliveGarbage     = 39,
   nisDomainName           = 40,
   nisServers              = 41,
   ntpServers              = 42,
   vendorSpecificInfo      = 43,
   netBIOSnameServer       = 44,
   netBIOSdgramDistServer  = 45,
   netBIOSnodeType         = 46,
   netBIOSscope            = 47,
   xFontServer             = 48,
   xDisplayManager         = 49,
   dhcpRequestedIPaddr     = 50,
   dhcpIPaddrLeaseTime     = 51,
   dhcpOptionOverload      = 52,
   dhcpMessageType         = 53,
   dhcpServerIdentifier    = 54,
   dhcpParamRequest        = 55,
   dhcpMsg                 = 56,
   dhcpMaxMsgSize          = 57,
   dhcpT1value             = 58,
   dhcpT2value             = 59,
   dhcpClassIdentifier     = 60,
   dhcpClientIdentifier    = 61,
   endOption               = 255
};










void dhcp_discover(dhcp_pack * discover);



u8 dhcp_retry(void);



//获取option数据
u8 * nest_Options(u8 *options);





typedef unsigned char SOCKET;
extern void	Write_W5500_SOCK_1Byte(SOCKET s,unsigned short reg,unsigned short dat);
extern void Write_W5500_SOCK_2Byte(SOCKET s, unsigned short reg, unsigned short dat);
extern void Write_W5500_SOCK_4Byte(SOCKET s, unsigned short reg, unsigned char *dat_ptr);
extern void Write_W5500_nByte(unsigned short reg, unsigned char *dat_ptr, unsigned short size);
extern u8 Read_W5500_SOCK_1Byte(SOCKET s, unsigned short reg);
extern u8 Read_W5500_1Byte(unsigned short reg);

#endif


















