#include "bsp_Flash.h"

/**
  * ��    ����FLASH��ȡһ��32λ����
  * ��    ����Address Ҫ��ȡ���ݵ��ֵ�ַ
  * �� �� ֵ��ָ����ַ�µ�����
  */
uint32_t Flash_ReadWord(uint32_t Address)
{
	return *((__IO uint32_t *)(Address));	//ʹ��ָ�����ָ����ַ�µ����ݲ�����
}

/**
  * ��    ����FLASH��ȡһ��16λ�İ���
  * ��    ����Address Ҫ��ȡ���ݵİ��ֵ�ַ
  * �� �� ֵ��ָ����ַ�µ�����
  */
uint16_t Flash_ReadHalfWord(uint32_t Address)
{
	return *((__IO uint16_t *)(Address));	//ʹ��ָ�����ָ����ַ�µ����ݲ�����
}

/**
  * ��    ����FLASH��ȡһ��8λ���ֽ�
  * ��    ����Address Ҫ��ȡ���ݵ��ֽڵ�ַ
  * �� �� ֵ��ָ����ַ�µ�����
  */
uint8_t Flash_ReadByte(uint32_t Address)
{
	return *((__IO uint8_t *)(Address));	//ʹ��ָ�����ָ����ַ�µ����ݲ�����
}

/**
  * ��    ����FLASHȫ����
  * ��    ������
  * �� �� ֵ����
  * ˵    �������ô˺�����FLASH������ҳ���ᱻ���������������ļ����������󣬳��򽫲�������
  */
void Flash_EraseAllPages(void)
{
	FLASH_Unlock();					//����
	FLASH_EraseAllPages();			//ȫ����
	FLASH_Lock();					//����
}

/**
  * ��    ����FLASHҳ����
  * ��    ����PageAddress Ҫ����ҳ��ҳ��ַ
  * �� �� ֵ����
  */
void Flash_ErasePage(uint32_t PageAddress)
{
	FLASH_Unlock();					//����
	FLASH_ErasePage(PageAddress);	//ҳ����
	FLASH_Lock();					//����
}

/**
  * ��    ����FLASH�����
  * ��    ����Address Ҫд�����ݵ��ֵ�ַ
  * ��    ����Data Ҫд���32λ����
  * �� �� ֵ����
  */
void Flash_ProgramWord(uint32_t Address, uint32_t Data)
{
	FLASH_Unlock();							//����
	FLASH_ProgramWord(Address, Data);		//�����
	FLASH_Lock();							//����
}

/**
  * ��    ����FLASH��̰���
  * ��    ����Address Ҫд�����ݵİ��ֵ�ַ
  * ��    ����Data Ҫд���16λ����
  * �� �� ֵ����
  */
void Flash_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
	FLASH_Unlock();							//����
	FLASH_ProgramHalfWord(Address, Data);	//��̰���
	FLASH_Lock();							//����
}
