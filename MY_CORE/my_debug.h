

#ifndef __MY_DEBUG_H
#define __MY_DEBUG_H

#include "sys.h"
#include "my_w5500.h"
#include "cjson.h"


/***************************************************

					����ʱʹ���������������Ϣ

***************************************************/



extern unsigned char Gateway_IP[4];	//����IP��ַ 
extern unsigned char Sub_Mask[4];	//�������� 
extern unsigned char Phy_Addr[6];	//�����ַ(MAC) 
extern unsigned char IP_Addr[4];	//����IP��ַ 

			//�¿������ϴ���������ַ
extern u8 SERVER_IP[4];
extern u16 SERVER_PORT;


/***************************************************

					����ʱ�ļ�ϵͳ���������Ϣ

***************************************************/


extern u8 DBG_FATS;


/***************************************************

					����ڴ�ռ����Ϣ��Ϣ

***************************************************/

extern u32 memsize    ;							//�ڴ��ܴ�С


/***************************************************

					�����������״̬��Ϣ

***************************************************/

extern u8 DBG_INTER_STATE;


extern u16 SPI_FLASH_TYPE;

extern u16 Get_MyAddr(void);

//��������λ����ͨ��
extern u8 DBG_COPY_TO_S1CK;

//�������ͻ�ȡ����
typedef struct 
{
	u8 objtype;
	u8 *name;
}objname;



//���Ժ����������
void my_debug (void);

void dbg_Interpreter(void);

	//������Ϣ
void dbg_info (void);

void dbg_err (u8 errtype);

void dbg_devconfig(void);

void dbg_help(void);

void dbg_reboot (void);

void dbg_oche (u8 *);

void dbg_booting(void);

	//�������ݵ����Զ˿�
void dbg_copydata (u8 *);


				//ʹ��DNS��ȡ������IP��ַ
void dbg_getip(u8 *buff);


				//����mqtt����
void dbg_mqtt(u8 *buff);





/**********һЩ��������****************/

			//�Ƚ��ַ�������ͬΪ1����ͬΪ0
u8 samestr(u8 *str1,u8 *str2);
				//�ַ���תΪ��������
u16 str2num(u8 *str);

			//��ȡ�ַ����е����֣��÷ָ���point�ֿ�,��length������
u8 getnumfstr(u8 *out,u8 *strin,u8 point,u8 length);


//�����豸���ͻ�ȡ����
u8 *dbg_getdevname(u8 devtype);



#endif



















