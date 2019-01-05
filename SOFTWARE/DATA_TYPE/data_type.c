
#include "includes.h"
#include "data_type.h"


#include "flash.h"

/********************************

		����ļ�����ʹ�ö�̬�����ڴ����������

***********************************/


//��ȡϵͳ�����ļ�,����0ʧ��
u8 load_sys_cfg(SysCfgDef *buff)
{ 
	if ((SPI_FLASH_TYPE==W25Q80)||(SPI_FLASH_TYPE==W25Q16)||(SPI_FLASH_TYPE==W25Q32)||(SPI_FLASH_TYPE==W25Q64)||(SPI_FLASH_TYPE==W25Q128))
	{
		SPI_Flash_Read((u8*)buff,0,sizeof(SysCfgDef));//��ַ0
		return 1;
	}
	else
	{
		return 0;
	}
	
}
 
u8 load_dev_cfg(DeviceDef **buff,u8 num)
{
	if ((SPI_FLASH_TYPE==W25Q80)||(SPI_FLASH_TYPE==W25Q16)||(SPI_FLASH_TYPE==W25Q32)||(SPI_FLASH_TYPE==W25Q64)||(SPI_FLASH_TYPE==W25Q128))
	{
		SPI_Flash_Read((u8*)buff,1024,sizeof(DeviceDef)*num);//��ַ0
		return 1;
	}
	else
	{
		return 0;
	}

}


u8 load_env_cfg(CtrllimitDef **buff,u8 num)
{
	if ((SPI_FLASH_TYPE==W25Q80)||(SPI_FLASH_TYPE==W25Q16)||(SPI_FLASH_TYPE==W25Q32)||(SPI_FLASH_TYPE==W25Q64)||(SPI_FLASH_TYPE==W25Q128))
	{
		SPI_Flash_Read((u8*)buff,2048,sizeof(CtrllimitDef)*num);//��ַ0
		return 1;
	}
	else
	{
		return 0;
	}

}







SysCfgDef *SYS_CFG=0;//ϵͳ����
DeviceDef **DEV_CFG=0;//�豸��������
CtrllimitDef **ENV_CFG=0;//������������
EnvirDef **ENV_VALUE=0;//�ɼ�������ֵ

void sys_cfg_init(void)
{
	SYS_CFG=mymalloc(sizeof(SysCfgDef));
	load_sys_cfg(SYS_CFG);
	
	for (u16 i=0;i<sizeof(SysCfgDef);i++)
	{
		if (((u8*)SYS_CFG)[i]==0xff)
		{
			((u8*)SYS_CFG)[i]=0;
		}
	}
	if (SYS_CFG->numberOfDevices==0)
	{
		SYS_CFG->numberOfDevices=20;
	}
	
	if ((SYS_CFG->numberOfDevices<=20))//�Ѿ����ù�����ȡ������Ϣ
	{
		DEV_CFG=mymalloc(sizeof(DeviceDef)*SYS_CFG->numberOfDevices);
		ENV_CFG=mymalloc(sizeof(CtrllimitDef)*SYS_CFG->numberOfDevices);
		load_dev_cfg(DEV_CFG,SYS_CFG->numberOfDevices);
		for (u16 i=0;i<sizeof(DeviceDef)*SYS_CFG->numberOfDevices;i++)
		{
			if (((u8*)DEV_CFG)[i]==0xff)
			{
				((u8*)DEV_CFG)[i]=0;
			}
		}
		for (u16 i=0;i<sizeof(CtrllimitDef)*SYS_CFG->numberOfDevices;i++)
		{
			if (((u8*)ENV_CFG)[i]==0xff)
			{
				((u8*)ENV_CFG)[i]=0;
			}
		}
	}
	if (SYS_CFG->ifUse==0)
	{
		SYS_CFG->numberOfDevices=0;
	}
	
	SYS_CFG->collectorNumber=get_collectorNumber();
	ENV_VALUE=mymalloc(sizeof(EnvirDef)*SYS_CFG->collectorNumber);//�ɼ�������ֵ����
	
	for (u16 i=0;i<sizeof(EnvirDef)*SYS_CFG->collectorNumber;i++)
	{
		if (((u8*)ENV_VALUE)[i]==0xff)
		{
			((u8*)ENV_VALUE)[i]=0;
		}
	}
	
}


u8 save_sys_cfg(SysCfgDef *buff)
{
	if ((SPI_FLASH_TYPE==W25Q80)||(SPI_FLASH_TYPE==W25Q16)||(SPI_FLASH_TYPE==W25Q32)||(SPI_FLASH_TYPE==W25Q64)||(SPI_FLASH_TYPE==W25Q128))
	{
		SPI_Flash_Write((u8*)buff,0,sizeof(SysCfgDef));//��ַ0
		return 1;
	}
	else
	{
		return 0;
	}
	
}


u8 save_dev_cfg(DeviceDef **buff,u8 num)
{
	if ((SPI_FLASH_TYPE==W25Q80)||(SPI_FLASH_TYPE==W25Q16)||(SPI_FLASH_TYPE==W25Q32)||(SPI_FLASH_TYPE==W25Q64)||(SPI_FLASH_TYPE==W25Q128))
	{
		SPI_Flash_Write((u8*)buff,1024,sizeof(DeviceDef)*num);//��ַ0
		return 1;
	}
	else
	{
		return 0;
	}

}


u8 save_env_cfg(CtrllimitDef **buff,u8 num)
{
	if ((SPI_FLASH_TYPE==W25Q80)||(SPI_FLASH_TYPE==W25Q16)||(SPI_FLASH_TYPE==W25Q32)||(SPI_FLASH_TYPE==W25Q64)||(SPI_FLASH_TYPE==W25Q128))
	{
		SPI_Flash_Write((u8*)buff,1024,sizeof(CtrllimitDef)*num);//��ַ0
		return 1;
	}
	else
	{
		return 0;
	}

}




			//��ȡϵͳ����
SysCfgDef *get_syscfg(void)
{
	return SYS_CFG;
}

			//��ȡָ��λ�õ��豸����,���������������жϣ�
DeviceDef *get_devcfg(u8 num)
{
	u8 *data=(u8 *)DEV_CFG;
	return (DeviceDef *)(data+sizeof(DeviceDef)*num);
}


			//��ȡָ����ַ���豸����
DeviceDef *get_devcfgbyid(u16 id)
{
	for (u8 i=0;i<get_syscfg()->numberOfDevices;i++)
	{
		if (get_devcfg(i)->devId==id)
		{
			return get_devcfg(i);
		}
	}
}


			//��ȡָ��λ�õĻ�������
CtrllimitDef *get_envcfg(u8 num)
{
	u8 *data=(u8 *)ENV_CFG;
	return (CtrllimitDef *)(data+sizeof(CtrllimitDef)*num);
}

			//��ȡ�ɼ���������
u8 get_collectorNumber(void)
{
	u8 collector_num=0;
	for (u8 i=0;i<get_syscfg()->numberOfDevices;i++)
	{
		if (get_devcfg(i)->devType==devTypeCJQ)
		{
			collector_num++;
		}
	}
	return collector_num;
}



			//��ȡָ�������豸������
u8 get_DevNumberByType(u8 devtype)
{
	u8 collector_num=0;
	for (u8 i=0;i<get_syscfg()->numberOfDevices;i++)
	{
		if (get_devcfg(i)->devType==devtype)
		{
			collector_num++;
		}
	}
	return collector_num;
}

//��ȡָ�������豸��״̬
void get_DevStateByType(u8 devtype,u8 *offline,u8 *power,u8 *state)
{
	u8 collector_num=0;
	//*offline=offlineYes;
	for (u8 i=0;i<get_syscfg()->numberOfDevices;i++)
	{
		if (get_devcfg(i)->devType==devtype)
		{
			if (*offline==offlineYes)
			{
				*offline=get_devcfg(i)->offline;
			}
			if (*offline==offlineNo)
			{
				*power=get_devcfg(i)->devPower;
				*state=get_devcfg(i)->devState;
			}
		}
	}
}



//����ID��ȡ����ֵ�ṹ�壬��û�е�������Զ����
EnvirDef *get_envirbyid (u16 devid)
{
	for (u8 i=0;i<get_syscfg()->collectorNumber;i++)
	{
		if (ENV_VALUE[i]->collectorId==devid)
		{
			return ENV_VALUE[i];
		}
		if (ENV_VALUE[i]->collectorId==0)
		{
			ENV_VALUE[i]->collectorId=devid;
			return ENV_VALUE[i];
		}
	}
	
}



