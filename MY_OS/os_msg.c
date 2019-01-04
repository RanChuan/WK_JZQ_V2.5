
#include "os_msg.h"
#include "includes.h"



INT8U os_msg_init (void)
{
	msgInit();
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
		if (my_messeg[i].msgto==type) return 1;
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
		if (my_messeg[i].msgto==TASK_MAX_NUM) break;
	}
	if (i==MESSEG_NUM) return 1;//����������
	my_messeg[i].msgto=type;
	
	my_messeg[i].msgfrom=TASK_MAX_NUM;//�������������ʹ���µ���Ϣ���պ�������
	my_messeg[i].msgtype=MSG_TYPE_U8;
	
	for (t=0;t<MESSEG_DATA;t++)
	{
		my_messeg[i].data.u8dat[t]=data[t];
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
			if (my_messeg[i].msgto==type) break;
		}
		if (i==MESSEG_NUM) return 1;//û�ҵ������͵�
	for (t=0;t<MESSEG_DATA;t++)
	{
		data[t]=my_messeg[i].data.u8dat[t];
		my_messeg[i].data.u8dat[t]=0;
	}
	my_messeg[i].msgto=TASK_MAX_NUM;	//��ȡ������ջ�����
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
			if (my_messeg[i].msgto==type) break;
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
		data[t]=my_messeg[i].data.u8dat[t];
		my_messeg[i].data.u8dat[t]=0;
	}
	my_messeg[i].msgto=TASK_MAX_NUM;	//��ȡ������ջ�����
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


///////////////���һЩ�µ���Ϣ������2018.12.29//////////////////////

/*************************************************************
	��������������Ϣ����
	������to_task_pro ��ϢĿ���������ȼ�
	������msg �����õ���Ϣ����
	������msgtype ��Ϣ���ݵ����� ֻ��������ȡֵMSG_TYPE_U8��MSG_TYPE_U8PTR��MSG_TYPE_U16PTR��MSG_TYPE_U32PTR��MSG_TYPE_VOIDPTR
	������msgflag ��Ϣ��־ ����Ϊ���²��������MSG_FLAG_SEND��MSG_FLAG_RECV��MSG_FLAG_ENRE��MSG_FLAG_CEECK
	������reterr �Զ���������ͱ�־����msgflagΪMSG_FLAG_RECVʱ��Ҫ�õ�
	������errstr �Զ���Ĵ�������
	����ֵ����Ϣ�������ͽṹ��
**************************************************************/
msgerr Msg_Send (u8 to_task_pro,msgdata *msg,u8 msgtype, u8 msgflag,u16 reterr,char *errstr)
{
	u16 i=0;
	u16 t=0;
	for (i=0;i<MESSEG_NUM;i++)//������û�п�λ
	{
		if (my_messeg[i].msgto==TASK_MAX_NUM) break;
	}
	if (i==MESSEG_NUM) 
	{
		msgerr err;
		err.errType=msgQueueIsFull;//����������
		err.errStr=msgErrToStr(err.errType);
		err.msgfrom=OSPrioHighRdy;//��ϢԴͷ
		err.msgto=to_task_pro;//��Ϣ�ʹ��
		err.extErrType=my_messeg[MESSEG_NUM-1].msgto;//һ����Ϣ��������Ϊĳ������û�н��գ����Ի�ȡ���һ����Ϣ��Ŀ�ĵ�
		return err;
	}


	if (msgflag&MSG_FLAG_CEECK)			//Ҫ������Ϣ�������
	{
		for (t=0;t<MESSEG_NUM;t++)
		{
			if (my_messeg[t].msgto==to_task_pro) break;
		}
		if (t!=MESSEG_NUM) //��ͬ����Ϣ�����ش���
		{
			msgerr err;
			err.errType=msgSameType;
			err.errStr=msgErrToStr(err.errType);
			err.msgfrom=OSPrioHighRdy;//��ϢԴͷ
			err.msgto=to_task_pro;//��Ϣ�ʹ��
			err.extErrType=t;//ͬ����Ϣ��λ��
			return err;
		}
	}


	my_messeg[i].msgto=to_task_pro;
	my_messeg[i].msgfrom=OSPrioHighRdy;
	my_messeg[i].msgtype=msgtype;
	my_messeg[i].msgflag=msgflag;
	my_messeg[i].exterr=reterr;
	my_messeg[i].exterrstr=errstr;
	//������Ϣ����
	msgCopyMsg(msg,&my_messeg[i].data,msgtype);
	
	
	
	TaskSendMsg(to_task_pro,TASK_MSG);
				//�ж���Ϣ��־
	
	msgerr err;
	err.errType=msgNoneErr;
	err.errStr=msgErrToStr(err.errType);
	err.msgfrom=my_messeg[i].msgfrom;
	err.msgto=my_messeg[i].msgto;
	return err;
	
}


/****************************************************************

			���������ȴ���Ϣ,û����Ϣ�����������޵ȴ�
			������msg_type ��Ϣ���ͣ�����Ϊ����ֵ��������ϣ�SYS_MSG��TASK_MSG 
			������msg ��Ϣ���ݣ������洢���յ���Ϣ
			������datatype ��Ϣ�������ͣ�������ý��յ���Ϣ������
			������msgflag  ��Ϣ��־λ
****************************************************************/
msgerr Msg_Wait (u32 msg_type,msgdata *msg,u8 *datatype,u8 *msgflag)
{
	u16 i=0;
	u16 t=0;
	msgerr err={0};
	for (;;)
	{
		u32 task_msg=TaskGetMsg();
		if (msg_type&task_msg)
		{
			switch (task_msg)
			{
				case SYS_MSG:
					break;
				case TASK_MSG:
					for (i=0;i<MESSEG_NUM;i++)//������û�п�λ
					{
						if (my_messeg[i].msgto==OSPrioHighRdy) break;
					}
					*msgflag=my_messeg[i].msgflag;
					*datatype=my_messeg[i].msgtype;
					msgCopyMsg(&my_messeg[i].data,msg,*datatype);
					my_messeg[i].msgto=TASK_MAX_NUM;//ȥ����ϢĿ�ĵأ������Ϣ
					break;
				default :
					break;
			}
			if (my_messeg[i].exterr)//��ִ�д���
			{
				err.errType=msgMoreErr;
				err.errStr=msgErrToStr(err.errType);
				err.extErrType=my_messeg[i].exterr;
				err.extErrStr=my_messeg[i].exterrstr;
				err.msgfrom=my_messeg[i].msgfrom;
				err.msgto=my_messeg[i].msgto;
			}
			else
			{
				err.errType=msgNoneErr;
				err.errStr=msgErrToStr(err.errType);
				err.msgfrom=my_messeg[i].msgfrom;
				err.msgto=my_messeg[i].msgto;
			}
			return err;
		}
	}
}




/****************************************************************

			����������ѯ��Ϣ,����Ϣ�͸��ƣ�û����Ϣ������
			������msg_type ��Ϣ���ͣ�����Ϊ����ֵ��������ϣ�SYS_MSG��TASK_MSG 
			������msg ��Ϣ���ݣ������洢���յ���Ϣ
			������datatype ��Ϣ�������ͣ�������ý��յ���Ϣ������
			������msgflag  ��Ϣ��־λ
****************************************************************/

msgerr Msg_Get (u32 msg_type,msgdata *msg,u8 *datatype,u8 *msgflag)
{
	u16 i=0;
	u16 t=0;
	msgerr err={0};
	for (i=0;i<MESSEG_NUM;i++)//������û�п�λ
	{
		if (my_messeg[i].msgto==OSPrioHighRdy) break;
	}
	if (i<MESSEG_NUM)
	{
		*msgflag=my_messeg[i].msgflag;
		*datatype=my_messeg[i].msgtype;
		msgCopyMsg(&my_messeg[i].data,msg,*datatype);
		my_messeg[i].msgto=TASK_MAX_NUM;//ȥ����ϢĿ�ĵأ������Ϣ

		if (my_messeg[i].exterr)//��ִ�д���
		{
			err.errType=msgMoreErr;
			err.errStr=msgErrToStr(err.errType);
			err.extErrType=my_messeg[i].exterr;
			err.extErrStr=my_messeg[i].exterrstr;
			err.msgfrom=my_messeg[i].msgfrom;
			err.msgto=my_messeg[i].msgto;
		}
		else
		{
			err.errType=msgNoneErr;
			err.errStr=msgErrToStr(err.errType);
			err.msgfrom=my_messeg[i].msgfrom;
			err.msgto=my_messeg[i].msgto;
		}
		return err;
	}else
	{
		err.errType=msgNoneMsg;
		err.errStr=msgErrToStr(err.errType);
		return err;
	}
}




			//��ʼ����Ϣ�ṹ��
void msgInit(void)
{
	u16 i=0;
	for (i=0;i<MESSEG_NUM;i++)
	{
		my_messeg[i].msgto=TASK_MAX_NUM;
	}
}





		//������Ϣ�ṹ��
void msgCopyMsg(msgdata *from,msgdata *to,u8 msgtype)
{
	u16 t=0;
				//������Ϣ���͸�����Ϣ
	switch (msgtype)
	{
		case MSG_TYPE_U8:
			for (t=0;t<MESSEG_DATA;t++)
			{
				to->u8dat[t]=from->u8dat[t];
			}
		break;
		case MSG_TYPE_U8PTR:
			to->u8ptr=from->u8ptr;
		break;
		case MSG_TYPE_U16PTR:
			to->u16ptr=from->u16ptr;
		break;
		case MSG_TYPE_U32PTR:
			to->u32ptr=from->u32ptr;
		break;
		case MSG_TYPE_VOIDPTR:
			to->voidptr=from->voidptr;
		default:
			break;
	}

}





//��ȡ�������͵��ַ�����ʽ
char *msgErrToStr (u16 errType)
{
	switch (errType)
	{
		case msgNoneErr:
			return "�ɹ�";
		break;
		case msgUnknownErr:
			return "δ֪�Ĵ���";
		break;
		case msgQueueIsFull:
			return "����������ɵĴ���";
		break;
		case msgSameType:
			return "����ͬ����Ϣ�ڶ�����";
		break;
		case msgAddrErr:
			return "Ŀ���ַ����";
		break;
		case msgMoreErr:
			return "������ִ�н����ɵĴ���";
		break;
		default:
			return "��������";
		break;
	}
}



