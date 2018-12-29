#ifndef __MY_RF_H
#define __MY_RF_H
#include "sys.h"
#include "rf.h"


/****************************************************************

		EN_CONFIG���20λ�洢IP��ַ�������



*****************************************************************/


//ģʽ2�еĴ������Ͷ���

#define ERR_SUCCESS         0x0000			//�ɹ�
#define ERR_FAIL						0x0001			//δ֪����
#define ERR_DATAVALE				0x0002			//���ݴ���
#define ERR_DATALENGTH			0x0003			//���ݳ��ȴ���
#define ERR_WANTLENGTH			0x0004			//��Ҫ��ȡ�����ݳ��ȴ���
#define ERR_NULLCMD					0x0005			//��֧�ֵ��������ͣ�
#define ERR_NOCONNECT				0x0006			//û�����ӣ�һ�в�����Ҫ��ͨ�ſ�ʼ֮��
#define ERR_NOCJADDR				0x0007			//�ɼ����ĵ�ַ���Ǳ����󶨵Ĳɼ�����ַ
#define ERR_CANNOTCFG				0x0008			//�ڲ��ǿ�����״̬���յ�����������
#define ERR_NOTMYTYPE				0x0009			//��Ҫ���Ƶ��豸�뱾������
#define ERR_TIMEOUT					0x000a   		//��ʱ
#define ERR_OFFLINE         0x000b   		//�豸������
#define ERR_NONEADDR				0x000c      //�豸��ַ����















				//�ҳ��豸����
void Updata_DeviceNum (void);



	//����ָ����ַ���豸����
u8 GetDeviceType (u16 addr);
					//����ָ����ַ���豸״̬
u16 GetDeviceState(u16 addr);
					//����ָ�����͵��豸����
u16 GetDeviceNum (u8 type);
			//���ص�i��type���͵ĵ�ַ
u16 GetNextDeviceaddr (u8 type,u16 i);


				//��ȡ������Ϣ���׵�ַ
u16 *GetCfgData (void);




				//��ȡ�豸��Ϣ��recv�ǽ�������
u16 Cmd_0x01 (u16 addr ,u8 *recv);


					//�����豸
u16 Cmd_0x03 (u16 addr ,u8 type,u8 power,u8 state);

u16 Cmd_0x06 (u16 addr ,u8 state);

u16 Cmd_0x07 (u16 addr ,u8 type);



//������Ϣѭ��
void my_rf_loop (void * t);

				//���ߵ����ݴ���
void my_rf_deal (void * t);


					//�����ֶ���Ϣ
void my_rf_hand (void * t);


void loushui_warn(u16 addr,u8 devtype);

u16 CheckId (u16 addr);//�ж��Ƿ��д��豸id


					//�豸����״ָ̬ʾ�����ͣ���������״̬��������
void Device_state_see(u8 type,u8 power,u8 state);

						//��ȡ�µĿ����豸��״̬
u16 get_newstate(u8 nowpower,u8 key,u16 addr);
							//��ȡһ�������״̬
u16 yt_newstate(u8 nowpower,u8 key,u16 addr);

									//���¼������м�¼���豸״̬
u16 Updata_devicestate(u16 addr ,u8 power,u8 state);





#endif


