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
uint8 DNS_SERVER[4];//DNS������IP��ַ
uint16 MSG_ID = 0x1122;
//uint8 BUFPUB[1024];

 // CONFIG_MSG ConfigMsg;

/*
********************************************************************************
*              MAKE DNS QUERY MESSAGE
*
* Description : �˺������� dns ��ѯ��Ϣ��
* Arguments   : op   - ����ĵݹ� ֵΪ0
*               name - ��ָ��������ָ�롣
*               buf  - ��ָ�� dns ��Ϣ�Ļ�������ָ�롣
*               len  - �����ֵ���������Ĵ�С��
* Returns     :  dns ��Ϣ�Ĵ�С
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
  p = (op << 11) | 0x0100;			/* ����ĵݹ� */
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
    /* ������һ���� */
    cp1 = (unsigned char*)strchr((char*)dname, '.');
    
    if (cp1) len = cp1 - dname;	/* ���༴������ */
    else len = dlen;			/* ���һ����� */
    
    *cp++ = len;				/* �����д�볤�� */
    if (len == 0) break;
    
    /* ��������Ƶ� (��������) �� */
    strncpy((char *)cp, (char*)dname, len);
    cp += len;
    if (!cp1)
    {
      *cp++ = 0;			/* ���һ����д��ղ���� */
      break;
    }
    dname += len+1;
    dlen -= len+1;
  }
  
  *(uint16*)&cp[0] = htons(0x0001);				/* ���� */
  *(uint16*)&cp[2] = htons(0x0001);				/* �� */
  cp += sizeof(uint16)*2;
  
  return ((int)((uint32)(cp) - (uint32)(buf)));
}

/*
********************************************************************************
*              CONVERT A DOMAIN NAME TO THE HUMAN-READABLE FORM
*
* Description : �˺�����ѹ��������ת��Ϊ����ɶ�����ʽ
* Arguments   : msg        - ��ָ��ظ���Ϣ��ָ��
*               compressed - �ǻظ���Ϣ�е�������ָ�롣
*               buf        - ��ָ��ɶ��������ƵĻ�������ָ�롣
*               len        - �����ֵ���������Ĵ�С��
* Returns     : ѹ����Ϣ�ĳ���
* Note        :
********************************************************************************
*/
#define __USE_MYEM 0

#if __USE_MYEM==0
						//���������������������յ��˷�DNS����
int parse_name(uint8 * msg, uint8 * compressed, /*char * buf,*/ uint16 len)
{
  uint16 slen;		/* ��ǰ�εĳ��� */
  uint8  * cp;
  int16  clen = 0;		/* ѹ�����Ƶ��ܳ��� */
  int16  indirect = 0;	/* ��������������, ������ */
  int16  nseg = 0;		/* �����еĶ����� */
  int8   *name=mymalloc(MAX_DNS_BUF_SIZE);
  int8   *buf;
  
  buf = name;
  
  cp = compressed;
  
  for (;;)
  {
    slen = *cp++;	/* �˶εĳ��� */
    
    if (!indirect) clen++;
    
    if ((slen & 0xc0) == 0xc0)
    {
      if (!indirect)
        clen++;
      indirect = 1;
      /* ���ŷ����� */
      cp = &msg[((slen & 0x3f)<<8) + *cp];//�ҵ���������ʼ��ַ
      slen = *cp++;
    }
    
    if (slen == 0)	/* �㳤�� = = ȫ����� */
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
    /* ������;��ʾΪ���� */
    *buf++ = '.';
    len--;
  }
  
  *buf++ = '\0';
  len--;
  myfree(name);
  return clen;	/* ѹ����Ϣ�ĳ��� */
}
#else



#endif








/*
********************************************************************************
*              PARSE QUESTION SECTION
*
* Description : �˺�����������Ϣ�� qeustion ��¼��
* Arguments   : msg - ��ָ��ظ���Ϣ��ָ��
*               cp  - ��ָ�� qeustion ��¼��ָ�롣
* Returns     : ָ����һ����¼��ָ�롣
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
* Description : �˺�����������Ϣ��Ӧ���¼��
* Arguments   : msg - ��ָ��ظ���Ϣ��ָ��
*               cp  - ��ָ��𰸼�¼��ָ�롣
* Returns     : ָ����һ����¼��ָ�롣
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
      /* ��Щ���Ͷ���һ��������� */
      /* ����ת��Ϊ ascii ��ʽ */
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
      /* ��ȡ������������*/
      len = parse_name(msg, cp,/* name,*/ MAX_DNS_BUF_SIZE);
      if (len == -1) return 0;
      
      cp += len;
      break;
    case TYPE_SOA:
      /* ��ȡ���Ʒ����������� */
      len = parse_name(msg, cp,/* name,*/ MAX_DNS_BUF_SIZE);
      if (len == -1) return 0;
      
      cp += len;
      
      /* ��ȡ�����˵����� */
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
      /* ֻ�ǲ����� */
      break;
    default:
    /* Ignore ����*/
    break;
  }
  
  return cp;
}

/*
********************************************************************************
*              PARSE THE DNS REPLY
*
* Description :�˺����������� dns �������Ĵ���Ϣ��
* Arguments   : dhdr - ��ָ�� dns ��Ϣ�ı�ͷ��ָ��
*               buf  - ��ָ��ظ���Ϣ��ָ�롣
*               len  - �ǻظ���Ϣ�Ĵ�С��
* Returns     : ����1���ɹ���1��ʧ��
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
  
  /* ���ڷ����ɱ䳤�Ȳ��� */
  cp = &msg[12];
  
  /* ���ⲿ�� */
  for (i = 0; i < pdhdr->qdcount; i++)
  {
    cp = dns_question(msg, cp);//ԭʼ���ݣ�ȥ����ͷ������
  }
  
  /* �𰸲��� */
  for (i = 0; i < pdhdr->ancount; i++)
  {
    cp = dns_answer(msg, cp);
  }
  
  /* ���Ʒ����� (Ȩ��) ���� */
  for (i = 0; i < pdhdr->nscount; i++)
  {
    ;
  }
  
  /* �������� */
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
* Description : �˺���ʹ dns ��ѯ��Ϣ���������� dns �������Ĵ𸴡�
* Arguments   : name - ��ָ��������ָ�롣
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

