#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "delay.h"
//#include "util.h"
#include "w5500.h"
//#include "socket.h"
#include "dns.h"
//#include "sockutil.h"


uint8 DNS_GET_IP[4];
uint8 DNS_SERVER[4];//DNS服务器IP地址
uint16 MSG_ID = 0x1122;
//uint8 BUFPUB[1024];

 // CONFIG_MSG ConfigMsg;

/*
********************************************************************************
*              MAKE DNS QUERY MESSAGE
*
* Description : 此函数生成 dns 查询消息。
* Arguments   : op   - 所需的递归 值为0
*               name - 是指向域名的指针。
*               buf  - 是指向 dns 消息的缓冲区的指针。
*               len  - 是最大值。缓冲区的大小。
* Returns     :  dns 消息的大小
* Note        :
********************************************************************************
*/
int dns_makequery(uint16 op, uint8 * name, uint8 * buf, uint16 len)
{
  uint8  *cp;
  uint8   *cp1;
  //	int8   sname[MAX_DNS_BUF_SIZE];
  uint8  *dname;
  uint16 p;
  uint16 dlen;
  
  cp = buf;
  
  MSG_ID++;
  *(uint16*)&cp[0] = htons(MSG_ID); 
  p = (op << 11) | 0x0100;			/* 所需的递归 */
  *(uint16*)&cp[2] = htons(p);
  *(uint16*)&cp[4] = htons(1);
  *(uint16*)&cp[6] = htons(0);
  *(uint16*)&cp[8] = htons(0);
  *(uint16*)&cp[10]= htons(0);
  
  cp += sizeof(uint16)*6;
  //	strcpy(sname, name);
  dname = name;
  dlen = strlen((char*)dname);
  for (;;)
  {
    /* 查找下一个点 */
    cp1 = (unsigned char*)strchr((char*)dname, '.');
    
    if (cp1) len = cp1 - dname;	/* 更多即将到来 */
    else len = dlen;			/* 最后一个组件 */
    
    *cp++ = len;				/* 组件的写入长度 */
    if (len == 0) break;
    
    /* 将组件复制到 (但不包括) 点 */
    strncpy((char *)cp, (char*)dname, len);
    cp += len;
    if (!cp1)
    {
      *cp++ = 0;			/* 最后一个。写入空并完成 */
      break;
    }
    dname += len+1;
    dlen -= len+1;
  }
  
  *(uint16*)&cp[0] = htons(0x0001);				/* 类型 */
  *(uint16*)&cp[2] = htons(0x0001);				/* 类 */
  cp += sizeof(uint16)*2;
  
  return ((int)((uint32)(cp) - (uint32)(buf)));
}

/*
********************************************************************************
*              CONVERT A DOMAIN NAME TO THE HUMAN-READABLE FORM
*
* Description : 此函数将压缩的域名转换为人类可读的形式
* Arguments   : msg        - 是指向回复消息的指针
*               compressed - 是回复消息中的域名的指针。
*               buf        - 是指向可读窗体名称的缓冲区的指针。
*               len        - 是最大值。缓冲区的大小。
* Returns     : 压缩消息的长度
* Note        :
********************************************************************************
*/
#define __USE_MYEM 0

#if __USE_MYEM==0
						//程序在这里死机可能是收到了非DNS报文
int parse_name(uint8 * msg, uint8 * compressed, /*char * buf,*/ uint16 len)
{
  uint16 slen;		/* 当前段的长度 */
  uint8  * cp;
  int16  clen = 0;		/* 压缩名称的总长度 */
  int16  indirect = 0;	/* 如果遇到间接问题, 则设置 */
  int16  nseg = 0;		/* 名称中的段总数 */
  int8   *name=mymalloc(MAX_DNS_BUF_SIZE);
  int8   *buf;
  
  buf = name;
  
  cp = compressed;
  
  for (;;)
  {
    slen = *cp++;	/* 此段的长度 */
    
    if (!indirect) clen++;
    
    if ((slen & 0xc0) == 0xc0)
    {
      if (!indirect)
        clen++;
      indirect = 1;
      /* 跟着方向走 */
      cp = &msg[((slen & 0x3f)<<8) + *cp];//找到域名的起始地址
      slen = *cp++;
    }
    
    if (slen == 0)	/* 零长度 = = 全部完成 */
      break;
    
    len -= slen + 1;
    
    if (len <= 0) {myfree(name);return -1;}
    
    if (!indirect) clen += slen;
    
    while (slen-- != 0) *buf++ = (int8)*cp++;
    *buf++ = '.';
    nseg++;
  }
  
  if (nseg == 0)
  {
    /* 根名称;表示为单点 */
    *buf++ = '.';
    len--;
  }
  
  *buf++ = '\0';
  len--;
  myfree(name);
  return clen;	/* 压缩消息的长度 */
}
#else



#endif








/*
********************************************************************************
*              PARSE QUESTION SECTION
*
* Description : 此函数分析答复消息的 qeustion 记录。
* Arguments   : msg - 是指向回复消息的指针
*               cp  - 是指向 qeustion 记录的指针。
* Returns     : 指向下一条记录的指针。
* Note        :
********************************************************************************
*/
uint8 * dns_question(uint8 * msg, uint8 * cp)
{
  int16 len;
  //	int8  xdata name[MAX_DNS_BUF_SIZE];
  
  len = parse_name(msg, cp, /*name,*/ MAX_DNS_BUF_SIZE);
  
  if (len == -1) return 0;
  
  cp += len;
  cp += 2;		/* type */
  cp += 2;		/* class */
  
  return cp;
}


/*
********************************************************************************
*              PARSE ANSER SECTION
*
* Description : 此函数分析答复消息的应答记录。
* Arguments   : msg - 是指向回复消息的指针
*               cp  - 是指向答案记录的指针。
* Returns     : 指向下一条记录的指针。
* Note        :
********************************************************************************
*/
uint8 * dns_answer(uint8 * msg, uint8 * cp)
{
  int16 len, type;
  //	int8  xdata name[MAX_DNS_BUF_SIZE];
  
  len = parse_name(msg, cp, /*name,*/ MAX_DNS_BUF_SIZE);
  
  if (len == -1) return 0;
  
  cp += len;
  type = ntohs(*((uint16*)&cp[0])); 
  cp += 2;		/* type */
  cp += 2;		/* class */
  cp += 4;		/* ttl */
  cp += 2;		/* len */
  
  switch (type)
  {
    case TYPE_A:
      DNS_GET_IP[0] = *cp++;
      DNS_GET_IP[1] = *cp++;
      DNS_GET_IP[2] = *cp++;
      DNS_GET_IP[3] = *cp++;
      break;
    case TYPE_CNAME:
    case TYPE_MB:
    case TYPE_MG:
    case TYPE_MR:
    case TYPE_NS:
    case TYPE_PTR:
      /* 这些类型都由一个域名组成 */
      /* 将其转换为 ascii 格式 */
      len = parse_name(msg, cp, /*name,*/ MAX_DNS_BUF_SIZE);
      if (len == -1) return 0;
      
      cp += len;
      break;
    case TYPE_HINFO:
      len = *cp++;
      cp += len;
      
      len = *cp++;
      cp += len;
      break;
    case TYPE_MX:
      cp += 2;
      /* 获取交换器的域名*/
      len = parse_name(msg, cp,/* name,*/ MAX_DNS_BUF_SIZE);
      if (len == -1) return 0;
      
      cp += len;
      break;
    case TYPE_SOA:
      /* 获取名称服务器的域名 */
      len = parse_name(msg, cp,/* name,*/ MAX_DNS_BUF_SIZE);
      if (len == -1) return 0;
      
      cp += len;
      
      /* 获取负责人的域名 */
      len = parse_name(msg, cp,/* name,*/ MAX_DNS_BUF_SIZE);
      if (len == -1) return 0;
      
      cp += len;
      
      cp += 4;
      cp += 4;
      cp += 4;
      cp += 4;
      cp += 4;
      break;
    case TYPE_TXT:
      /* 只是藏起来 */
      break;
    default:
    /* Ignore 忽略*/
    break;
  }
  
  return cp;
}

/*
********************************************************************************
*              PARSE THE DNS REPLY
*
* Description :此函数分析来自 dns 服务器的答复消息。
* Arguments   : dhdr - 是指向 dns 消息的标头的指针
*               buf  - 是指向回复消息的指针。
*               len  - 是回复消息的大小。
* Returns     : 返回1，成功，1，失败
* Note        :
********************************************************************************
*/
uint8 parseMSG(struct dhdr * pdhdr, uint8 * pbuf)
{
  uint16 tmp;
  uint16 i;
  uint8 * msg;
  uint8 * cp;
  
  msg = pbuf;
  memset(pdhdr, 0, sizeof(struct dhdr));
  
  pdhdr->id = ntohs(*((uint16*)&msg[0]));
  tmp = ntohs(*((uint16*)&msg[2]));
  if (tmp & 0x8000) pdhdr->qr = 1;
  
  pdhdr->opcode = (tmp >> 11) & 0xf;
  
  if (tmp & 0x0400) pdhdr->aa = 1;
  if (tmp & 0x0200) pdhdr->tc = 1;
  if (tmp & 0x0100) pdhdr->rd = 1;
  if (tmp & 0x0080) pdhdr->ra = 1;
  
  pdhdr->rcode = tmp & 0xf;
  pdhdr->qdcount = ntohs(*((uint16*)&msg[4]));
  pdhdr->ancount = ntohs(*((uint16*)&msg[6]));
  pdhdr->nscount = ntohs(*((uint16*)&msg[8]));
  pdhdr->arcount = ntohs(*((uint16*)&msg[10]));
  
  /* 现在分析可变长度部分 */
  cp = &msg[12];
  
  /* 问题部分 */
  for (i = 0; i < pdhdr->qdcount; i++)
  {
    cp = dns_question(msg, cp);//原始数据，去掉包头的数据
  }
  
  /* 答案部分 */
  for (i = 0; i < pdhdr->ancount; i++)
  {
    cp = dns_answer(msg, cp);
  }
  
  /* 名称服务器 (权限) 部分 */
  for (i = 0; i < pdhdr->nscount; i++)
  {
    ;
  }
  
  /* 其他部分 */
  for (i = 0; i < pdhdr->arcount; i++)
  {
    ;
  }
  
  if(pdhdr->rcode == 0) return 1;		// No error
  else return 0;
}



/*
********************************************************************************
*              MAKE DNS QUERY AND PARSE THE REPLY
*
* Description : 此函数使 dns 查询消息并分析来自 dns 服务器的答复。
* Arguments   : name - 是指向域名的指针。
* Returns     : if succeeds : 1, fails : -1
* Note        :
********************************************************************************
*/
uint8 dns_query(uint8 s, uint8 * name,u8 * ip)
{
	extern u8 NET_S0_STATE;
	extern u8 NET_S1_STATE;
	extern u8 NET_S2_STATE;
	u8 *buff=mymalloc(2048);
	u16 lens=0;
	struct dhdr ddd;
	u8 ret=0;
	u8 i=0;
	if (udp_init(s,444)==TRUE)
	{
		lens=dns_makequery(0,name,buff,2048);
		udp_send(s,DNS_SERVER,53,buff,lens);
		for (i=0;i<10;i++)
		{
			delay_ms(200);
			if (s==0)
			{
				if(NET_S0_STATE&IR_RECV)
				{
					NET_S0_STATE&=~IR_RECV;
					Read_SOCK_Data_Buffer(s,buff);
					if (parseMSG(&ddd,buff+8))
					{
						mymemcpy(ip,DNS_GET_IP,4);
						ret=1;
						break;
					}
				}
			}
			else if (s==1)
			{
				if(NET_S1_STATE&IR_RECV)
				{
					NET_S1_STATE&=~IR_RECV;
					Read_SOCK_Data_Buffer(s,buff);
					if (parseMSG(&ddd,buff+8))
					{
						mymemcpy(ip,DNS_GET_IP,4);
						ret=1;
						break;
					}
				}
			}
			else if (s==2)
			{
				if(NET_S2_STATE&IR_RECV)
				{
					NET_S2_STATE&=~IR_RECV;
					Read_SOCK_Data_Buffer(s,buff);
					if (parseMSG(&ddd,buff+8))
					{
						mymemcpy(ip,DNS_GET_IP,4);
						ret=1;
						break;
					}
				}
			}
		}
		
	}
	if (i>=9) ret=0;
	myfree(buff);
	return ret;
}

