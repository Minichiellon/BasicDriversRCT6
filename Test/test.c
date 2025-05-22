
#include "test.h"

void Key_Led_test(void)
{
    if(Key_GetNum() == KEY_1_NUM)
    {
        LED_RED_TOGGLE();
    }
    if(Key_GetNum() == KEY_2_NUM)
    {
        LED_BLUE_TOGGLE();
    }
    if(Key_GetNum() == KEY_3_NUM)
    {
        LED_RED_TOGGLE();
        LED_BLUE_TOGGLE();
    }
}

void Usart_test(void)
{
    if(Key_GetNum() == KEY_1_NUM)
	{
        // ����1�������ַ���
        USART1_SendString("����1�������ַ�����Test.\r\n");
    }
    if(Key_GetNum() == KEY_2_NUM)
	{
        // ����2�����͸�����ֵ����, ע�⣬�����������16������ʾ
        uint8_t data[30];             // ��Ҫ���͵����ݻ���
        char *strTemp = "����2��";    // �ַ���
        data[0] = *strTemp++;         // ���ֵ�GBK���룬ʹ��2���ֽڱ�������ʾһ������
        data[1] = *strTemp++;
        data[2] = *strTemp++;
        data[3] = *strTemp++;
        data[4] = *strTemp++;
        data[5] = *strTemp++;
        data[6] = *strTemp++;
        data[7] = 123;                // ���ͣ�ע�⣺�����ASCII��ʽ��ʾ�������ϣ���Ϊ��{��
        data[8] = '5';                // �ַ�
        data[9] = '2';                // �ַ�
        data[10] = '0';               // �ַ�
        data[11] =  5 ;               // ���ͣ�ע�⣺������ASCII����ǿ���ʾ�ķ�Χ�ڣ��ڴ������ϣ���ASCII��ʽ���޷���ʾ. Ҫ��16������ʾ���ܿ�������
        data[12] =  2 ;               // ����
        data[13] =  0 ;               // ����
        data[14] = 0x05;              // ���ͣ�ע�⣺�ܶ���û�����16���ƺ�ʮ���Ƶ����������ݴ����ϣ���û������ģ��·�������ֵ����ͬ���Ϸ�5,2,0������ֵ
        data[15] = 0x02;              // ����
        data[16] = 0x00;              // ����

        USART1_SendData(data, 20);    // USART1_SendData���������Ժ����ط��͸�����������
    }
    
    /******  ����UART���� ******/
    if (xUSART.USART1ReceivedNum)                                    // �ж��Ƿ���յ������ݣ������ֽڳ���>0
    {
        printf("\r<<<<<< UART ���յ������� <<<<<<");                 // ׼�����
        printf("\r ����(�ֽ���)��%d", xUSART.USART1ReceivedNum);     // ����; ��λ:�ֽ�
        printf("\r ����(16����)��");                                 // 16���Ʒ�ʽ��ʾ���ݣ�����۲���ʵ����
        for (uint8_t i = 0; i < xUSART.USART1ReceivedNum; i++)       // ���ÿһ�ֽ���ֵ
            printf("0x%X ", xUSART.USART1ReceivedData[i]);
        printf("\r ����(ASCII) ��%s\r", xUSART.USART1ReceivedData);  // ASCII��ʽ��ʾ���ݣ�����۲��ַ�������;

        xUSART.USART1ReceivedNum = 0;                                // ��Ҫ�������������ˣ��ѽ��������声
    }
}

void Can_test(void)
{
    uint32_t ID = 0x480;
    uint8_t data[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    if(Key_GetNum() == KEY_3_NUM)
	{
        CAN1_SendData(ID, data);
    }
}

void Timer_test(void)
{
    if(TIM2_IrqFlag)
    {
        printf("%d\r\n",TIM2_Num);
        TIM2_IrqFlag = 0;
    }
}

void OLED_test(void)
{
        /*��(0, 0)λ����ʾ�ַ�'A'�������СΪ8*16����*/
        OLED_ShowChar(0, 0, 'A', OLED_8X16);
        
        /*��(16, 0)λ����ʾ�ַ���"Hello World!"�������СΪ8*16����*/
        OLED_ShowString(16, 0, "Hello World!", OLED_8X16);
        
        /*��(0, 18)λ����ʾ�ַ�'A'�������СΪ6*8����*/
        OLED_ShowChar(0, 18, 'A', OLED_6X8);
        
        /*��(16, 18)λ����ʾ�ַ���"Hello World!"�������СΪ6*8����*/
        OLED_ShowString(16, 18, "Hello World!", OLED_6X8);
        
        /*��(0, 28)λ����ʾ����12345������Ϊ5�������СΪ6*8����*/
        OLED_ShowNum(0, 28, 12345, 5, OLED_6X8);
        
        /*��(40, 28)λ����ʾ�з�������-66������Ϊ2�������СΪ6*8����*/
        OLED_ShowSignedNum(40, 28, -66, 2, OLED_6X8);
        
        /*��(70, 28)λ����ʾʮ����������0xA5A5������Ϊ4�������СΪ6*8����*/
        OLED_ShowHexNum(70, 28, 0xA5A5, 4, OLED_6X8);
        
        /*��(0, 38)λ����ʾ����������0xA5������Ϊ8�������СΪ6*8����*/
        OLED_ShowBinNum(0, 38, 0xA5, 8, OLED_6X8);
        
        /*��(60, 38)λ����ʾ��������123.45���������ֳ���Ϊ3��С�����ֳ���Ϊ2�������СΪ6*8����*/
        OLED_ShowFloatNum(60, 38, 123.45, 3, 2, OLED_6X8);
        
        /*��(0, 48)λ����ʾӢ�ĺͺ��ִ�"Hello,���硣"��֧����Ӣ�Ļ�д*/
        OLED_ShowString(0, 48, "Hello,���硣", OLED_8X16);
        
        /*��(96, 48)λ����ʾͼ�񣬿�16���أ���16���أ�ͼ������ΪDiode����*/
        OLED_ShowImage(96, 48, 16, 16, Diode);
        
        /*��(96, 18)λ�ô�ӡ��ʽ���ַ����������СΪ6*8���󣬸�ʽ���ַ���Ϊ"[%02d]"*/
        OLED_Printf(96, 18, OLED_6X8, "[%02d]", 6);
        
        /*����OLED_Update��������OLED�Դ���������ݸ��µ�OLEDӲ��������ʾ*/
        OLED_Update();
        
        /*��ʱ3000ms���۲�����*/
        System_DelayMS(3000);
        
        /*���OLED�Դ�����*/
        OLED_Clear();
        
        /*��(5, 8)λ�û���*/
        OLED_DrawPoint(5, 8);
        
        /*��ȡ(5, 8)λ�õĵ�*/
        if (OLED_GetPoint(5, 8))
        {
            /*���ָ�������������(10, 4)λ����ʾ�ַ���"YES"�������СΪ6*8����*/
            OLED_ShowString(10, 4, "YES", OLED_6X8);
        }
        else
        {
            /*���ָ����δ����������(10, 4)λ����ʾ�ַ���"NO "�������СΪ6*8����*/
            OLED_ShowString(10, 4, "NO ", OLED_6X8);
        }
        
        /*��(40, 0)��(127, 15)λ��֮�仭ֱ��*/
        OLED_DrawLine(40, 0, 127, 15);
        
        /*��(40, 15)��(127, 0)λ��֮�仭ֱ��*/
        OLED_DrawLine(40, 15, 127, 0);
        
        /*��(0, 20)λ�û����Σ���12���أ���15���أ�δ���*/
        OLED_DrawRectangle(0, 20, 12, 15, OLED_UNFILLED);
        
        /*��(0, 40)λ�û����Σ���12���أ���15���أ����*/
        OLED_DrawRectangle(0, 40, 12, 15, OLED_FILLED);
        
        /*��(20, 20)��(40, 25)��(30, 35)λ��֮�仭�����Σ�δ���*/
        OLED_DrawTriangle(20, 20, 40, 25, 30, 35, OLED_UNFILLED);
        
        /*��(20, 40)��(40, 45)��(30, 55)λ��֮�仭�����Σ����*/
        OLED_DrawTriangle(20, 40, 40, 45, 30, 55, OLED_FILLED);
        
        /*��(55, 27)λ�û�Բ���뾶8���أ�δ���*/
        OLED_DrawCircle(55, 27, 8, OLED_UNFILLED);
        
        /*��(55, 47)λ�û�Բ���뾶8���أ����*/
        OLED_DrawCircle(55, 47, 8, OLED_FILLED);
        
        /*��(82, 27)λ�û���Բ���������12���أ��������8���أ�δ���*/
        OLED_DrawEllipse(82, 27, 12, 8, OLED_UNFILLED);
        
        /*��(82, 47)λ�û���Բ���������12���أ��������8���أ����*/
        OLED_DrawEllipse(82, 47, 12, 8, OLED_FILLED);
        
        /*��(110, 18)λ�û�Բ�����뾶15���أ���ʼ�Ƕ�25�ȣ���ֹ�Ƕ�125�ȣ�δ���*/
        OLED_DrawArc(110, 18, 15, 25, 125, OLED_UNFILLED);
        
        /*��(110, 38)λ�û�Բ�����뾶15���أ���ʼ�Ƕ�25�ȣ���ֹ�Ƕ�125�ȣ����*/
        OLED_DrawArc(110, 38, 15, 25, 125, OLED_FILLED);
        
        /*����OLED_Update��������OLED�Դ���������ݸ��µ�OLEDӲ��������ʾ*/
        OLED_Update();
        
        /*��ʱ3000ms���۲�����*/
        System_DelayMS(3000);
        
        
        for (uint8_t i = 0; i < 4; i ++)
		{
			/*��OLED�Դ����鲿������ȡ������(0, i * 16)λ�ÿ�ʼ����128���أ���16����*/
			OLED_ReverseArea(0, i * 16, 128, 16);
			
			/*����OLED_Update��������OLED�Դ���������ݸ��µ�OLEDӲ��������ʾ*/
			OLED_Update();
			
			/*��ʱ1000ms���۲�����*/
			System_DelayMS(3000);
			
			/*��ȡ�������ݷ�ת����*/
			OLED_ReverseArea(0, i * 16, 128, 16);
		}
		
		/*��OLED�Դ�����ȫ������ȡ��*/
		OLED_Reverse();
		
		/*����OLED_Update��������OLED�Դ���������ݸ��µ�OLEDӲ��������ʾ*/
		OLED_Update();
		
		/*��ʱ1000ms���۲�����*/
		System_DelayMS(3000);
        
        /*���OLED�Դ�����*/
        OLED_Clear();
}

void IC_test(void)
{
    static uint16_t delay = 0;
    static uint8_t duty = 50;
    static uint16_t prescaler = 720;
    static uint8_t flash_flag = 0;
    if(flash_flag == 0)
    {
        flash_flag = 1;
        /*��ʾ��̬�ַ���*/
        OLED_ShowString(0, 0, "Freq:00000Hz", OLED_8X16);      //1��1����ʾ�ַ���Freq:00000Hz
        OLED_ShowString(0, 17, "Duty:00%", OLED_8X16);          //2��1����ʾ�ַ���Duty:00%
        OLED_Update();
    }
    if(TIM2_IrqFlag)
    {
        delay++;
        if(delay > 3000)    //3s
        {
            /*ʹ��PWMģ���ṩ���벶��Ĳ����źţ�ÿ3��ı�һ��*/
            PWM_SetPrescaler(prescaler - 1);                  //PWMƵ��Freq = 72M / (PSC + 1) / 100
            prescaler /= 2;
            if(prescaler < 45) prescaler = 720;
            
            PWM_SetCompare1(duty);                        //PWMռ�ձ�Duty = CCR / 100
            duty += 10;
            if(duty > 100)  duty = 0;
        }
        TIM2_IrqFlag = 0;
    }
    OLED_ShowNum(40, 0, IC_GetFreq(), 5, OLED_8X16);    //����ˢ����ʾ���벶���õ�Ƶ��
    OLED_ShowNum(40, 17, IC_GetDuty(), 2, OLED_8X16);    //����ˢ����ʾ���벶���õ�ռ�ձ�
    OLED_Update();
}

void ADC_test(void)
{
    static uint8_t flash_flag = 0;
    if(flash_flag == 0)
    {
        flash_flag = 1;
        /*��ʾ��̬�ַ���*/
        OLED_ShowString(0, 0, "AD0:", OLED_8X16);
        OLED_ShowString(0, 17, "AD1:", OLED_8X16);
        OLED_ShowString(0, 33, "AD2:", OLED_8X16);
        OLED_ShowString(0, 49, "AD3:", OLED_8X16);
        OLED_Update();
    }
    
    OLED_ShowNum(33, 0, AD_Value[0], 4, OLED_8X16);		//��ʾת�������0������
    OLED_ShowNum(33, 17, AD_Value[1], 4, OLED_8X16);		//��ʾת�������1������
    OLED_ShowNum(33, 33, AD_Value[2], 4, OLED_8X16);		//��ʾת�������2������
    OLED_ShowNum(33, 49, AD_Value[3], 4, OLED_8X16);		//��ʾת�������3������
    OLED_Update();
    
    System_DelayMS(100);							//��ʱ100ms���ֶ�����һЩת���ļ��ʱ��
}

void MPU6050_test(void)
{
    uint8_t ID;                             //�������ڴ��ID�ŵı���
    int16_t AX, AY, AZ, GX, GY, GZ;			//�������ڴ�Ÿ������ݵı���
    static uint8_t flash_flag = 0;
    
    if(flash_flag == 0)
    {
        flash_flag = 1;
        /*��ʾ��̬�ַ���*/
        OLED_ShowString(0, 0, "ID:", OLED_8X16);		//��ʾ��̬�ַ���
        ID = MPU6050_GetID();				            //��ȡMPU6050��ID��
        OLED_ShowHexNum(24, 0, ID, 2, OLED_8X16);		//OLED��ʾID��
        OLED_Update();
    }
    MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);		//��ȡMPU6050������
    OLED_ShowSignedNum(0, 16, AX, 5, OLED_8X16);			//OLED��ʾ����
    OLED_ShowSignedNum(0, 32, AY, 5, OLED_8X16);
    OLED_ShowSignedNum(0, 48, AZ, 5, OLED_8X16);
    OLED_ShowSignedNum(56, 16, GX, 5, OLED_8X16);
    OLED_ShowSignedNum(56, 32, GY, 5, OLED_8X16);
    OLED_ShowSignedNum(56, 48, GZ, 5, OLED_8X16);
    OLED_Update();
}

void W25Q64_test(void)
{
    uint8_t MID;                            //�������ڴ��MID�ŵı���
    uint16_t DID;                            //�������ڴ��DID�ŵı���

    uint8_t ArrayWrite[] = {0x01, 0x02, 0x03, 0x04};    //����Ҫд�����ݵĲ�������
    uint8_t ArrayRead[4];                                //����Ҫ��ȡ���ݵĲ�������
    static uint8_t flash_flag = 0;
    
    if(flash_flag == 0)
    {
        flash_flag = 1;
        /*��ʾ��̬�ַ���*/
        OLED_ShowString(0, 0, "MID:   DID:", OLED_8X16);
        OLED_ShowString(0, 16, "W:", OLED_8X16);
        OLED_ShowString(0, 32, "R:", OLED_8X16);
        
        /*��ʾID��*/
        W25Q64_ReadID(&MID, &DID);            //��ȡW25Q64��ID��
        OLED_ShowHexNum(32, 0, MID, 2, OLED_8X16);        //��ʾMID
        OLED_ShowHexNum(88, 0, DID, 4, OLED_8X16);        //��ʾDID
        
        /*W25Q64���ܺ�������*/
        W25Q64_SectorErase(0x000000);                    //��������
        W25Q64_PageProgram(0x000000, ArrayWrite, 4);    //��д�����ݵĲ�������д�뵽W25Q64��
        
        W25Q64_ReadData(0x000000, ArrayRead, 4);        //��ȡ��д��Ĳ������ݵ���ȡ���ݵĲ���������
        
        /*��ʾ����*/
        OLED_ShowHexNum(16, 16, ArrayWrite[0], 2, OLED_8X16);        //��ʾд�����ݵĲ�������
        OLED_ShowHexNum(40, 16, ArrayWrite[1], 2, OLED_8X16);
        OLED_ShowHexNum(64, 16, ArrayWrite[2], 2, OLED_8X16);
        OLED_ShowHexNum(88, 16, ArrayWrite[3], 2, OLED_8X16);
        
        OLED_ShowHexNum(16, 32, ArrayRead[0], 2, OLED_8X16);            //��ʾ��ȡ���ݵĲ�������
        OLED_ShowHexNum(40, 32, ArrayRead[1], 2, OLED_8X16);
        OLED_ShowHexNum(64, 32, ArrayRead[2], 2, OLED_8X16);
        OLED_ShowHexNum(88, 32, ArrayRead[3], 2, OLED_8X16);
        OLED_Update();
    }
}

void Flash_test(void)
{
    static uint32_t STORE_START_ADDRESS = 0x0800FC00;		//�洢����ʼ��ַ
    static uint16_t STORE_COUNT = 512;				//�洢���ݵĸ���
    static uint16_t Store_Data[512];				//����SRAM����
    
    static uint8_t flash_flag = 0;
    uint8_t KeyNum = Key_GetNum();
    
    if(flash_flag == 0)
    {
        flash_flag = 1;
        /*��ʾ��̬�ַ���*/
        OLED_ShowString(0, 0, "Flag:", OLED_8X16);
        OLED_ShowString(0, 16, "Data:", OLED_8X16);
        OLED_Update();
        
        /*�ж��ǲ��ǵ�һ��ʹ��*/
        if (Flash_ReadHalfWord(STORE_START_ADDRESS) != 0xA5A5)	//��ȡ��һ�����ֵı�־λ��if��������ִ�е�һ��ʹ�õĳ�ʼ��
        {
            Flash_ErasePage(STORE_START_ADDRESS);					//����ָ��ҳ
            Flash_ProgramHalfWord(STORE_START_ADDRESS, 0xA5A5);	//�ڵ�һ������д���Լ��涨�ı�־λ�������ж��ǲ��ǵ�һ��ʹ��
            for (uint16_t i = 1; i < STORE_COUNT; i ++)				//ѭ��STORE_COUNT�Σ����˵�һ����־λ
            {
                Flash_ProgramHalfWord(STORE_START_ADDRESS + i * 2, 0x0000);		//���˱�־λ����Ч����ȫ����0
            }
        }
        
        /*�ϵ�ʱ�����������ݼ��ػ�SRAM���飬ʵ��SRAM����ĵ��粻��ʧ*/
        for (uint16_t i = 0; i < STORE_COUNT; i ++)					//ѭ��STORE_COUNT�Σ�������һ����־λ
        {
            Store_Data[i] = Flash_ReadHalfWord(STORE_START_ADDRESS + i * 2);		//����������ݼ��ػ�SRAM����
        }
    }
    
    if(KeyNum == KEY_1_NUM)
    {
        Store_Data[1] ++;		//�任��������
        Store_Data[2] += 2;
        Store_Data[3] += 3;
        Store_Data[4] += 4;
        
        Flash_ErasePage(STORE_START_ADDRESS);				//����ָ��ҳ
        for (uint16_t i = 0; i < STORE_COUNT; i ++)			//ѭ��STORE_COUNT�Σ�������һ����־λ
        {
            Flash_ProgramHalfWord(STORE_START_ADDRESS + i * 2, Store_Data[i]);	//��SRAM��������ݱ��ݱ��浽����
        }
    }
    
    if(Key_GetNum() == KEY_2_NUM)
    {
        for (uint16_t i = 1; i < STORE_COUNT; i ++)			//ѭ��STORE_COUNT�Σ����˵�һ����־λ
        {
            Store_Data[i] = 0x0000;							//SRAM������Ч������0
        }
        
    	Flash_ErasePage(STORE_START_ADDRESS);				//����ָ��ҳ
        for (uint16_t i = 0; i < STORE_COUNT; i ++)			//ѭ��STORE_COUNT�Σ�������һ����־λ
        {
            Flash_ProgramHalfWord(STORE_START_ADDRESS + i * 2, Store_Data[i]);	//��SRAM��������ݱ��ݱ��浽����
        }
    }
    
    OLED_ShowHexNum(40, 0, Store_Data[0], 4, OLED_8X16);	//��ʾStore_Data�ĵ�һλ��־λ
    OLED_ShowHexNum(0, 32, Store_Data[1], 4, OLED_8X16);	//��ʾStore_Data����Ч�洢����
    OLED_ShowHexNum(40, 32, Store_Data[2], 4, OLED_8X16);
    OLED_ShowHexNum(0, 48, Store_Data[3], 4, OLED_8X16);
    OLED_ShowHexNum(40, 48, Store_Data[4], 4, OLED_8X16);
    OLED_Update();
}
