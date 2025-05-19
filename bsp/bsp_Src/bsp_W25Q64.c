#include "bsp_SPI.h"
#include "bsp_W25Q64.h"
#include "bsp_W25Q64_Ins.h"

/**
  * ��    ����W25Q64��ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void W25Q64_Init(void)
{
    SPI1_Init();
}

/**
  * ��    ����MPU6050��ȡID��
  * ��    ����MID ����ID��ʹ�������������ʽ����
  * ��    ����DID �豸ID��ʹ�������������ʽ����
  * �� �� ֵ����
  */
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
    SPI1_Start();                                   //SPI��ʼ
    SPI1_SwapByte(W25Q64_JEDEC_ID);                 //�������Ͷ�ȡID��ָ��
    *MID = SPI1_SwapByte(W25Q64_DUMMY_BYTE);        //��������MID��ͨ�������������
    *DID = SPI1_SwapByte(W25Q64_DUMMY_BYTE);        //��������DID��8λ
    *DID <<= 8;                                     //��8λ�Ƶ���λ
    *DID |= SPI1_SwapByte(W25Q64_DUMMY_BYTE);       //���Ͻ�������DID�ĵ�8λ��ͨ�������������
    SPI1_Stop();                                    //SPI��ֹ
}

/**
  * ��    ����W25Q64дʹ��
  * ��    ������
  * �� �� ֵ����
  */
void W25Q64_WriteEnable(void)
{
    SPI1_Start();                                   //SPI��ʼ
    SPI1_SwapByte(W25Q64_WRITE_ENABLE);             //��������дʹ�ܵ�ָ��
    SPI1_Stop();                                    //SPI��ֹ
}

/**
  * ��    ����W25Q64�ȴ�æ
  * ��    ������
  * �� �� ֵ����
  */
void W25Q64_WaitBusy(void)
{
    uint32_t Timeout;
    SPI1_Start();                                   //SPI��ʼ
    SPI1_SwapByte(W25Q64_READ_STATUS_REGISTER_1);   //�������Ͷ�״̬�Ĵ���1��ָ��
    Timeout = 100000;                               //������ʱ����ʱ��
    while ((SPI1_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)    //ѭ���ȴ�æ��־λ
    {
        Timeout --;                                 //�ȴ�ʱ������ֵ�Լ�
        if (Timeout == 0)                           //�Լ���0�󣬵ȴ���ʱ
        {
            /*��ʱ�Ĵ�������룬������ӵ��˴�*/
            break;                                  //�����ȴ���������
        }
    }
    SPI1_Stop();                                    //SPI��ֹ
}

/**
  * ��    ����W25Q64ҳ���
  * ��    ����Address ҳ��̵���ʼ��ַ����Χ��0x000000~0x7FFFFF
  * ��    ����DataArray    ����д�����ݵ�����
  * ��    ����Count Ҫд�����ݵ���������Χ��0~256
  * �� �� ֵ����
  * ע�����д��ĵ�ַ��Χ���ܿ�ҳ
  */
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
    uint16_t i;
    
    W25Q64_WriteEnable();                       //дʹ��
    
    SPI1_Start();                               //SPI��ʼ
    SPI1_SwapByte(W25Q64_PAGE_PROGRAM);         //��������ҳ��̵�ָ��
    SPI1_SwapByte(Address >> 16);               //�������͵�ַ23~16λ
    SPI1_SwapByte(Address >> 8);                //�������͵�ַ15~8λ
    SPI1_SwapByte(Address);                     //�������͵�ַ7~0λ
    for (i = 0; i < Count; i ++)                //ѭ��Count��
    {
        SPI1_SwapByte(DataArray[i]);            //��������ʼ��ַ��д������
    }
    SPI1_Stop();                                //SPI��ֹ
    
    W25Q64_WaitBusy();                          //�ȴ�æ
}

/**
  * ��    ����W25Q64����������4KB��
  * ��    ����Address ָ�������ĵ�ַ����Χ��0x000000~0x7FFFFF
  * �� �� ֵ����
  */
void W25Q64_SectorErase(uint32_t Address)
{
    W25Q64_WriteEnable();                       //дʹ��
    
    SPI1_Start();                               //SPI��ʼ
    SPI1_SwapByte(W25Q64_SECTOR_ERASE_4KB);     //������������������ָ��
    SPI1_SwapByte(Address >> 16);               //�������͵�ַ23~16λ
    SPI1_SwapByte(Address >> 8);                //�������͵�ַ15~8λ
    SPI1_SwapByte(Address);                     //�������͵�ַ7~0λ
    SPI1_Stop();                                //SPI��ֹ
    
    W25Q64_WaitBusy();                          //�ȴ�æ
}

/**
  * ��    ����W25Q64��ȡ����
  * ��    ����Address ��ȡ���ݵ���ʼ��ַ����Χ��0x000000~0x7FFFFF
  * ��    ����DataArray ���ڽ��ն�ȡ���ݵ����飬ͨ�������������
  * ��    ����Count Ҫ��ȡ���ݵ���������Χ��0~0x800000
  * �� �� ֵ����
  */
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
    uint32_t i;
    SPI1_Start();                               //SPI��ʼ
    SPI1_SwapByte(W25Q64_READ_DATA);            //�������Ͷ�ȡ���ݵ�ָ��
    SPI1_SwapByte(Address >> 16);               //�������͵�ַ23~16λ
    SPI1_SwapByte(Address >> 8);                //�������͵�ַ15~8λ
    SPI1_SwapByte(Address);                     //�������͵�ַ7~0λ
    for (i = 0; i < Count; i ++)                //ѭ��Count��
    {
        DataArray[i] = SPI1_SwapByte(W25Q64_DUMMY_BYTE);    //��������ʼ��ַ���ȡ����
    }
    SPI1_Stop();                                //SPI��ֹ
}
