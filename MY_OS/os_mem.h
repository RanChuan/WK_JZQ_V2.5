
#include "sys.h"
#include "malloc.h"



					//�ڴ�����ʼ��
void os_mem_init (void);


void myfree(void *ptr);  			//�ڴ��ͷ�
void *mymalloc(u32 size);			//�ڴ����
void *myrealloc(void *ptr,u32 size);//���·����ڴ�


