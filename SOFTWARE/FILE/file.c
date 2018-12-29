
#include "file.h"
#include "fats.h"
#include "rf.h"
#include "string.h"
#include "malloc.h"

//��ȡSD���д洢��������Ϣ��0���ɹ���1��ʧ��
u8 read_config (void)
{
	FRESULT ret;
	UINT real_length=0;
	if (fats_state()==0) return 1;
	ret=FATS->f_open(file,_T("0:/wk_config.bcfg"),FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
	if (ret==FR_OK)
	{
		FATS->f_read(file,EN_CONFIG,CONFIG_DATA_NUM*2,&real_length);
		FATS->f_close(file);
		if (real_length!=CONFIG_DATA_NUM*2) return 3;
	}
	else
	{
		return 2;
	}
	
}


//��ȡSD���д洢��������Ϣ��0���ɹ���1��ʧ��
u8 write_config (void)
{
	FRESULT ret;
	UINT real_length=0;
	if (fats_state()==0) return 1;
	ret=FATS->f_open(file,_T("0:/wk_config.json"),FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
	if (ret==FR_OK)
	{
		FATS->f_write(file,EN_CONFIG,CONFIG_DATA_NUM*2,&real_length);
		FATS->f_close(file);
		if (real_length!=CONFIG_DATA_NUM*2) return 3;
	}
	else
	{
		return 2;
	}
	
}

//�������ã���ȡjson�ļ���ɾ���ļ��е������ַ����ո񣬻س���
//������file_name,Ҫ�򿪵��ļ�����buf�ǻ�������ַ��buff_size�ǻ�������С
//���أ�0���ɹ���1��ʧ��
u8 read_json(TCHAR *file_name,u8 *buf,u16 buff_size)
{
	FRESULT ret;
	FILINFO fileinfo;
	UINT readsize=0;
	if (fats_state()==0) return 1;//��֧���ļ�ϵͳ������
	TCHAR *file_type=file_name+strlen(file_name)-5;
	if (strcmp((const char*)file_type,(const char*)".json")!=0)//�ж�json��ʽ������json��ʽ�ͷ���
	{
		return 1;
	}
	ret=FATS->f_open(file,file_name,FA_READ);
	if (ret!=FR_OK)
	{
		return 1;//��ʧ�ܣ�����
	}
	ret=FATS->f_stat(file_name,&fileinfo);
	if (ret!=FR_OK)
	{
		return 1;//��ȡʧ�ܣ�����
	}
	if (fileinfo.fsize>FILE_MAX_SIZE)//�ļ�̫���ܲ���
	{
		FATS->f_close(file);
		return 1;
	}
	u8 *file_buf=0;
	file_buf=mymalloc(fileinfo.fsize);
	if (file_buf==0)//�ڴ����ʧ�ܣ�����
	{
		FATS->f_close(file);
		return 1;	
	}
	ret=FATS->f_read(file,file_buf,fileinfo.fsize,&readsize);
	if (ret!=FR_OK)
	{
		myfree(file_buf);
		return 1;
	}
	u8 *file_ptr=file_buf;//�ļ�����ָ��
	while (readsize--)
	{
		if (*file_ptr==' ') file_ptr++;
		else if (*file_ptr=='\t') file_ptr++;
		else if (*file_ptr=='\r') file_ptr++;
		else if (*file_ptr=='\n') file_ptr++;
		else
		{
			*buf++=*file_ptr++;
			buff_size--;
			if (buff_size==0) break;//����json�ַ����Ļ��治��
		}
	}
	myfree(file_buf);//�ͷ��ڴ�
	if (readsize!=0)//ת��û�����
	{
		return 1;
	}
	return 0;//ת�����
	
}




