
#include "os_msg.h"
#include "includes.h"



INT8U os_msg_init (void)
{
	return 0;
}




messeg my_messeg[MESSEG_NUM];


/**********************************************

		����ṹ���У�MYWork��Ϊ0��ʾ�յ�����Ϣ
		����ֽ�Ϊ0x00,�յ�����ϵͳ��Ϣ
							0x01,������Ϣ


**********************************************/

			//�Ҹ����͵���Ϣ�Ƿ���ڣ�1���У�0����
u8 find_messeg(u8 type)
{
	u16 i=0;
	u16 t=0;
	for (i=0;i<MESSEG_NUM;i++)
	{
		if (my_messeg[i].type==type) return 1;
	}
	return 0;//û�ҵ������͵�
	
}

//������Ϣ��0���ɹ���1��ʧ��
u8 send_messeg (u8 type,u8 *data)
{
	u16 i=0;
	u16 t=0;
	for (i=0;i<MESSEG_NUM;i++)//������û�п�λ
	{
		if (my_messeg[i].type==0) break;
	}
	if (i==MESSEG_NUM) return 1;//����������
	my_messeg[i].type=type;
	for (t=0;t<MESSEG_DATA;t++)
	{
		my_messeg[i].data[t]=data[t];
	}
	TaskSendMsg(type,2);
	return 0;
}


//������Ϣ��0���ɹ���1��ʧ��
u8 get_messeg (u8 type,u8 *data)
{
	u16 i=0;
	u16 t=0;
	for (i=0;i<MESSEG_DATA;i++)
	{
		data[i]=0;
	}
	
		for (i=0;i<MESSEG_NUM;i++)
		{
			if (my_messeg[i].type==type) break;
		}
		if (i==MESSEG_NUM) return 1;//û�ҵ������͵�
	for (t=0;t<MESSEG_DATA;t++)
	{
		data[t]=my_messeg[i].data[t];
		my_messeg[i].data[t]=0;
	}
	my_messeg[i].type=0;	//��ȡ������ջ�����
	return 0;
}



//�ȴ���Ϣ��0���ɹ���1��ʧ��
u8 wait_messeg (u8 type,u8 *data)
{
	u16 i=0;
	u16 t=0;
	for (i=0;i<MESSEG_DATA;i++)
	{
		data[i]=0;
	}
	
	while(1)
	{
		for (i=0;i<MESSEG_NUM;i++)
		{
			if (my_messeg[i].type==type) break;
		}
		if (i==MESSEG_NUM) //return 1;//û�ҵ������͵�
		{
			TaskGetMsg();
		}
		else
		{
			break;
		}
	}
	for (t=0;t<MESSEG_DATA;t++)
	{
		data[t]=my_messeg[i].data[t];
		my_messeg[i].data[t]=0;
	}
	my_messeg[i].type=0;	//��ȡ������ջ�����
	return 0;
}









u8 INPUT_FOCUS=TASK_MAX_NUM;//���뽹������Ϊһ�������ܵ�ֵ
u8 SetFocus(u8 pro)
{
	if (pro<TASK_MAX_NUM)
	{
		INPUT_FOCUS=pro;
		return 0;
	}
	else
	{
		return 1;
	}
}

u8 GetFocus (void)
{
	return INPUT_FOCUS;
}



