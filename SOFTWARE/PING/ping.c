#include "enternet.h"
#include "ping.h"



static u16 PingDelay=0;


/**
*@brief		�Զ�PING����IP����
*@param		s��socket number
*@param		addr������IP��ַ
*@return	��
*/

u16 ping_auto(uint8 s, uint8 *addr)
{
	u16 delay_time=0;
	Write_W5500_SOCK_1Byte(s,Sn_CR,SN_CLOSE);
	Write_W5500_SOCK_1Byte(s,Sn_PROTO,IPPROTO_ICMP);
	Write_W5500_SOCK_1Byte(s,Sn_MR,Sn_MR_IPRAW);//��Socket*/
	Write_W5500_SOCK_2Byte(s, Sn_PORT,30);
	Write_W5500_SOCK_1Byte(s,Sn_CR,SN_OPEN);
	while (Read_W5500_SOCK_1Byte(s,Sn_CR));					//�ȴ�ִ�����
	
	while(Read_W5500_SOCK_1Byte(s,Sn_SR)!=SOCK_IPRAW);//�ȴ��������
	delay_ms(2);
	Write_W5500_SOCK_4Byte(s, Sn_DIPR, addr);			
	ping_request(s, addr);															/*����Ping����*/
	PingDelay=0; 
	u8 i=200;
	extern u8 NET_S2_STATE;
	do
	{
		delay_ms(20);
	//	if (Read_W5500_SOCK_1Byte(s,Sn_RX_RSR1))//������յ���Ϣ
		if (NET_S2_STATE&IR_RECV)
		{
			NET_S2_STATE&=~IR_RECV;
			if (ping_reply(s, addr, 2048)==0)												/*��ȡ�ظ���Ϣ*/
				break;
		}
	}while(--i);
	if (i==0)
	{
		PingDelay=-1;
	}
	if (PingDelay==0) PingDelay=1;
	return PingDelay;
}

//		ͳ��Ping������������ʱ
void PingCalculationDelayIRQ (void)
{
	PingDelay+=10;
}


static uint16 RandomID = 0x1234; 
static uint16 RandomSeqNum = 0x4321;





/**
*@brief		�����ַ���У��ֵ
*@param		Ҫת��������
*@return 	У��ֵ
*/ 
unsigned short checksum(
	unsigned char * src, 	/**< pointer to stream  */
	unsigned int len		/**< size of stream */
	)
{
	u_int sum, tsum, i, j;
	u_long lsum; 

	j = len >> 1;

	lsum = 0;

	for (i = 0; i < j; i++) 
	{
		tsum = src[i * 2];
		tsum = tsum << 8;
		tsum += src[i * 2 + 1];
		lsum += tsum;
	}

	if (len % 2) 
	{
		tsum = src[i * 2];
		lsum += (tsum << 8);
	}


	sum = lsum;
	sum = ~(sum + (lsum >> 16));
	return (u_short) sum;	 
}






/**
*@brief		ping��Ӧ����
*@param		s-  socket number
*@param		addr-  P��ַ
*@return	��
*/
uint8 ping_request(uint8 s, uint8 *addr)
{
	uint16 i;
	PINGMSGR PingRequest ;      												
	PingRequest.Type = PING_REQUEST;                  				/*Ping-Request*/
	PingRequest.Code = CODE_ZERO;	                   					/*���� '0'*/
	PingRequest.ID = htons(RandomID++);	       								/*����ping��ӦIDΪ��������ͱ���*/
	PingRequest.SeqNum =htons(RandomSeqNum++);								/*����ping��Ӧ�����к�Ϊ������α���*/
	for(i = 0 ; i < BUF_LEN; i++)
	{	                                
		PingRequest.Data[i] = (i) % 8+'a';		 				 							/*ping��Ӧ������'0'~'8��*/
	}
	PingRequest.CheckSum = 0;
	/* ������Ӧ����*/
	PingRequest.CheckSum = htons(checksum((uint8*)&PingRequest,sizeof(PingRequest))); 
	/*����ping��Ӧ��Ŀ�ķ� */
	Write_SOCK_Data_Buffer(s, (uint8 *)&PingRequest,sizeof(PingRequest));
	return 0;
} 



/**
*@brief		����Ping�ظ�
*@param		s-  socket number
*@param		addr- Ping��ַ
*@return	��
*/
uint8 ping_reply(uint8 s, uint8 *addr,  uint16 rlen)
{ 
	uint16 tmp_checksum;
	uint16 datalen=0;
	
	PINGMSGR PingReply;
	u8 *data_buf=mymalloc(2048);
	datalen=Read_SOCK_Data_Buffer(s, (uint8 *)data_buf);			/*��Ŀ�Ķ˽�������*/
	if ((addr[0]==data_buf[0])&&(addr[1]==data_buf[1])&&(addr[2]==data_buf[2])&&(addr[3]==data_buf[3]))
	{
		if(data_buf[6] == PING_REPLY) 
		{
			myfree(data_buf);
			return 0;
		}
	}
	myfree(data_buf);
	return 1;
}




