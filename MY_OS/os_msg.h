#ifndef __OS_MSG_H
#define __OS_MSG_H

#include "includes.h"



		//�����������
#define MESSEG_NUM 30
		//����λ����
#define MESSEG_DATA 9


					//ϵͳ��Ϣ����
#define  MSG_TYPE 		0xff000000
#define  SYS_MSG	 		0x01000000
#define  TASK_MSG 		0x02000000
#define  GET_MSGTYPE(msg,type) (((msg)&(MSG_TYPE))==(type))//��Ϣ�����ж���



//�������ͽṹ��
typedef struct
{
	u8 msgfrom;
	u8 msgto;
	u16 errType;//���ش�������
	u16 extErrType;//��չ��������
	char *errStr;//���ش��������ַ���
	char *extErrStr;//��չ���������ַ���
} msgerr;


enum 
{
	msgNoneErr=0,				//û�д���
	msgUnknownErr=1,				//δ֪�Ĵ���
	msgQueueIsFull=2,				//��������
	msgAddrErr=3,				//Ŀ���ַ����
	msgMoreErr=4,				//������ִ�н����ɵĴ���
	msgSameType=5,			//����ͬ����Ϣ�ڶ�����
};





			//��Ϣ����������
typedef union 
{
	u8 u8dat[MESSEG_DATA];
	u8 *u8ptr;
	u16 *u16ptr;
	u32 *u32ptr;
	void *voidptr;
} msgdata;



#define MSG_FLAG_SEND 0x08			//���͵���Ϣ
#define MSG_FLAG_RECV 0x80			//������Ҫ���͵�ִ�н��������Ϣ
#define MSG_FLAG_ENRE	0x01			//����Ϣ��Ҫ����ִ�н��
#define MSG_FLAG_CEECK 0x02			//����ʱ���������Ƿ���ڷ��͵��õ�ַ����Ϣ������з��ش���

#define MSG_TYPE_U8  			0x01
#define MSG_TYPE_U8PTR 		0x02
#define MSG_TYPE_U16PTR 	0x03
#define MSG_TYPE_U32PTR 	0x04
#define MSG_TYPE_VOIDPTR 	0x05

typedef struct
{
	u8 msgto;			//���͸�˭�ģ�Ϊ0��û����
	u8 msgfrom;		//��ϢԴͷ
	u8 msgflag;		//��Ϣ��־���Ƿ�ȴ����ص�
	u8 msgtype;		//��Ϣ���ͣ�
	msgdata data;				//��Ϣ�����ݣ���Ϣ���ݸ��ݾ����ʹ����ȷ��
} messeg,*pmesseg;


extern messeg my_messeg[MESSEG_NUM];



//������Ϣ��0���ɹ���1��ʧ��
u8 send_messeg (u8 type,u8 *data);

//������Ϣ��0���ɹ���1��ʧ��
u8 get_messeg (u8 type,u8 *data);
			//�Ҹ����͵���Ϣ�Ƿ���ڣ�1���У�0����
u8 find_messeg(u8 type);

//�ȴ���Ϣ��0���ɹ���1��ʧ��
u8 wait_messeg (u8 type,u8 *data);

u8 SetFocus(u8 pro);

u8 GetFocus (void);

INT8U os_msg_init (void);


///////////////��������Ϣ����2018.12.29////////////////////////

//��ȡ�������͵��ַ�����ʽ
char *msgErrToStr (u16 errType);

		//������Ϣ�ṹ��
void msgCopyMsg(msgdata *from,msgdata *to,u8 msgtype);

msgerr Msg_Wait (u32 msg_type,msgdata *msg,u8 *datatype,u8 *msgflag);

msgerr Msg_Send (u8 to_task_pro,msgdata *msg,u8 msgtype, u8 msgflag);//,u16 reterr,char *errstr)

msgerr Msg_Get (u32 msg_type,msgdata *msg,u8 *datatype,u8 *msgflag);

#endif

