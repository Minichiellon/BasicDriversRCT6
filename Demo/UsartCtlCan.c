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

#define CAN_LIST_NUM    5
CAN_List canTx_list[CAN_LIST_NUM];
CAN_List canRx_list[CAN_LIST_NUM];

int8_t FindCanListIdleIndex(uint8_t CanListType)
{
    if(CanListType == 1)
    {
        for(int i = 0; i < CAN_LIST_NUM; i++)
        {
            if(canTx_list[i].IdleFlag == 0)
            {
                return i;
            }
        }
        return -1;
    }
    else
    {
        for(int i = 0; i < CAN_LIST_NUM; i++)
        {
            if(canRx_list[i].IdleFlag == 0)
            {
                return i;
            }
        }
        return -1;
    }
}

int8_t CheckIsInCanList(uint8_t CanListType, uint32_t CanId)
{
    if(CanListType == 1)
    {
        for(int i = 0; i < CAN_LIST_NUM; i++)
        {
            if(CanId == canTx_list[i].can_info.can_id)
            {
                return i;
            }
        }
        return -1;
    }
    else
    {
        for(int i = 0; i < CAN_LIST_NUM; i++)
        {
            if(CanId == canRx_list[i].can_info.can_id)
            {
                return i;
            }
        }
        return -1;
    }
}

void InsertCanList(uint8_t CanListType, uint8_t Pos, CAN_INFO CanInfo)
{
    if(CanListType == 1)
    {
        canTx_list[Pos].can_info.can_id = CanInfo.can_id;
        memcpy(canTx_list[Pos].can_info.can_data, CanInfo.can_data, 8);
        canTx_list[Pos].IdleFlag = 1;
    }
}

void ProcessSingleLine(char *line) {
    
    CAN_INFO can_info;
    // 跳过行首空白字符
    while(*line == ' ' || *line == '\r') line++;
    
    char *ptr = strtok(line, " ");
    if(!ptr) return;

    // 解析命令位
    uint8_t command = atoi(ptr);
    if(command != 0 && command != 1) return;

    // 解析CAN ID（关键修改）
    ptr = strtok(NULL, " ");
    if(!ptr) return;
    
    // 使用strtoul直接转换完整十六进制字符串
    can_info.can_id = strtoul(ptr, NULL, 16);
    
    // 验证ID范围（标准帧ID范围0x000-0x7FF）
    if(can_info.can_id > 0x7FF) return;

    static uint8_t cnt = 0;
    static uint8_t cnt_100ms = 0;
    if(TIM2_IrqFlag == 1)
    {
        cnt++;
        if(cnt >= 10)
        {
            cnt = 0;
            cnt_100ms = 1;
        }
    }
    // 解析数据段（保持原逻辑）
    for(int i = 0; i < 8; i++) {
        ptr = strtok(NULL, " ");
        if(!ptr) return;
        can_info.can_data[i] = (uint8_t)strtoul(ptr, NULL, 16);
    }
	if(cnt_100ms == 1)
    {
        for(int i = 0; i < 8; i++)
        {
            can_info.can_data[i]++;
        }
        cnt_100ms = 0;
    }
    char id[5] = {0};
    char data[25] = {0};
	if(command == 1) {
        CAN1_SendData(can_info.can_id, can_info.can_data); // 假设CAN发送函数已实现
        sprintf(id, "%03X ", can_info.can_id);
        sprintf(data, "%02X %02X %02X %02X %02X %02X %02X %02X", can_info.can_data[0], can_info.can_data[1], can_info.can_data[2], 
                can_info.can_data[3], can_info.can_data[4], can_info.can_data[5], can_info.can_data[6], can_info.can_data[7]);
        
        int pos = CheckIsInCanList(1, can_info.can_id);
        int IdlePos = FindCanListIdleIndex(1);
        if(pos != -1)
        {
            LCD_String(0, SIZE_16_LINE(pos), id, 16, RED, BLACK);
            LCD_String(32, SIZE_16_LINE(pos), data, 16, GREEN, BLACK);
        }
        else if(IdlePos != -1)
        {
            InsertCanList(1, IdlePos, can_info);
            LCD_String(0, SIZE_16_LINE(IdlePos), id, 16, RED, BLACK);
            LCD_String(32, SIZE_16_LINE(IdlePos), data, 16, GREEN, BLACK);
        }
//        LCD_String(0, 0, id, 16, RED, BLACK);
//        LCD_String(32, 0, data, 16, GREEN, BLACK);
		System_DelayMS(1);
    }
}

void ProcessUartData(uint8_t *data, uint16_t len) {
    char *saveptr; // 用于strtok_r的上下文指针
    char *line = strtok_r((char *)data, "\r\n", &saveptr);
    
    while(line != NULL) {
        // 跳过空行
        if(strlen(line) < 20) { // 最小有效行长度检查
            line = strtok_r(NULL, "\r\n", &saveptr);
            continue;
        }
        
        ProcessSingleLine(line);
        line = strtok_r(NULL, "\r\n", &saveptr);
    }
}

void UsartCtlCan(void)
{
    LCD_Line(0, 160, 239, 160, WHITE);
    if(xUSART.USART1ReceivedNum > 0)
    {
        ProcessUartData(xUSART.USART1ReceivedData, xUSART.USART1ReceivedNum);
        xUSART.USART1ReceivedNum = 0;
    }
    
    char id[5] = {0};
    char data[25] = {0};
    if(xCAN.RxFlag == 1)                        // 通过接收标志xCan.RxFlag，判断是否收到新报文
    {
        sprintf(id, "%03X ", xCAN.RxData.StdId);
        sprintf(data, "%02X %02X %02X %02X %02X %02X %02X %02X", xCAN.RxData.Data[0], xCAN.RxData.Data[1], xCAN.RxData.Data[2], 
                xCAN.RxData.Data[3], xCAN.RxData.Data[4], xCAN.RxData.Data[5], xCAN.RxData.Data[6], xCAN.RxData.Data[7]);
        
        LCD_String(0, 170, id, 16, RED, BLACK);
        LCD_String(32, 170, data, 16, GREEN, BLACK);
        
        xCAN.RxFlag = 0;                       // 处理完数据了，把接收标志清0                        
    }
}

