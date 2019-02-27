#ifndef Ping_H_
#define Ping_H_



#define BUF_LEN 128
#define PING_REQUEST 8
#define PING_REPLY 0
#define CODE_ZERO 0

#define SOCKET_ERROR 1
#define TIMEOUT_ERROR 2
#define SUCCESS 3
#define REPLY_ERROR 4

#ifndef uint8
	#define uint8 u8
#endif
#ifndef uint16
	#define uint16 u16
#endif
#ifndef u_int
	#define u_int unsigned int
#endif
#ifndef u_long 
	#define u_long unsigned long
#endif
#ifndef u_short
	#define u_short unsigned short
#endif



typedef struct pingmsg
{
  uint8  Type; 		// 0 - Ping Reply, 8 - Ping Request
  uint8  Code;		// Always 0
  uint16  CheckSum;	// Check sum
  uint16  ID;	            // Identification
  uint16  SeqNum; 	// Sequence Number
  int8_t  Data[BUF_LEN];// Ping Data  : 1452 = IP RAW MTU - sizeof(Type+Code+CheckSum+ID+SeqNum)
} PINGMSGR;


u16 ping_auto(uint8 s, uint8 *addr);
uint8 ping_request(uint8 s, uint8 *addr);
uint8 ping_reply(uint8 s, uint8 *addr,  uint16 rlen);




#endif



