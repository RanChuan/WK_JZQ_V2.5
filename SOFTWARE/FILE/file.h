#ifndef __FILE_H
#define __FILE_H

#include "sys.h"
#include "fats.h"


//�ļ�֧�ֵ�����С�����������С����Ŀ��ܳ����ڴ治��
#define FILE_MAX_SIZE  4096


u8 read_json(TCHAR *file_name,u8 *buf,u16 buff_size);//��ȡjson�ļ���buf�ǻ�������ַ��buff_size�ǻ�������С

u8 write_config (void);
u8 read_config (void);





#endif








