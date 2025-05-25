#ifndef __BSP_TIM_H
#define __BSP_TIM_H

#include <stm32f10x.h>
#include "bsp_usart.h"

extern uint16_t TIM2_Num;
extern uint8_t TIM2_IrqFlag1;
extern uint8_t TIM2_IrqFlag2;
extern uint8_t TIM2_IrqFlag3;
extern uint8_t TIM2_IrqFlag4;

extern void Timer_Init(void);

#endif



