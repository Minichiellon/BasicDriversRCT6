#ifndef _USARTCTLCAN_H_
#define _USARTCTLCAN_H_

#include "stm32f10x.h"                  // Device header

typedef struct CAN_INFO
{
    uint32_t can_id;
    uint8_t can_data[8];
}CAN_INFO;

typedef struct CAN_List
{
    uint8_t IdleFlag;
    CAN_INFO can_info;
}CAN_List;

extern void UsartCtlCan(void);

#endif
