
#include "os_sys.h"
#include "includes.h"


/***********************************

		�������ã�ϵͳ��ʼ��
							��ʼ���������
							��ʼ����̬�ڴ����
							��ʼ���ļ�ϵͳ��

**************************************/

			
INT8U os_init (void)
{
	os_mem_init();
	os_task_init();
	os_msg_init();
	return os_fats_init();

}






