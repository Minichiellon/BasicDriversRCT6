#ifndef _USARTCTLCAN_H_
#define _USARTCTLCAN_H_

#include "stm32f10x.h"                  // Device header

typedef struct
{
    uint32_t can_id;
    uint8_t can_data[8];
    uint16_t TxTime;
    uint16_t TimeOutTime;       //超时时间，单位ms
    uint8_t TimeOutFlag;
    uint64_t last_time;
}CAN_INFO;

typedef struct
{
    uint8_t IdleFlag;
    CAN_INFO can_info;
}CAN_List;

void UsartCtlCan(void);
void CanListInit(void);

#endif
