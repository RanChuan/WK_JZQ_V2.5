#ifndef	_DNS_H_
#define	_DNS_H_

//#include "types.h"

//#include "device.h"
//#include "config.h"
#include "includes.h"
#include "enternet.h"





/***********以下是移植者添加**********/

#define uint8 u8
#define uint16 u16
#define uint32 u32
#define int16 s16
#define int8 s8
#define int32 s32


#define getSn_SR(s) Read_W5500_SOCK_1Byte(s,Sn_SR)
#define getSn_RX_RSR(s) (2048)		//获取接收缓冲区大小
#define htons(s) __REV16(s)					//16位大小端转换
#define ntohs(s) __REV16(s)					//16位大小端转换

/**********移植者添加End***************/




extern uint8 BUFPUB[1024];

#define  DNS_RESPONSE_TIMEOUT 3 // 3 seconds
#define  DNS_RETRY            3//3 times
#define  DNS_RET_PROGRESS  0
#define  DNS_RET_FAIL      1
#define  DNS_RET_SUCCESS   2

#define  EXTERN_DNS_SERVERIP  ConfigMsg.dns
#define  EXTERN_DNS_SOCKBUF   gBUFPUBLIC
#define  EXTERN_DNS_ANYPORT   2000//tsvDEVCONFnew.any_port

#define	MAX_DNS_BUF_SIZE	256		/* maximum size of DNS buffer. */


#define  INITRTT     2000L	/* Initial smoothed response time */
#define  MAXCNAME    10	/* Maximum amount of cname recursion */
         
#define  TYPE_A		1	/* Host address */
#define  TYPE_NS		2	/* Name server */
#define  TYPE_MD		3	/* Mail destination (obsolete) */
#define  TYPE_MF		4	/* Mail forwarder (obsolete) */
#define  TYPE_CNAME	5	/* Canonical name */
#define  TYPE_SOA    6	/* Start of Authority */
#define  TYPE_MB		7	/* Mailbox name (experimental) */
#define  TYPE_MG		8	/* Mail group member (experimental) */
#define  TYPE_MR		9	/* Mail rename name (experimental) */
#define  TYPE_NULL	10	/* Null (experimental) */
#define  TYPE_WKS    11	/* Well-known sockets */
#define  TYPE_PTR    12	/* Pointer record */
#define  TYPE_HINFO	13	/* Host information */
#define  TYPE_MINFO	14	/* Mailbox information (experimental)*/
#define  TYPE_MX		15	/* Mail exchanger */
#define  TYPE_TXT    16	/* Text strings */
#define  TYPE_ANY    255/* Matches any type */
         
#define  CLASS_IN    1  /* The ARPA Internet */

/* Round trip timing parameters */
#define  AGAIN       8	/* Average RTT gain = 1/8 */
#define  LAGAIN      3	/* Log2(AGAIN) */
#define  DGAIN       4  /* Mean deviation gain = 1/4 */
#define  LDGAIN      2  /* log2(DGAIN) */
         
#define  IPPORT_DOMAIN	53













#define	QUERY		0
#define	RESPONSE	1
#define	IQUERY		1

#define	NO_ERROR	0
#define	FORMAT_ERROR	1
#define	SERVER_FAIL	2
#define	NAME_ERROR	3
#define	NOT_IMPL	4
#define	REFUSED		5

/* Header for all domain messages */
struct dhdr
{
	uint16  id;		/* Identification */
	uint8 	qr;		/* Query/Response */
	uint8 	opcode;
	uint8 	aa;		/* Authoratative answer */
	uint8 	tc;		/* Truncation */
	uint8 	rd;		/* Recursion desired */
	uint8 	ra;		/* Recursion available */
	uint8 	rcode;		/* Response code */
	uint16  qdcount;	/* Question count */
	uint16  ancount;	/* Answer count */
	uint16  nscount;	/* Authority (name server) count */
	uint16  arcount;	/* Additional record count */
};



extern uint8 DNS_GET_IP[4];
uint8 dns_query(uint8 s, uint8 * name,u8 *ip);//获取域名的IP地址
int dns_makequery(uint16 op, uint8 * name, uint8 * buf, uint16 len);

#endif	/* _DNS_H_ */
