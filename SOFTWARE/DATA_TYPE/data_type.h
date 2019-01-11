
#ifndef __DATA_TYPE_H
#define __DATA_TYPE_H
#include "includes.h"



/******�����ǻ���������ص���������**********/
 

typedef struct 
{
	u16 collectorId;//�ɼ����ĵ�ַ
	float temperture;
	float Humidity;
	float tvoc;
} EnvirDef; //����ֵ����


typedef struct 
{
	float tempertureH;
	float tempertureL;
	float HumidityH;
	float HumidityL;
	float TVOCH;
	
} CtrllimitDef;//���������޶�





/******����������ص���������End**********/




/*******�豸���������������*************/

typedef struct
{
	u16 devId;					//�豸���
	u8 devType;					//�豸����
	u8 devPower;				//�豸��Դ
	u8 devState;				//�豸״̬
	u8 devCmdState;			//����״̬ ���Զ������еĶ�̬����
	u8 devHand;					//�Ƿ���ֶ�
	u8 channel;					//�豸�ŵ�
	u8 offline;					//����
	u8 devError;				//�Ƿ��д���
	u8 numberOfFailures;//ʧ�ܴ���
	u8 devData;					//�豸��չ����
} DeviceDef;

		//�����豸����
enum
{
	devTypeJZQ=0,
	devTypeCJQ=1,
	devTypeKT=2,
	devTypeCS=3,
	devTypeJH=4,
	devTypeJS=5,
	devTypeYT=6,
};


enum
{
	devPowerOff=0,			//�豸��Դ��
	devPowerOn=1,				//�豸��Դ��
};

enum
{
	devStateNone=0,			//�豸��״̬
	devStateUp=1,				//�豸��
	devStateDown=2,			//�豸��
	devStateOther=3,		//����״̬
};



enum
{
	devHandOff=0,
	devHandOn=1,
};

enum
{
	offlineNo=0,			//����
	offlineYes=1,			//������
};



			//�����豸��������
enum
{
	devErrorLeaking=1,//©ˮ����
	devErrorDevice=2,//�豸����
	devErrorSleep=3,//�豸������
};




typedef struct
{
	u16 ImplementerIP;			//ִ����IP��ַ
	u8 cmdType;					//��������,�Զ����ƣ��ֶ����ƣ���̬�������Ƶ�
	u8 cmdSource;				//������Դ��������λ�������������ֶ�����
	u8 cmdPermissions;	//����Ȩ�� 0�����Ȩ��
	u8 cmdScopes ;			//����������
	u8 cmd[3];					//�������
} CtrlCmdDef;


enum
{
	cmdTypeAuto=0,								//���������Զ�
	cmdTypeHand=1,								//���������ֶ�
	cmdTypeAdjustment=2,					//�������Ͷ�̬����
};

enum
{
	cmdSourceJZQAuto=0,				//�������Զ�
	cmdSourceJZQHand=1,				//�������ֶ�
	cmdSourceServerAuto=2,				//�������Զ�
	cmdSourceServerHand=3,			//�������ֶ�
};

enum
{
	cmdPermissionsAdmin=0,			//����Ա�����Ȩ�޵�����ǿ��ִ��
	cmdPermissionsUser=1,				//�û������Ȩ�޿����ж϶�̬�����е��Զ��豸
	cmdPermissionsAuto=2,						//�Զ������Ȩ�޲��ܿ����Ѿ������Զ�ģʽ���豸
	cmdPermissionsVisitor=3,			//�οͣ����Ȩ��ֻ�ܿ��ƴ����ֶ�ģʽ���豸			
};


enum
{
	cmdScopesOnly=0,				//����ֻ������ִ����
	cmdScopesSame=1,				//������������ִ����ͬ���͵��豸
	cmdScopesJZQ=2,					//���������ڼ�����֮�������豸
};



/********�豸���������������End**********/




/********������ϵͳ������ص���������*******/


typedef struct
{
	u8 myName[20];//��������
	u16 ifUse;				//�Ƿ����ù�
	u16 channel;			//�����ŵ�
	u16 addressDomainL;//��ַ��ͱ߽�
	u16 addressDomainH;//��ַ�򵽱߽�
	u16 autoGetTime;//�Զ���ȡʱ���־
	u16 devExclusiveset;//ÿ���豸��ר����������������
	
	u16 numberOfDevices;//�豸�����������ɼ���
	u16 collectorNumber;//�ɼ�������
	
	u16 alarmInterval; //����ʱ����
	u16 controlInterval;//���Ƽ��
	u16 deployBeginH;//������ʼʱ
	u16 deployBeginM;//������ʼ��
	u16 deployEndH;//��������ʱ
	u16 deployEndM;//����������
	u16 serverMode;//������ģʽ
	u16 offAll;//�ػ�ʱ�ر����������豸
	u16 noticeSynchronous;//�����Ƿ�ͬ����������
	u8  notice[20];//����
	
	u16 screenBrightness;//��Ļ����
	u16 keyboardBrightness;//��������
	u16 alarmBrightness;//��ʾ������
	u16 hibernationTime;//����ʱ��
	u16 alarmModeInNormal;//����ʱ��ʾ��ģʽ��0���أ�1������2����˸
	u16 alarmModeInWarning;//�о���ʱ��ʾ��ģʽ
	u16 alarmModeInError;//�д���ʱ��ʾ��ģʽ
	u16 keyLightMode;//������ģʽ
	
	u16 timeSynchronous;//�Ƿ�ʱ��ͬ��
	
	u8 serverIP[4];//Ĭ�Ϸ�����IP��ַ
	u16 serverCOM;//Ĭ�Ϸ������˿�
	u16 debugMode;//����ģʽ
	u16 postSelfTest;//�����Լ�
	u16 linkToYun;//���ӵ�������
	u16 findServer;//�Ƿ��Զ�Ѱ�ҷ�����
	u16 executingScripts;//�Ƿ�ִ�нű�
	u16 mainCollectorId;//��Ҫ�Ĳɼ���ID
	
	u16 handMode;//�ֶ�ģʽ
} SysCfgDef;



/********ϵͳ������ص���������End**********/





//��ȡϵͳ�����ļ�0,ʧ��
u8 load_sys_cfg(SysCfgDef *buff);
u8 save_sys_cfg(SysCfgDef *buff);
u8 load_dev_cfg(DeviceDef *buff,u8 num);
u8 save_dev_cfg(DeviceDef *buff,u8 num);
u8 load_env_cfg(CtrllimitDef *buff,u8 num);
u8 save_env_cfg(CtrllimitDef *buff,u8 num);

void sys_cfg_init(void);
SysCfgDef *get_syscfg(void);
DeviceDef *get_devcfg(u8 num);
CtrllimitDef *get_envcfg(u8 num);
u8 get_collectorNumber(void);
DeviceDef *get_devcfgbyid(u16 id);
EnvirDef *get_envirbyid (u16 devid);
			//��ȡָ�������豸������
u8 get_DevNumberByType(u8 devtype);
//��ȡָ�������豸��״̬
void get_DevStateByType(u8 devtype,u8 *offline,u8 *power,u8 *state);

u8 get_OnLineDevIdListByType (u8 devtype,u16 *idbuff);

#endif


