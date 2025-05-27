#include "system_f103.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bsp_usart.h"
#include "bsp_can.h"
#include "bsp_tim.h"
#include "bsp_lcd_ili9341.h"
#include "UsartCtlCan.h"

#define SIZE_16_LINE(x)     (x*16)

#define CAN_TX_LIST     0
#define CAN_RX_LIST     1

#define CAN_LIST_NUM    5           //报文列表大小
CAN_List canTx_list[CAN_LIST_NUM];  //发送报文列表，用于Lcd屏刷新
CAN_List canRx_list[CAN_LIST_NUM];  //接收报文列表，用于Lcd屏刷新

void CanListInit(void)
{
    for(uint8_t i = 0; i < CAN_LIST_NUM; i++)
    {
        //Tx列表初始化
        canTx_list[i].can_info.can_id = 0;
        memset(canTx_list[i].can_info.can_data, 0, 8);

        canTx_list[i].can_info.TxTime= 0xFFFF;
        canTx_list[i].can_info.TimeOutTime = 1000;
        canTx_list[i].can_info.last_time = 0;
        canTx_list[i].IdleFlag = 1;
        canTx_list[i].can_info.TimeOutFlag = 1;
        
        //Rx列表初始化
        canRx_list[i].can_info.can_id = 0;
        memset(canRx_list[i].can_info.can_data, 0, 8);

        canRx_list[i].can_info.TimeOutTime = 1000;
        canRx_list[i].can_info.last_time = 0;
        canRx_list[i].IdleFlag = 1;
        canRx_list[i].can_info.TimeOutFlag = 1;
    }
}
/**
  * 函    数：查找报文列表是否有空闲位置
  * 参    数：CanListType：报文列表类型，0-发送列表，1-接收列表
  * 返 回 值：报文列表空闲位置，0 ~ （CAN_LIST_NUM-1）代表找到空闲位置，-1代表没有空闲位置
  */
static int8_t FindCanListIdleIndex(uint8_t CanListType)
{
    int8_t IdleIndex = -1;
    if(CanListType == CAN_TX_LIST)
    {
        for(uint8_t i = 0; i < CAN_LIST_NUM; i++)
        {
            if(canTx_list[i].IdleFlag == 1)
            {
                IdleIndex = i;
                break;          //找到即退出
            }
        }
    }
    else if(CanListType == CAN_RX_LIST)
    {
        for(uint8_t i = 0; i < CAN_LIST_NUM; i++)
        {
            if(canRx_list[i].IdleFlag == 1)
            {
                IdleIndex = i;
                break;
            }
        }
    }
    return IdleIndex;
}

/**
  * 函    数：查找报文是否已经在报文列表中
  * 参    数：CanListType：报文列表类型，0-发送列表，1-接收列表
              CanId：要查找的报文Id
  * 返 回 值：报文在列表的位置，0 ~ （CAN_LIST_NUM-1），-1代表不存在
  */
static int8_t CheckIsInCanList(uint8_t CanListType, uint32_t CanId)
{
    int8_t Index = -1;
    if(CanListType == CAN_TX_LIST)
    {
        for(int i = 0; i < CAN_LIST_NUM; i++)
        {
            if(CanId == canTx_list[i].can_info.can_id)
            {
                Index = i;
                break;
            }
        }
    }
    else if(CanListType == CAN_RX_LIST)
    {
        for(int i = 0; i < CAN_LIST_NUM; i++)
        {
            if(CanId == canRx_list[i].can_info.can_id)
            {
                Index = i;
                break;
            }
        }
    }
    return Index;
}

/**
  * 函    数：更新报文列表
  * 参    数：CanListType：报文列表类型，0-发送列表，1 -接收列表
              Pos：插入位置
              CanInfo：要插入的报文
  * 返 回 值：无
  */
static void UpdateCanList(uint8_t CanListType, uint8_t Pos, CAN_INFO CanInfo)
{
    if(CanListType == CAN_TX_LIST)
    {
        canTx_list[Pos].can_info.can_id = CanInfo.can_id;
        memcpy(canTx_list[Pos].can_info.can_data, CanInfo.can_data, 8);
        canTx_list[Pos].can_info.last_time = CanInfo.last_time;
        canTx_list[Pos].can_info.TxTime= CanInfo.TxTime;
        canTx_list[Pos].IdleFlag = 0;
        
    }
    else if(CanListType == CAN_RX_LIST)
    {
        canRx_list[Pos].can_info.can_id = CanInfo.can_id;
        memcpy(canRx_list[Pos].can_info.can_data, CanInfo.can_data, 8);
        canRx_list[Pos].can_info.last_time = CanInfo.last_time;
        canRx_list[Pos].IdleFlag = 0;
    }
}

/**
  * 函    数：报文状态Lcd屏闪烁
  * 参    数：CanListType：报文列表类型，0-发送列表，1-接收列表
              Pos：要闪烁的报文位置，0 ~ （CAN_LIST_NUM-1）分别在0 ~ （CAN_LIST_NUM-1）行
  * 返 回 值：无
  */
static uint16_t cnt[2*CAN_LIST_NUM] = {0};          //每条报文的cnt计数，0 ~ CAN_LIST_NUM-1表示Tx报文，CAN_LIST_NUM ~ 2*CAN_LIST_NUM-1表示Rx报文
static uint16_t StateFlag[2*CAN_LIST_NUM] = {0};    //每条报文的指示状态，0 ~ CAN_LIST_NUM-1表示Tx报文，CAN_LIST_NUM ~ 2*CAN_LIST_NUM-1表示Rx报文
static void LcdFlash(uint8_t CanListType, int8_t Pos)
{
    if(cnt[CanListType*CAN_LIST_NUM+Pos] >= 1 && cnt[CanListType*CAN_LIST_NUM+Pos] <= 500)      //亮
    {
        StateFlag[CanListType*CAN_LIST_NUM+Pos] = 1;
    }
    else if(cnt[CanListType*CAN_LIST_NUM+Pos] > 500 && cnt[CanListType*CAN_LIST_NUM+Pos] <= 1000)      //灭
    {
        StateFlag[CanListType*CAN_LIST_NUM+Pos] = 0;
    }
    
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    if(StateFlag[CanListType*CAN_LIST_NUM+Pos] == 1)
    {
        LCD_Fill(28*8, (CanListType)*170 + SIZE_16_LINE(Pos)+2, 29*8, (CanListType)*170 + SIZE_16_LINE(Pos)+12, GREEN);
    }
    else
    {
        LCD_Fill(28*8, (CanListType)*170 + SIZE_16_LINE(Pos)+2, 29*8, (CanListType)*170 + SIZE_16_LINE(Pos)+12, BLACK);
    }
    __set_PRIMASK(primask);
}

void UpdateTimeOutFlag(uint8_t CanListType)
{
    if(CanListType == CAN_TX_LIST)
    {
        for(uint8_t i = 0; i < CAN_LIST_NUM; i++)
        {
            uint64_t currentTime = System_GetTimeMs();
            if(currentTime - canTx_list[i].can_info.last_time > canTx_list[i].can_info.TimeOutTime)
            {
                canTx_list[i].can_info.TimeOutFlag = 1;
            }
            else
            {
                canTx_list[i].can_info.TimeOutFlag = 0;
            }
        }
    }
    else if(CanListType == CAN_RX_LIST)
    {
        for(uint8_t i = 0; i < CAN_LIST_NUM; i++)
        {
            uint64_t currentTime = System_GetTimeMs();
            if(currentTime - canRx_list[i].can_info.last_time > canRx_list[i].can_info.TimeOutTime)
            {
                canRx_list[i].can_info.TimeOutFlag = 1;
            }
            else
            {
                canRx_list[i].can_info.TimeOutFlag = 0;
            }
        }
    }
}
/**
  * 函    数：解析单行报文数据并发送
  * 参    数：line：需解析的字符串
  * 返 回 值：无
  */
static void ProcessSingleLine(char *line) {
    
    CAN_INFO can_info;
    // 跳过行首空白字符
    while(*line == ' ' || *line == '\r') line++;
    
    char *ptr = strtok(line, " ");
    if(!ptr) return;

    // 解析命令位
    uint8_t command = atoi(ptr);
    if(command != 0 && command != 1) return;
    
    // 解析CAN ID
    ptr = strtok(NULL, " ");
    if(!ptr) return;
    // 使用strtoul直接转换完整十六进制字符串
    can_info.can_id = strtoul(ptr, NULL, 16);
    
    // 验证ID范围（标准帧ID范围0x000-0x7FF）
    if(can_info.can_id > 0x7FF) return;
    
    // 解析发送时间
    ptr = strtok(NULL, " ");
    can_info.TxTime = atoi(ptr);

    // 解析数据段
    for(int i = 0; i < 8; i++) {
        ptr = strtok(NULL, " ");
        if(!ptr) return;
        can_info.can_data[i] = (uint8_t)strtoul(ptr, NULL, 16);
    }
    
/*************************** 更新至发送列表******************************/
    if(command == 1) {
        can_info.last_time = System_GetTimeMs();
        //更新CAN_TX_LIST
        int8_t ExistPos = CheckIsInCanList(CAN_TX_LIST, can_info.can_id);
        int8_t IdlePos = FindCanListIdleIndex(CAN_TX_LIST);
        if(ExistPos != -1)
        {       //如果该报文已经在CanTxList中，则刷新
            UpdateCanList(CAN_TX_LIST, ExistPos, can_info);
        }
        else if(IdlePos != -1)
        {       //如果不在CanTxList中，则加入找到的空闲位置并刷新
            UpdateCanList(CAN_TX_LIST, IdlePos, can_info);
        }
    }
/**********************************************************************/   
    
/********************************* 测试 *******************************/
#if 0
    static uint8_t cnt = 0;
    static uint8_t cnt_100ms = 0;
    if(TIM2_IrqFlag1 == 1)
    {
        TIM2_IrqFlag1 = 0;
        cnt++;
        if(cnt >= 10)
        {
            cnt = 0;
            cnt_100ms = 1;
        }
    }
	if(cnt_100ms == 1)
    {
        for(int i = 0; i < 8; i++)
        {
            can_info.can_data[i]++;
        }
        cnt_100ms = 0;
    }
#endif
/*************************************************************************/
}

/**
  * 函    数：解析串口发送的报文数据
  * 参    数：data：需解析的字符串
              len:字符串长度
  * 返 回 值：无
  */
static void ProcessUartData(uint8_t *data, uint16_t len) {
    char *saveptr; // 用于strtok_r的上下文指针
    char *line = strtok_r((char *)data, "\r\n", &saveptr);
    
    while(line != NULL) {
        // 跳过空行
        if(strlen(line) < 20)   // 最小有效行长度检查
        {
            line = strtok_r(NULL, "\r\n", &saveptr);
            continue;
        }
        
        ProcessSingleLine(line);
        line = strtok_r(NULL, "\r\n", &saveptr);
    }
}



void UsartCtlCan(void)
{
    static uint8_t CanListInitFlag = 0;
    if(CanListInitFlag == 0)
    {
        CanListInitFlag = 1;
        CanListInit();
    }
    
    CAN_INFO can_info;
    
    LCD_Line(0, 160, 239, 160, WHITE);      //Tx区和Rx区分界线
    
    //处理发送
    if(xUSART.USART1ReceivedNum > 0)
    {
        ProcessUartData(xUSART.USART1ReceivedData, xUSART.USART1ReceivedNum);
        xUSART.USART1ReceivedNum = 0;
    }
    UpdateTimeOutFlag(CAN_TX_LIST);
    //处理接收
    if(CAN_CheckReceived(&can_info.can_id, can_info.can_data))
    {
        can_info.last_time = System_GetTimeMs();
        int8_t ExistPos = CheckIsInCanList(CAN_RX_LIST, can_info.can_id);
        int8_t IdlePos = FindCanListIdleIndex(CAN_RX_LIST);
        if(ExistPos != -1)
        {       //如果该报文已经在CanRxList中，则刷新
            UpdateCanList(CAN_RX_LIST, ExistPos, can_info);
        }
        else if(IdlePos != -1)
        {       //如果不在CanRxList中，则加入找到的空闲位置并刷新
            UpdateCanList(CAN_RX_LIST, IdlePos, can_info);
        }
    }
    UpdateTimeOutFlag(CAN_RX_LIST);
    
    char id[5] = {0};
    char data[25] = {0};
    for(uint8_t i = 0; i < CAN_LIST_NUM; i++)
    {
        if(canTx_list[i].can_info.TimeOutFlag == 0 && canTx_list[i].IdleFlag == 0)
        {
            //将can报文ID和数据解析成字符串，用于LCD显示
            sprintf(id, "%03X ", canTx_list[i].can_info.can_id);
            sprintf(data, "%02X %02X %02X %02X %02X %02X %02X %02X",
                    canTx_list[i].can_info.can_data[0],
                    canTx_list[i].can_info.can_data[1],
                    canTx_list[i].can_info.can_data[2],
                    canTx_list[i].can_info.can_data[3],
                    canTx_list[i].can_info.can_data[4],
                    canTx_list[i].can_info.can_data[5],
                    canTx_list[i].can_info.can_data[6],
                    canTx_list[i].can_info.can_data[7]
            );
            LCD_String(0, SIZE_16_LINE(i), id, 16, RED, BLACK);
            LCD_String(32, SIZE_16_LINE(i), data, 16, GREEN, BLACK);
            LcdFlash(CAN_TX_LIST, i);
        }
        else
        {
            LCD_Fill(28*8, SIZE_16_LINE(i)+2, 29*8, SIZE_16_LINE(i)+12, BLACK);
        }
        
        if(canRx_list[i].can_info.TimeOutFlag == 0 && canRx_list[i].IdleFlag == 0)
        {
            //将can报文ID和数据解析成字符串，用于LCD显示
            sprintf(id, "%03X ", canRx_list[i].can_info.can_id);
            sprintf(data, "%02X %02X %02X %02X %02X %02X %02X %02X",
                    canRx_list[i].can_info.can_data[0],
                    canRx_list[i].can_info.can_data[1],
                    canRx_list[i].can_info.can_data[2],
                    canRx_list[i].can_info.can_data[3],
                    canRx_list[i].can_info.can_data[4],
                    canRx_list[i].can_info.can_data[5],
                    canRx_list[i].can_info.can_data[6],
                    canRx_list[i].can_info.can_data[7]
            );
            LCD_String(0, 170 + SIZE_16_LINE(i), id, 16, RED, BLACK);
            LCD_String(32, 170 + SIZE_16_LINE(i), data, 16, GREEN, BLACK);
            LcdFlash(CAN_RX_LIST, i);
        }
        else
        {
            LCD_Fill(28*8, 170 + SIZE_16_LINE(i)+2, 29*8, 170 + SIZE_16_LINE(i)+12, BLACK);
        }
    }
}

//定时器中断函数，可以复制到使用它的地方
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
        for(int i = 0; i < 2*CAN_LIST_NUM; i++)
        {
            cnt[i]++;
            if(cnt[i] > 1000) cnt[i] = 0;
        }
		TIM2_IrqFlag1 = 1;
        TIM2_IrqFlag2 = 1;
        TIM2_IrqFlag3 = 1;
        TIM2_IrqFlag4 = 1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

void TIM3_IRQHandler(void)
{
    static uint16_t Tim3_cnt = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
        Tim3_cnt++;
        if(Tim3_cnt > 5000) Tim3_cnt = 1;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        
        for(uint8_t i = 0; i < CAN_LIST_NUM; i++)
        {
            if(canTx_list[i].IdleFlag == 0)
            {
                if(Tim3_cnt % canTx_list[i].can_info.TxTime == 0)
                {
                    CAN1_SendData(canTx_list[i].can_info.can_id, canTx_list[i].can_info.can_data);
                    canTx_list[i].can_info.last_time = System_GetTimeMs();
                }
            }
        }
	}
}
