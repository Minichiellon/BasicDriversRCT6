#ifndef __BSP_RTC_H
#define __BSP_RTC_H
/**==================================================================================================================
 **【文件名称】  bsp_RTC.h
 **【功能测试】  RTC实时时钟
 **==================================================================================================================
 **【实验平台】  STM32F103 + KEIL5.27 
 **
 **【实验操作】  1_无需接线
 **              2_需要PC14、PC15设计有32.768K晶振电路，且芯片的VBat引脚由外部电源持续供电，不随开发板的断电而断电，方可进行此实验测试;
 **              3_魔女开发板系列，部分型号，已带32.768K晶振，且已为VBat引脚预留CR1220纽扣电池座;
 **              4_魔女开发板，发货时，由于快递原因，不再附带纽扣电池; 进行本实验，需要另购CR1220电池安装上;
 **              5_CR1220规格的纽扣电池大约0.2元一粒，购买链接参考：https://item.taobao.com/item.htm?spm=a230r.1.14.27.db217ccfthH6Y6&id=17713634795&ns=1&abbucket=4#detail
 **
 **【划 重 点】  1_每次唤醒估计误差1秒;
 **              2_对于需要频繁休眠唤醒的使用需求，使用内部的RTC会造成计时误差逐渐越差越大; 建议使用外部日历芯片;
 **
 **【函数使用】  初始化：  RTC_Init();       // 配置RTC
 **              设置数据：RTC_SetDate();    // 设置年、月、日、时、分、秒; 设置一次即可，只要VBat引脚不断电，而板子、芯片断电，不影响RTC数据的记录
 **              获取数据：RTC_SetDate();    // 获取数据，存放至xRTC结构体中，具体使用，可参考示例，或查看h文件的结构体定义;
 **
 **【备注说明】  代码版权归魔女科技所有，请勿商用，谢谢！
 **              https://demoboard.taobao.com
====================================================================================================================*/
#include <stm32f10x.h>



/*****************************************************************************
 ** 全局变量
****************************************************************************/
typedef struct
{
    uint16_t Year;           // 年
    uint8_t  Month;          // 月
    uint8_t  Day;            // 日
    uint8_t  Hour;           // 时
    uint8_t  Minute;         // 分
    uint8_t  Second;         // 秒
    uint8_t  Week;           // 星期，数字，0、1、2、3、4、5、6
    char*    WeekString;     // 星期，中文，"日”、"一"、"二"、"三"、"四"、"五"、"六"
} xRTC_TypeDef;
extern xRTC_TypeDef  xRTC;   // 声明为全局变量,方便记录信息、状态



/*****************************************************************************
 ** 声明全局函数
****************************************************************************/
u8   RTC_Init(void);           // RTC初始化
u8   RTC_SetDate(uint16_t Year, uint8_t Month, uint8_t Day, uint8_t Hour, uint8_t Minute, uint8_t Second);   // 设置年月日时分秒
u8   RTC_GetDate(void);        // 获取日期、时间、星期，存放到xRTC结构体中



#endif
