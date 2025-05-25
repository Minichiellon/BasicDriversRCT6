#ifndef _USARTCTLCAN_H_
#define _USARTCTLCAN_H_

#include "stm32f10x.h"                  // Device header

typedef struct
{
    uint32_t can_id;
    uint8_t can_data[8];
}CAN_INFO;

typedef struct
{
    uint8_t IdleFlag;
    uint16_t TimeOutFlag;
    CAN_INFO can_info;
}CAN_List;

extern void UsartCtlCan(void);

#endif
