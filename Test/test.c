
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

void OLED_test(void)
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
        
        
        for (uint8_t i = 0; i < 4; i ++)
		{
			/*将OLED显存数组部分数据取反，从(0, i * 16)位置开始，宽128像素，高16像素*/
			OLED_ReverseArea(0, i * 16, 128, 16);
			
			/*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
			OLED_Update();
			
			/*延时1000ms，观察现象*/
			System_DelayMS(3000);
			
			/*把取反的内容翻转回来*/
			OLED_ReverseArea(0, i * 16, 128, 16);
		}
		
		/*将OLED显存数组全部数据取反*/
		OLED_Reverse();
		
		/*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
		OLED_Update();
		
		/*延时1000ms，观察现象*/
		System_DelayMS(3000);
        
        /*清空OLED显存数组*/
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
        /*显示静态字符串*/
        OLED_ShowString(0, 0, "Freq:00000Hz", OLED_8X16);      //1行1列显示字符串Freq:00000Hz
        OLED_ShowString(0, 17, "Duty:00%", OLED_8X16);          //2行1列显示字符串Duty:00%
        OLED_Update();
    }
    if(TIM2_IrqFlag)
    {
        delay++;
        if(delay > 3000)    //3s
        {
            /*使用PWM模块提供输入捕获的测试信号，每3秒改变一次*/
            PWM_SetPrescaler(prescaler - 1);                  //PWM频率Freq = 72M / (PSC + 1) / 100
            prescaler /= 2;
            if(prescaler < 45) prescaler = 720;
            
            PWM_SetCompare1(duty);                        //PWM占空比Duty = CCR / 100
            duty += 10;
            if(duty > 100)  duty = 0;
        }
        TIM2_IrqFlag = 0;
    }
    OLED_ShowNum(40, 0, IC_GetFreq(), 5, OLED_8X16);    //不断刷新显示输入捕获测得的频率
    OLED_ShowNum(40, 17, IC_GetDuty(), 2, OLED_8X16);    //不断刷新显示输入捕获测得的占空比
    OLED_Update();
}

void ADC_test(void)
{
    static uint8_t flash_flag = 0;
    if(flash_flag == 0)
    {
        flash_flag = 1;
        /*显示静态字符串*/
        OLED_ShowString(0, 0, "AD0:", OLED_8X16);
        OLED_ShowString(0, 17, "AD1:", OLED_8X16);
        OLED_ShowString(0, 33, "AD2:", OLED_8X16);
        OLED_ShowString(0, 49, "AD3:", OLED_8X16);
        OLED_Update();
    }
    
    OLED_ShowNum(33, 0, AD_Value[0], 4, OLED_8X16);		//显示转换结果第0个数据
    OLED_ShowNum(33, 17, AD_Value[1], 4, OLED_8X16);		//显示转换结果第1个数据
    OLED_ShowNum(33, 33, AD_Value[2], 4, OLED_8X16);		//显示转换结果第2个数据
    OLED_ShowNum(33, 49, AD_Value[3], 4, OLED_8X16);		//显示转换结果第3个数据
    OLED_Update();
    
    System_DelayMS(100);							//延时100ms，手动增加一些转换的间隔时间
}

void MPU6050_test(void)
{
    uint8_t ID;                             //定义用于存放ID号的变量
    int16_t AX, AY, AZ, GX, GY, GZ;			//定义用于存放各个数据的变量
    static uint8_t flash_flag = 0;
    
    if(flash_flag == 0)
    {
        flash_flag = 1;
        /*显示静态字符串*/
        OLED_ShowString(0, 0, "ID:", OLED_8X16);		//显示静态字符串
        ID = MPU6050_GetID();				            //获取MPU6050的ID号
        OLED_ShowHexNum(24, 0, ID, 2, OLED_8X16);		//OLED显示ID号
        OLED_Update();
    }
    MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);		//获取MPU6050的数据
    OLED_ShowSignedNum(0, 16, AX, 5, OLED_8X16);			//OLED显示数据
    OLED_ShowSignedNum(0, 32, AY, 5, OLED_8X16);
    OLED_ShowSignedNum(0, 48, AZ, 5, OLED_8X16);
    OLED_ShowSignedNum(56, 16, GX, 5, OLED_8X16);
    OLED_ShowSignedNum(56, 32, GY, 5, OLED_8X16);
    OLED_ShowSignedNum(56, 48, GZ, 5, OLED_8X16);
    OLED_Update();
}

void W25Q64_test(void)
{
    uint8_t MID;                            //定义用于存放MID号的变量
    uint16_t DID;                            //定义用于存放DID号的变量

    uint8_t ArrayWrite[] = {0x01, 0x02, 0x03, 0x04};    //定义要写入数据的测试数组
    uint8_t ArrayRead[4];                                //定义要读取数据的测试数组
    static uint8_t flash_flag = 0;
    
    if(flash_flag == 0)
    {
        flash_flag = 1;
        /*显示静态字符串*/
        OLED_ShowString(0, 0, "MID:   DID:", OLED_8X16);
        OLED_ShowString(0, 16, "W:", OLED_8X16);
        OLED_ShowString(0, 32, "R:", OLED_8X16);
        
        /*显示ID号*/
        W25Q64_ReadID(&MID, &DID);            //获取W25Q64的ID号
        OLED_ShowHexNum(32, 0, MID, 2, OLED_8X16);        //显示MID
        OLED_ShowHexNum(88, 0, DID, 4, OLED_8X16);        //显示DID
        
        /*W25Q64功能函数测试*/
        W25Q64_SectorErase(0x000000);                    //扇区擦除
        W25Q64_PageProgram(0x000000, ArrayWrite, 4);    //将写入数据的测试数组写入到W25Q64中
        
        W25Q64_ReadData(0x000000, ArrayRead, 4);        //读取刚写入的测试数据到读取数据的测试数组中
        
        /*显示数据*/
        OLED_ShowHexNum(16, 16, ArrayWrite[0], 2, OLED_8X16);        //显示写入数据的测试数组
        OLED_ShowHexNum(40, 16, ArrayWrite[1], 2, OLED_8X16);
        OLED_ShowHexNum(64, 16, ArrayWrite[2], 2, OLED_8X16);
        OLED_ShowHexNum(88, 16, ArrayWrite[3], 2, OLED_8X16);
        
        OLED_ShowHexNum(16, 32, ArrayRead[0], 2, OLED_8X16);            //显示读取数据的测试数组
        OLED_ShowHexNum(40, 32, ArrayRead[1], 2, OLED_8X16);
        OLED_ShowHexNum(64, 32, ArrayRead[2], 2, OLED_8X16);
        OLED_ShowHexNum(88, 32, ArrayRead[3], 2, OLED_8X16);
        OLED_Update();
    }
}

void Flash_test(void)
{
    static uint32_t STORE_START_ADDRESS = 0x0800FC00;		//存储的起始地址
    static uint16_t STORE_COUNT = 512;				//存储数据的个数
    static uint16_t Store_Data[512];				//定义SRAM数组
    
    static uint8_t flash_flag = 0;
    uint8_t KeyNum = Key_GetNum();
    
    if(flash_flag == 0)
    {
        flash_flag = 1;
        /*显示静态字符串*/
        OLED_ShowString(0, 0, "Flag:", OLED_8X16);
        OLED_ShowString(0, 16, "Data:", OLED_8X16);
        OLED_Update();
        
        /*判断是不是第一次使用*/
        if (Flash_ReadHalfWord(STORE_START_ADDRESS) != 0xA5A5)	//读取第一个半字的标志位，if成立，则执行第一次使用的初始化
        {
            Flash_ErasePage(STORE_START_ADDRESS);					//擦除指定页
            Flash_ProgramHalfWord(STORE_START_ADDRESS, 0xA5A5);	//在第一个半字写入自己规定的标志位，用于判断是不是第一次使用
            for (uint16_t i = 1; i < STORE_COUNT; i ++)				//循环STORE_COUNT次，除了第一个标志位
            {
                Flash_ProgramHalfWord(STORE_START_ADDRESS + i * 2, 0x0000);		//除了标志位的有效数据全部清0
            }
        }
        
        /*上电时，将闪存数据加载回SRAM数组，实现SRAM数组的掉电不丢失*/
        for (uint16_t i = 0; i < STORE_COUNT; i ++)					//循环STORE_COUNT次，包括第一个标志位
        {
            Store_Data[i] = Flash_ReadHalfWord(STORE_START_ADDRESS + i * 2);		//将闪存的数据加载回SRAM数组
        }
    }
    
    if(KeyNum == KEY_1_NUM)
    {
        Store_Data[1] ++;		//变换测试数据
        Store_Data[2] += 2;
        Store_Data[3] += 3;
        Store_Data[4] += 4;
        
        Flash_ErasePage(STORE_START_ADDRESS);				//擦除指定页
        for (uint16_t i = 0; i < STORE_COUNT; i ++)			//循环STORE_COUNT次，包括第一个标志位
        {
            Flash_ProgramHalfWord(STORE_START_ADDRESS + i * 2, Store_Data[i]);	//将SRAM数组的数据备份保存到闪存
        }
    }
    
    if(Key_GetNum() == KEY_2_NUM)
    {
        for (uint16_t i = 1; i < STORE_COUNT; i ++)			//循环STORE_COUNT次，除了第一个标志位
        {
            Store_Data[i] = 0x0000;							//SRAM数组有效数据清0
        }
        
    	Flash_ErasePage(STORE_START_ADDRESS);				//擦除指定页
        for (uint16_t i = 0; i < STORE_COUNT; i ++)			//循环STORE_COUNT次，包括第一个标志位
        {
            Flash_ProgramHalfWord(STORE_START_ADDRESS + i * 2, Store_Data[i]);	//将SRAM数组的数据备份保存到闪存
        }
    }
    
    OLED_ShowHexNum(40, 0, Store_Data[0], 4, OLED_8X16);	//显示Store_Data的第一位标志位
    OLED_ShowHexNum(0, 32, Store_Data[1], 4, OLED_8X16);	//显示Store_Data的有效存储数据
    OLED_ShowHexNum(40, 32, Store_Data[2], 4, OLED_8X16);
    OLED_ShowHexNum(0, 48, Store_Data[3], 4, OLED_8X16);
    OLED_ShowHexNum(40, 48, Store_Data[4], 4, OLED_8X16);
    OLED_Update();
}
