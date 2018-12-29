#ifndef __OS_MSG_H
#define __OS_MSG_H

#include "includes.h"



		//�����������
#define MESSEG_NUM 30
		//����λ����
#define MESSEG_DATA 9



#define  MSG_TYPE 		0xff000000
#define  SYS_MSG	 		0x00000000
#define  KEY_INPUT 		0x01000000
#define  GET_MSGTYPE(msg,type) (((msg)&(MSG_TYPE))==(type))//��Ϣ�����ж���



typedef struct
{
	u8 type;			//���͸�˭�ģ�Ϊ0��û����
	u8 data[MESSEG_DATA];				//��Ϣ������
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


#endif

