#include "test.h"                   // 测试函数

void bsp_init(void)
{
    uint16_t CAN1_baudrate = 250;
    USART1_Init(115200);        // 串口初始化        //TODO：串口2、3、4、5代码改写为库函数、加DMA
    System_SwdMode();           // 设置芯片调试方式(SWD); 关闭JTAG只保留SWD; 目的:释放PB3、PB4、PA15，只需PA13、PA14
    Led_Init();                 // LED初始化
    Key_Init();                 // KEY初始化
    Timer_Init();               // 定时器初始化
    OLED_Init();                // OLED初始化      //8*16大小，A行B列坐标：（ (B-1)*8, (A-1)*16 ）
//    PWM_Init();                 // PWM初始化
//    IC_Init();                  // 输入捕获初始化
    AD_Init();                  // ADC初始化
    MPU6050_Init();		        // MPU6050初始化
    W25Q64_Init();              // W25Q64初始化        //TODO：W25Q系列芯片统一
    W25qx_Init();               // 设备W25Q128（外部FLASH, 16M空间，前10M用户使用，后6M存有字模数据)
    LCD_Init();                 // 显示屏初始化
    //串口配置波特率方法：上位机一直发送ascii编码的数字，如：125、250等，然后按复位
	//如果不使用串口配置波特率的话，直接修改上面的变量
	if(xUSART.USART1ReceivedNum > 0)
	{
		CAN1_baudrate = (xUSART.USART1ReceivedData[0]-'0')*100 + (xUSART.USART1ReceivedData[1]-'0')*10 + xUSART.USART1ReceivedData[2]-'0';
		xUSART.USART1ReceivedNum = 0;
	}
    CAN1_Config(CAN1_baudrate);                                   // CAN1初始化
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
