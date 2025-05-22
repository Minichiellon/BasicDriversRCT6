#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "system_f103.h"

static uint8_t KeyPressFlag;        //按键1、2、3按下标志位

static uint8_t buttonValue;                //按键电平状态
static uint8_t keyPressStatus = RESET;     //按键按下时的电平状态
static uint8_t keyReleaseStatus = SET;     //按键释放时的电平状态
static uint8_t pressTimes = 0;             //按键次数
static ButtonState currentState = IDLE;    //状态机变量
static uint16_t pressStartTime = 0;        //按键按下时刻时间
static uint16_t releaseStartTime = 0;      //按键释放时刻时间

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;    // 作用：配置引脚工作模式
    NVIC_InitTypeDef NVIC_InitStruct ;      // 作用：配置优先级
    EXTI_InitTypeDef EXTI_InitStruct ;      // 作用：配置引脚中断方式

    // 使能AFIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE) ;      // EXTI的时钟要设置AFIO寄存器

    // 使能GPIO时钟, 为减少调试过程忘了使能时钟而出错，把相关GPIO端口时钟，都使能了；
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF, ENABLE);

    // KEY_1
    // 配置引脚工作模式
    GPIO_InitStructure.GPIO_Pin  = KEY_1_PIN;                  // 引脚KEY_1, 闲时下拉(重要)， 按下时被置高电平
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ;             // 引脚工作模式; 闲时电平状态(使用芯片内部电阻进行电平上下拉)
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;         // 输出电平反转速度；在输入状态时无效，但GPIO_Init函数需要用到；
    GPIO_Init(KEY_1_GPIO, &GPIO_InitStructure);                // 初始化，调用引脚工作模式配置函数
    // 配置中断的优先级
    NVIC_InitStruct.NVIC_IRQChannel = KEY_1_IRQN ;             // 中断号
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1 ;    // 配置抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0 ;           // 配置子优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE ;              // 使能中断通道
    NVIC_Init(&NVIC_InitStruct) ;                              // 初始化，调用优先级配置函数
    // 配置中断的方式
    GPIO_EXTILineConfig(KEY_1_GPIOSOURCE, KEY_1_PINSOURCE);    // 选择作为EXTI线的GPIO引脚
    EXTI_InitStruct.EXTI_Line    = KEY_1_EXTI_LINE ;           // 配置中断or事件线
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt ;       // 模式：中断：EXTI_Mode_Interrupt、事件：EXTI_Mode_Event
    EXTI_InitStruct.EXTI_Trigger = KEY_1_TRIM ;                // 边沿触发： 上升：EXTI_Trigger_Rising 、下降：EXTI_Trigger_Falling 、浮空：EXTI_Trigger_Rising_Falling
    EXTI_InitStruct.EXTI_LineCmd = ENABLE ;                    // 使能EXTI线
    EXTI_Init(&EXTI_InitStruct) ;                              // 初始化，调用中断线配置函数

    // KEY_2
    // 配置引脚工作模式
    GPIO_InitStructure.GPIO_Pin  = KEY_2_PIN;                  // 引脚KEY_1, 闲时下拉(重要)， 按下时被置高电平
    GPIO_InitStructure.GPIO_Mode = KEY_2_MODE ;                // 引脚工作模式; 闲时电平状态(使用芯片内部电阻进行电平上下拉)
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;         // 输出电平反转速度；在输入状态时无效，但GPIO_Init函数需要用到；
    GPIO_Init(KEY_2_GPIO, &GPIO_InitStructure);                // 初始化，调用引脚工作模式配置函数
    // 配置中断的优先级
    NVIC_InitStruct.NVIC_IRQChannel = KEY_2_IRQN ;             // 中断号
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1 ;    // 配置抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0 ;           // 配置子优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE ;              // 使能中断通道
    NVIC_Init(&NVIC_InitStruct) ;                              // 初始化，调用优先级配置函数
    // 配置中断的方式
    GPIO_EXTILineConfig(KEY_2_GPIOSOURCE, KEY_2_PINSOURCE);    // 选择作为EXTI线的GPIO引脚
    EXTI_InitStruct.EXTI_Line    = KEY_2_EXTI_LINE ;           // 配置中断or事件线
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt ;       // 模式：中断：EXTI_Mode_Interrupt、事件：EXTI_Mode_Event
    EXTI_InitStruct.EXTI_Trigger = KEY_2_TRIM ;                // 边沿触发： 上升：EXTI_Trigger_Rising 、下降：EXTI_Trigger_Falling 、浮空：EXTI_Trigger_Rising_Falling
    EXTI_InitStruct.EXTI_LineCmd = ENABLE ;                    // 使能EXTI线
    EXTI_Init(&EXTI_InitStruct) ;                              // 初始化，调用中断线配置函数

    // KEY_3
    // 配置引脚工作模式
    GPIO_InitStructure.GPIO_Pin  = KEY_3_PIN;                  // 引脚KEY_1, 闲时下拉(重要)， 按下时被置高电平
    GPIO_InitStructure.GPIO_Mode = KEY_3_MODE ;                // 引脚工作模式; 闲时电平状态(使用芯片内部电阻进行电平上下拉)
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;         // 输出电平反转速度；在输入状态时无效，但GPIO_Init函数需要用到；
    GPIO_Init(KEY_3_GPIO, &GPIO_InitStructure);                // 初始化，调用引脚工作模式配置函数
    // 配置中断的优先级
    NVIC_InitStruct.NVIC_IRQChannel = KEY_3_IRQN ;             // 中断号
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1 ;    // 配置抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0 ;           // 配置子优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE ;              // 使能中断通道
    NVIC_Init(&NVIC_InitStruct) ;                              // 初始化，调用优先级配置函数
    // 配置中断的方式
    GPIO_EXTILineConfig(KEY_3_GPIOSOURCE, KEY_3_PINSOURCE);    // 选择作为EXTI线的GPIO引脚
    EXTI_InitStruct.EXTI_Line    = KEY_3_EXTI_LINE ;           // 配置中断or事件线
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt ;       // 模式：中断：EXTI_Mode_Interrupt、事件：EXTI_Mode_Event
    EXTI_InitStruct.EXTI_Trigger = KEY_3_TRIM ;                // 边沿触发： 上升：EXTI_Trigger_Rising 、下降：EXTI_Trigger_Falling 、浮空：EXTI_Trigger_Rising_Falling
    EXTI_InitStruct.EXTI_LineCmd = ENABLE ;                    // 使能EXTI线
    EXTI_Init(&EXTI_InitStruct) ;                              // 初始化，调用中断线配置函数
}

/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：1~3，返回0代表没有按键按下
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

    return KeyNum;			//返回键码值，如果没有按键按下，所有if都不成立，则键码为默认值0
}

// KEY_1 中断服务函数
void KEY_1_IRQHANDLER(void)                        		// 提示：这个函数名，是h文件中的宏定义，在编译过程中，会被替换成宏定义的值
{
    if (EXTI_GetITStatus(KEY_1_EXTI_LINE))          	// 板子上的按键已使用电容作简单的硬件消抖,无需再使用软件延时消抖
    {
        EXTI_ClearITPendingBit(KEY_1_EXTI_LINE);    	// 清理中断标示
        KeyPressFlag = KEY_1_PRESS;
    }
}



// KEY_2 中断服务函数
void KEY_2_IRQHANDLER(void)                        		// 提示：这个函数名，是h文件中的宏定义，在编译过程中，会被替换成宏定义的值
{
    if (EXTI_GetITStatus(KEY_2_EXTI_LINE))				// 板子上的按键已使用电容作简单的硬件消抖,无需再使用软件延时消抖
    {
        EXTI_ClearITPendingBit(KEY_2_EXTI_LINE);		// 清理中断标示
        KeyPressFlag = KEY_2_PRESS;
    }
}



// KEY_3 中断服务函数
void KEY_3_IRQHANDLER(void)                        		// 提示：这个函数名，是h文件中的宏定义，在编译过程中，会被替换成宏定义的值
{
    if (EXTI_GetITStatus(KEY_3_EXTI_LINE))              // 板子上的按键已使用电容作简单的硬件消抖,无需再使用软件延时消抖
    {
        EXTI_ClearITPendingBit(KEY_3_EXTI_LINE);        // 清理中断标示
        KeyPressFlag = KEY_3_PRESS;
    }
}

void CheckKeyEvent(KeyIndex key)
{
    if(key == KEY_1)
    {
        keyPressStatus = SET;
        keyReleaseStatus = RESET;
        buttonValue = GPIO_ReadInputDataBit(KEY_1_GPIO, KEY_1_PIN); // 读取按键状态（按下为RESET）
    }
    else if(key == KEY_2)
        buttonValue = GPIO_ReadInputDataBit(KEY_2_GPIO, KEY_2_PIN); // 读取按键状态（按下为RESET）
    else if(key == KEY_3)
        buttonValue = GPIO_ReadInputDataBit(KEY_3_GPIO, KEY_3_PIN); // 读取按键状态（按下为RESET）

    uint16_t currentTime = System_GetTimeMs();

    switch (currentState) {
        // 空闲状态：等待按下
        case IDLE:
            if (buttonValue == keyPressStatus) {
                currentState = PRESSED;
                pressStartTime = currentTime;
            }

            break;

        // 按下状态：等待释放
        case PRESSED:
            if (buttonValue == keyReleaseStatus) {        // 按键释放
                pressTimes++;
                if(pressTimes == 2)     currentState = RELEASED;        //已经按了两次按键，直接进入RELEASED状态处理事件
                else                    currentState = WAIT_DOUBLE;     //按了一次按键，进入等待第二次按键按下状态
                releaseStartTime = currentTime;                         //记录按键释放时间
            }

            break;

        //释放状态：处理事件
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

        // 等待双击状态：检测二次按下或超时
        case WAIT_DOUBLE:
            if(buttonValue == keyPressStatus)
            {
                pressStartTime = currentTime;
                //在双击间隔内按下
                if(currentTime - releaseStartTime <= DOUBLE_CLICK_INTERVAL)
                {
                    currentState = PRESSED;
                }
                //在双击间隔外按下，视为新的一次按键事件，按键次数清零
                else if(currentTime - releaseStartTime > DOUBLE_CLICK_INTERVAL)
                {
                    currentState = PRESSED;
                    pressTimes = 0;
                }
            }
            else if(currentTime - releaseStartTime > DOUBLE_CLICK_INTERVAL)     //在双击间隔外不按下
            {
                currentState = RELEASED;
            }

            break;
    }
}

void handleClick(void)
{
    printf("单击\r\n");
}

void handleDoubleClick(void)
{
    printf("双击\r\n");
}

void handleLongPress(void)
{
    printf("长按\r\n");
}
