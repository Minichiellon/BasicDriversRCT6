#include "test.h"                   // ���Ժ���

void bsp_init(void)
{
    uint16_t CAN1_baudrate = 250;
    USART1_Init(115200);        // ���ڳ�ʼ��        //TODO������2��3��4��5�����дΪ�⺯������DMA
    System_SwdMode();           // ����оƬ���Է�ʽ(SWD); �ر�JTAGֻ����SWD; Ŀ��:�ͷ�PB3��PB4��PA15��ֻ��PA13��PA14
    Led_Init();                 // LED��ʼ��
    Key_Init();                 // KEY��ʼ��
    Timer_Init();               // ��ʱ����ʼ��
    OLED_Init();                // OLED��ʼ��      //8*16��С��A��B�����꣺�� (B-1)*8, (A-1)*16 ��
//    PWM_Init();                 // PWM��ʼ��
//    IC_Init();                  // ���벶���ʼ��
    AD_Init();                  // ADC��ʼ��
    MPU6050_Init();		        // MPU6050��ʼ��
    W25Q64_Init();              // W25Q64��ʼ��        //TODO��W25Qϵ��оƬͳһ
    W25qx_Init();               // �豸W25Q128���ⲿFLASH, 16M�ռ䣬ǰ10M�û�ʹ�ã���6M������ģ����)
    LCD_Init();                 // ��ʾ����ʼ��
    //�������ò����ʷ�������λ��һֱ����ascii��������֣��磺125��250�ȣ�Ȼ�󰴸�λ
	//�����ʹ�ô������ò����ʵĻ���ֱ���޸�����ı���
	if(xUSART.USART1ReceivedNum > 0)
	{
		CAN1_baudrate = (xUSART.USART1ReceivedData[0]-'0')*100 + (xUSART.USART1ReceivedData[1]-'0')*10 + xUSART.USART1ReceivedData[2]-'0';
		xUSART.USART1ReceivedNum = 0;
	}
    CAN1_Config(CAN1_baudrate);                                   // CAN1��ʼ��
}

void bsp_test(void)
{
//    Key_Led_test();
//    CheckKeyEvent(KEY_2);
//    Usart_test();
//    Can_test();
//    Timer_test();
//    OLED_test();
//    IC_test();
//    ADC_test();
//    MPU6050_test();
//    W25Q64_test(); 
//    Flash_test();
}
