
#include "bsp_IC.h"
/**
  * ��    �������벶���ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void IC_Init(void)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//����TIM3��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			//����GPIOB��ʱ��
	
    /*GPIO��ӳ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//����AFIO��ʱ�ӣ���ӳ������ȿ���AFIO��ʱ��
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);			//��TIM3�����Ų�����ӳ�䣬�����ӳ�䷽����鿴�ο��ֲ�
    
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);							//��PB6���ų�ʼ��Ϊ��������
	
	/*����ʱ��Դ*/
	TIM_InternalClockConfig(TIM3);		//ѡ��TIM3Ϊ�ڲ�ʱ�ӣ��������ô˺�����TIMĬ��ҲΪ�ڲ�ʱ��
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//����ṹ�����
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ��ѡ�񲻷�Ƶ���˲������������˲���ʱ�ӣ���Ӱ��ʱ����Ԫ����
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //������ģʽ��ѡ�����ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               //�������ڣ���ARR��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;               //Ԥ��Ƶ������PSC��ֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //�ظ����������߼���ʱ���Ż��õ�
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);             //���ṹ���������TIM_TimeBaseInit������TIM3��ʱ����Ԫ
	
	/*PWMIģʽ��ʼ��*/
	TIM_ICInitTypeDef TIM_ICInitStructure;							//����ṹ�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				//ѡ�����ö�ʱ��ͨ��1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//�����˲������������Թ����źŶ���
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//���ԣ�ѡ��Ϊ�����ش�������
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			//����Ԥ��Ƶ��ѡ�񲻷�Ƶ��ÿ���źŶ���������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	//�����źŽ��棬ѡ��ֱͨ��������
	TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);						//���ṹ���������TIM_PWMIConfig������TIM3�����벶��ͨ��
																	//�˺���ͬʱ�����һ��ͨ������Ϊ�෴�����ã�ʵ��PWMIģʽ

	/*ѡ�񴥷�Դ����ģʽ*/
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);					//����Դѡ��TI1FP1
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);					//��ģʽѡ��λ
																	//��TI1����������ʱ���ᴥ��CNT����
	
	/*TIMʹ��*/
	TIM_Cmd(TIM3, ENABLE);			//ʹ��TIM3����ʱ����ʼ����
}

/**
  * ��    ������ȡ���벶���Ƶ��
  * ��    ������
  * �� �� ֵ������õ���Ƶ��
  */
uint32_t IC_GetFreq(void)
{
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);		//���ܷ��õ�Ƶ��fx = fc / N�����ﲻִ��+1�Ĳ���Ҳ��
}

/**
  * ��    ������ȡ���벶���ռ�ձ�
  * ��    ������
  * �� �� ֵ������õ���ռ�ձ�
  */
uint32_t IC_GetDuty(void)
{
	return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);	//ռ�ձ�Duty = CCR2 / CCR1 * 100�����ﲻִ��+1�Ĳ���Ҳ��
}
