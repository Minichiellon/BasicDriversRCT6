
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
        // 测试1：发送字符串
        USART1_SendString("测试1：发送字符串，Test.\r\n");
    }
    if(Key_GetNum() == KEY_2_NUM)
	{
        // 测试2：发送各类数值数据, 注意，串口软件可用16进制显示
        uint8_t data[30];             // 将要发送的数据缓存
        char *strTemp = "测试2：";    // 字符串
        data[0] = *strTemp++;         // 汉字的GBK编码，使用2个字节编码来表示一个汉字
        data[1] = *strTemp++;
        data[2] = *strTemp++;
        data[3] = *strTemp++;
        data[4] = *strTemp++;
        data[5] = *strTemp++;
        data[6] = *strTemp++;
        data[7] = 123;                // 整型，注意：如果用ASCII方式显示到串口上，将为‘{’
        data[8] = '5';                // 字符
        data[9] = '2';                // 字符
        data[10] = '0';               // 字符
        data[11] =  5 ;               // 整型，注意：数据在ASCII编码非可显示的范围内，在串口软上，用ASCII方式将无法显示. 要用16进制显示才能看到数据
        data[12] =  2 ;               // 整型
        data[13] =  0 ;               // 整型
        data[14] = 0x05;              // 整型，注意：很多人没能理解16进制和十进制的区别，在数据传输上，是没有区别的，下方三个数值，等同于上方5,2,0三个数值
        data[15] = 0x02;              // 整型
        data[16] = 0x00;              // 整型

        USART1_SendData(data, 20);    // USART1_SendData函数，可以很灵活地发送各种类型数据
    }
    
    /******  处理UART数据 ******/
    if (xUSART.USART1ReceivedNum)                                    // 判断是否接收到新数据：接收字节长度>0
    {
        printf("\r<<<<<< UART 接收到新数据 <<<<<<");                 // 准备输出
        printf("\r 长度(字节数)：%d", xUSART.USART1ReceivedNum);     // 长度; 单位:字节
        printf("\r 数据(16进制)：");                                 // 16进制方式显示数据，方便观察真实数据
        for (uint8_t i = 0; i < xUSART.USART1ReceivedNum; i++)       // 输出每一字节数值
            printf("0x%X ", xUSART.USART1ReceivedData[i]);
        printf("\r 数据(ASCII) ：%s\r", xUSART.USART1ReceivedData);  // ASCII方式显示数据，方便观察字符串数据;

        xUSART.USART1ReceivedNum = 0;                                // 重要：处理完数据了，把接收数量清０
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

void OLED_test(uint8_t TESTx)
{
    if(TESTx == 1)
    {
        /*在(0, 0)位置显示字符'A'，字体大小为8*16点阵*/
        OLED_ShowChar(0, 0, 'A', OLED_8X16);
        
        /*在(16, 0)位置显示字符串"Hello World!"，字体大小为8*16点阵*/
        OLED_ShowString(16, 0, "Hello World!", OLED_8X16);
        
        /*在(0, 18)位置显示字符'A'，字体大小为6*8点阵*/
        OLED_ShowChar(0, 18, 'A', OLED_6X8);
        
        /*在(16, 18)位置显示字符串"Hello World!"，字体大小为6*8点阵*/
        OLED_ShowString(16, 18, "Hello World!", OLED_6X8);
        
        /*在(0, 28)位置显示数字12345，长度为5，字体大小为6*8点阵*/
        OLED_ShowNum(0, 28, 12345, 5, OLED_6X8);
        
        /*在(40, 28)位置显示有符号数字-66，长度为2，字体大小为6*8点阵*/
        OLED_ShowSignedNum(40, 28, -66, 2, OLED_6X8);
        
        /*在(70, 28)位置显示十六进制数字0xA5A5，长度为4，字体大小为6*8点阵*/
        OLED_ShowHexNum(70, 28, 0xA5A5, 4, OLED_6X8);
        
        /*在(0, 38)位置显示二进制数字0xA5，长度为8，字体大小为6*8点阵*/
        OLED_ShowBinNum(0, 38, 0xA5, 8, OLED_6X8);
        
        /*在(60, 38)位置显示浮点数字123.45，整数部分长度为3，小数部分长度为2，字体大小为6*8点阵*/
        OLED_ShowFloatNum(60, 38, 123.45, 3, 2, OLED_6X8);
        
        /*在(0, 48)位置显示英文和汉字串"Hello,世界。"，支持中英文混写*/
        OLED_ShowString(0, 48, "Hello,世界。", OLED_8X16);
        
        /*在(96, 48)位置显示图像，宽16像素，高16像素，图像数据为Diode数组*/
        OLED_ShowImage(96, 48, 16, 16, Diode);
        
        /*在(96, 18)位置打印格式化字符串，字体大小为6*8点阵，格式化字符串为"[%02d]"*/
        OLED_Printf(96, 18, OLED_6X8, "[%02d]", 6);
        
        /*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
        OLED_Update();
        
        /*延时3000ms，观察现象*/
        System_DelayMS(3000);
        
        /*清空OLED显存数组*/
        OLED_Clear();
        
        /*在(5, 8)位置画点*/
        OLED_DrawPoint(5, 8);
        
        /*获取(5, 8)位置的点*/
        if (OLED_GetPoint(5, 8))
        {
            /*如果指定点点亮，则在(10, 4)位置显示字符串"YES"，字体大小为6*8点阵*/
            OLED_ShowString(10, 4, "YES", OLED_6X8);
        }
        else
        {
            /*如果指定点未点亮，则在(10, 4)位置显示字符串"NO "，字体大小为6*8点阵*/
            OLED_ShowString(10, 4, "NO ", OLED_6X8);
        }
        
        /*在(40, 0)和(127, 15)位置之间画直线*/
        OLED_DrawLine(40, 0, 127, 15);
        
        /*在(40, 15)和(127, 0)位置之间画直线*/
        OLED_DrawLine(40, 15, 127, 0);
        
        /*在(0, 20)位置画矩形，宽12像素，高15像素，未填充*/
        OLED_DrawRectangle(0, 20, 12, 15, OLED_UNFILLED);
        
        /*在(0, 40)位置画矩形，宽12像素，高15像素，填充*/
        OLED_DrawRectangle(0, 40, 12, 15, OLED_FILLED);
        
        /*在(20, 20)、(40, 25)和(30, 35)位置之间画三角形，未填充*/
        OLED_DrawTriangle(20, 20, 40, 25, 30, 35, OLED_UNFILLED);
        
        /*在(20, 40)、(40, 45)和(30, 55)位置之间画三角形，填充*/
        OLED_DrawTriangle(20, 40, 40, 45, 30, 55, OLED_FILLED);
        
        /*在(55, 27)位置画圆，半径8像素，未填充*/
        OLED_DrawCircle(55, 27, 8, OLED_UNFILLED);
        
        /*在(55, 47)位置画圆，半径8像素，填充*/
        OLED_DrawCircle(55, 47, 8, OLED_FILLED);
        
        /*在(82, 27)位置画椭圆，横向半轴12像素，纵向半轴8像素，未填充*/
        OLED_DrawEllipse(82, 27, 12, 8, OLED_UNFILLED);
        
        /*在(82, 47)位置画椭圆，横向半轴12像素，纵向半轴8像素，填充*/
        OLED_DrawEllipse(82, 47, 12, 8, OLED_FILLED);
        
        /*在(110, 18)位置画圆弧，半径15像素，起始角度25度，终止角度125度，未填充*/
        OLED_DrawArc(110, 18, 15, 25, 125, OLED_UNFILLED);
        
        /*在(110, 38)位置画圆弧，半径15像素，起始角度25度，终止角度125度，填充*/
        OLED_DrawArc(110, 38, 15, 25, 125, OLED_FILLED);
        
        /*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
        OLED_Update();
        
        /*延时3000ms，观察现象*/
        System_DelayMS(3000);
    }
    if(TESTx == 2)
    {
        for (uint8_t i = 0; i < 4; i ++)
		{
			/*将OLED显存数组部分数据取反，从(0, i * 16)位置开始，宽128像素，高16像素*/
			OLED_ReverseArea(0, i * 16, 128, 16);
			
			/*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
			OLED_Update();
			
			/*延时1000ms，观察现象*/
			System_DelayMS(1000);
			
			/*把取反的内容翻转回来*/
			OLED_ReverseArea(0, i * 16, 128, 16);
		}
		
		/*将OLED显存数组全部数据取反*/
		OLED_Reverse();
		
		/*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
		OLED_Update();
		
		/*延时1000ms，观察现象*/
		System_DelayMS(1000);
    }
}
