#ifndef __BSP_TIM_H
#define __BSP_TIM_H

#include <stm32f10x.h>
#include "bsp_usart.h"
#include "stm32f10x_conf.h"

extern uint16_t TIM2_Num;
extern uint8_t TIM2_IrqFlag;

extern void Timer_Init(void);

#endif



