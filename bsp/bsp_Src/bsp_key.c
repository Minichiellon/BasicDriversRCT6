#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "system_f103.h"

static uint8_t KeyPressFlag;        //����1��2��3���±�־λ

static uint8_t buttonValue;                //������ƽ״̬
static uint8_t keyPressStatus = RESET;     //��������ʱ�ĵ�ƽ״̬
static uint8_t keyReleaseStatus = SET;     //�����ͷ�ʱ�ĵ�ƽ״̬
static uint8_t pressTimes = 0;             //��������
static ButtonState currentState = IDLE;    //״̬������
static uint16_t pressStartTime = 0;        //��������ʱ��ʱ��
static uint16_t releaseStartTime = 0;      //�����ͷ�ʱ��ʱ��

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;    // ���ã��������Ź���ģʽ
    NVIC_InitTypeDef NVIC_InitStruct ;      // ���ã��������ȼ�
    EXTI_InitTypeDef EXTI_InitStruct ;      // ���ã����������жϷ�ʽ

    // ʹ��AFIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE) ;      // EXTI��ʱ��Ҫ����AFIO�Ĵ���

    // ʹ��GPIOʱ��, Ϊ���ٵ��Թ�������ʹ��ʱ�Ӷ����������GPIO�˿�ʱ�ӣ���ʹ���ˣ�
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF, ENABLE);

    // KEY_1
    // �������Ź���ģʽ
    GPIO_InitStructure.GPIO_Pin  = KEY_1_PIN;                  // ����KEY_1, ��ʱ����(��Ҫ)�� ����ʱ���øߵ�ƽ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ;             // ���Ź���ģʽ; ��ʱ��ƽ״̬(ʹ��оƬ�ڲ�������е�ƽ������)
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;         // �����ƽ��ת�ٶȣ�������״̬ʱ��Ч����GPIO_Init������Ҫ�õ���
    GPIO_Init(KEY_1_GPIO, &GPIO_InitStructure);                // ��ʼ�����������Ź���ģʽ���ú���
    // �����жϵ����ȼ�
    NVIC_InitStruct.NVIC_IRQChannel = KEY_1_IRQN ;             // �жϺ�
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1 ;    // ������ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0 ;           // ���������ȼ�
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE ;              // ʹ���ж�ͨ��
    NVIC_Init(&NVIC_InitStruct) ;                              // ��ʼ�����������ȼ����ú���
    // �����жϵķ�ʽ
    GPIO_EXTILineConfig(KEY_1_GPIOSOURCE, KEY_1_PINSOURCE);    // ѡ����ΪEXTI�ߵ�GPIO����
    EXTI_InitStruct.EXTI_Line    = KEY_1_EXTI_LINE ;           // �����ж�or�¼���
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt ;       // ģʽ���жϣ�EXTI_Mode_Interrupt���¼���EXTI_Mode_Event
    EXTI_InitStruct.EXTI_Trigger = KEY_1_TRIM ;                // ���ش����� ������EXTI_Trigger_Rising ���½���EXTI_Trigger_Falling �����գ�EXTI_Trigger_Rising_Falling
    EXTI_InitStruct.EXTI_LineCmd = ENABLE ;                    // ʹ��EXTI��
    EXTI_Init(&EXTI_InitStruct) ;                              // ��ʼ���������ж������ú���

    // KEY_2
    // �������Ź���ģʽ
    GPIO_InitStructure.GPIO_Pin  = KEY_2_PIN;                  // ����KEY_1, ��ʱ����(��Ҫ)�� ����ʱ���øߵ�ƽ
    GPIO_InitStructure.GPIO_Mode = KEY_2_MODE ;                // ���Ź���ģʽ; ��ʱ��ƽ״̬(ʹ��оƬ�ڲ�������е�ƽ������)
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;         // �����ƽ��ת�ٶȣ�������״̬ʱ��Ч����GPIO_Init������Ҫ�õ���
    GPIO_Init(KEY_2_GPIO, &GPIO_InitStructure);                // ��ʼ�����������Ź���ģʽ���ú���
    // �����жϵ����ȼ�
    NVIC_InitStruct.NVIC_IRQChannel = KEY_2_IRQN ;             // �жϺ�
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1 ;    // ������ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0 ;           // ���������ȼ�
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE ;              // ʹ���ж�ͨ��
    NVIC_Init(&NVIC_InitStruct) ;                              // ��ʼ�����������ȼ����ú���
    // �����жϵķ�ʽ
    GPIO_EXTILineConfig(KEY_2_GPIOSOURCE, KEY_2_PINSOURCE);    // ѡ����ΪEXTI�ߵ�GPIO����
    EXTI_InitStruct.EXTI_Line    = KEY_2_EXTI_LINE ;           // �����ж�or�¼���
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt ;       // ģʽ���жϣ�EXTI_Mode_Interrupt���¼���EXTI_Mode_Event
    EXTI_InitStruct.EXTI_Trigger = KEY_2_TRIM ;                // ���ش����� ������EXTI_Trigger_Rising ���½���EXTI_Trigger_Falling �����գ�EXTI_Trigger_Rising_Falling
    EXTI_InitStruct.EXTI_LineCmd = ENABLE ;                    // ʹ��EXTI��
    EXTI_Init(&EXTI_InitStruct) ;                              // ��ʼ���������ж������ú���

    // KEY_3
    // �������Ź���ģʽ
    GPIO_InitStructure.GPIO_Pin  = KEY_3_PIN;                  // ����KEY_1, ��ʱ����(��Ҫ)�� ����ʱ���øߵ�ƽ
    GPIO_InitStructure.GPIO_Mode = KEY_3_MODE ;                // ���Ź���ģʽ; ��ʱ��ƽ״̬(ʹ��оƬ�ڲ�������е�ƽ������)
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;         // �����ƽ��ת�ٶȣ�������״̬ʱ��Ч����GPIO_Init������Ҫ�õ���
    GPIO_Init(KEY_3_GPIO, &GPIO_InitStructure);                // ��ʼ�����������Ź���ģʽ���ú���
    // �����жϵ����ȼ�
    NVIC_InitStruct.NVIC_IRQChannel = KEY_3_IRQN ;             // �жϺ�
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1 ;    // ������ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0 ;           // ���������ȼ�
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE ;              // ʹ���ж�ͨ��
    NVIC_Init(&NVIC_InitStruct) ;                              // ��ʼ�����������ȼ����ú���
    // �����жϵķ�ʽ
    GPIO_EXTILineConfig(KEY_3_GPIOSOURCE, KEY_3_PINSOURCE);    // ѡ����ΪEXTI�ߵ�GPIO����
    EXTI_InitStruct.EXTI_Line    = KEY_3_EXTI_LINE ;           // �����ж�or�¼���
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt ;       // ģʽ���жϣ�EXTI_Mode_Interrupt���¼���EXTI_Mode_Event
    EXTI_InitStruct.EXTI_Trigger = KEY_3_TRIM ;                // ���ش����� ������EXTI_Trigger_Rising ���½���EXTI_Trigger_Falling �����գ�EXTI_Trigger_Rising_Falling
    EXTI_InitStruct.EXTI_LineCmd = ENABLE ;                    // ʹ��EXTI��
    EXTI_Init(&EXTI_InitStruct) ;                              // ��ʼ���������ж������ú���
}

/**
  * ��    ����������ȡ����
  * ��    ������
  * �� �� ֵ�����°����ļ���ֵ����Χ��1~3������0����û�а�������
  */
uint8_t Key_GetNum(void)
{
    uint8_t KeyNum;

    if(KeyPressFlag == KEY_1_PRESS)
    {
        KeyNum = KEY_1_NUM;
        KeyPressFlag = 0;
    }

    if(KeyPressFlag == KEY_2_PRESS)
    {
        KeyNum = KEY_2_NUM;
        KeyPressFlag = 0;
    }

    if(KeyPressFlag == KEY_3_PRESS)
    {
        KeyNum = KEY_3_NUM;
        KeyPressFlag = 0;
    }

    return KeyNum;			//���ؼ���ֵ�����û�а������£�����if���������������ΪĬ��ֵ0
}

// KEY_1 �жϷ�����
void KEY_1_IRQHANDLER(void)                        		// ��ʾ���������������h�ļ��еĺ궨�壬�ڱ�������У��ᱻ�滻�ɺ궨���ֵ
{
    if (EXTI_GetITStatus(KEY_1_EXTI_LINE))          	// �����ϵİ�����ʹ�õ������򵥵�Ӳ������,������ʹ�������ʱ����
    {
        EXTI_ClearITPendingBit(KEY_1_EXTI_LINE);    	// �����жϱ�ʾ
        KeyPressFlag = KEY_1_PRESS;
    }
}



// KEY_2 �жϷ�����
void KEY_2_IRQHANDLER(void)                        		// ��ʾ���������������h�ļ��еĺ궨�壬�ڱ�������У��ᱻ�滻�ɺ궨���ֵ
{
    if (EXTI_GetITStatus(KEY_2_EXTI_LINE))				// �����ϵİ�����ʹ�õ������򵥵�Ӳ������,������ʹ�������ʱ����
    {
        EXTI_ClearITPendingBit(KEY_2_EXTI_LINE);		// �����жϱ�ʾ
        KeyPressFlag = KEY_2_PRESS;
    }
}



// KEY_3 �жϷ�����
void KEY_3_IRQHANDLER(void)                        		// ��ʾ���������������h�ļ��еĺ궨�壬�ڱ�������У��ᱻ�滻�ɺ궨���ֵ
{
    if (EXTI_GetITStatus(KEY_3_EXTI_LINE))              // �����ϵİ�����ʹ�õ������򵥵�Ӳ������,������ʹ�������ʱ����
    {
        EXTI_ClearITPendingBit(KEY_3_EXTI_LINE);        // �����жϱ�ʾ
        KeyPressFlag = KEY_3_PRESS;
    }
}

void CheckKeyEvent(KeyIndex key)
{
    if(key == KEY_1)
    {
        keyPressStatus = SET;
        keyReleaseStatus = RESET;
        buttonValue = GPIO_ReadInputDataBit(KEY_1_GPIO, KEY_1_PIN); // ��ȡ����״̬������ΪRESET��
    }
    else if(key == KEY_2)
        buttonValue = GPIO_ReadInputDataBit(KEY_2_GPIO, KEY_2_PIN); // ��ȡ����״̬������ΪRESET��
    else if(key == KEY_3)
        buttonValue = GPIO_ReadInputDataBit(KEY_3_GPIO, KEY_3_PIN); // ��ȡ����״̬������ΪRESET��

    uint16_t currentTime = System_GetTimeMs();

    switch (currentState) {
        // ����״̬���ȴ�����
        case IDLE:
            if (buttonValue == keyPressStatus) {
                currentState = PRESSED;
                pressStartTime = currentTime;
            }

            break;

        // ����״̬���ȴ��ͷ�
        case PRESSED:
            if (buttonValue == keyReleaseStatus) {        // �����ͷ�
                pressTimes++;
                if(pressTimes == 2)     currentState = RELEASED;        //�Ѿ��������ΰ�����ֱ�ӽ���RELEASED״̬�����¼�
                else                    currentState = WAIT_DOUBLE;     //����һ�ΰ���������ȴ��ڶ��ΰ�������״̬
                releaseStartTime = currentTime;                         //��¼�����ͷ�ʱ��
            }

            break;

        //�ͷ�״̬�������¼�
        case RELEASED:
            if(pressTimes == 2 && releaseStartTime - pressStartTime < LONG_PRESS_TIME)
            {
                handleDoubleClick();
            }
            if(pressTimes == 1)
            {
                if(currentTime - pressStartTime > LONG_PRESS_TIME)
                {
                    handleLongPress();
                }
                else
                {
                    handleClick();
                }
            }
            pressTimes = 0;
            currentState = IDLE;
            break;

        // �ȴ�˫��״̬�������ΰ��»�ʱ
        case WAIT_DOUBLE:
            if(buttonValue == keyPressStatus)
            {
                pressStartTime = currentTime;
                //��˫������ڰ���
                if(currentTime - releaseStartTime <= DOUBLE_CLICK_INTERVAL)
                {
                    currentState = PRESSED;
                }
                //��˫������ⰴ�£���Ϊ�µ�һ�ΰ����¼���������������
                else if(currentTime - releaseStartTime > DOUBLE_CLICK_INTERVAL)
                {
                    currentState = PRESSED;
                    pressTimes = 0;
                }
            }
            else if(currentTime - releaseStartTime > DOUBLE_CLICK_INTERVAL)     //��˫������ⲻ����
            {
                currentState = RELEASED;
            }

            break;
    }
}

void handleClick(void)
{
    printf("����\r\n");
}

void handleDoubleClick(void)
{
    printf("˫��\r\n");
}

void handleLongPress(void)
{
    printf("����\r\n");
}
