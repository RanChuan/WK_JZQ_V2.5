#include "my_messeg.h"



	//�Ѵ���ת��Ϊ�ַ���
char *err_to_str(u16 err)
{
	switch (err)
	{
		case ERR_SUCCESS:
			return "�ɹ�";
		break;
		case ERR_FAIL:
			return "δ֪����";
		break;
		case ERR_DATAVALE:
			return "���ݴ���";
		break;
		case ERR_DATALENGTH:
			return "���ݳ��ȴ���";
		break;
		case ERR_WANTLENGTH:
			return "��Ҫ��ȡ�����ݳ��ȴ���";
		break;
		case ERR_NULLCMD:
			return "��֧�ֵ���������";
		break;
		case ERR_NOCONNECT:
			return "û������";
		break;
		case ERR_NOCJADDR:
			return "�ɼ����ĵ�ַ���Ǳ����󶨵Ĳɼ�����ַ";
		break;
		case ERR_CANNOTCFG:
			return "�ڲ��ǿ�����״̬���յ�����������";
		break;
		case ERR_NOTMYTYPE:
			return "��Ҫ���Ƶ��豸�뱾������";
		break;
		case ERR_TIMEOUT:
			return "��ʱ";
		break;
		case ERR_OFFLINE:
			return "�豸������";
		break;
		case ERR_NONEADDR:
			return "�豸��ַ����";
		break;
		case ERR_ADDRCROSS:
			return "����豸ʱ��ַԽ��";
		break;
		default:
			return "��������";
		break;
	}
}




