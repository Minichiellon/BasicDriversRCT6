#ifndef __IC_H
#define __IC_H

#include "stm32f10x_conf.h"
#include "stm32f10x.h"                  // Device header

extern void IC_Init(void);
extern uint32_t IC_GetFreq(void);
extern uint32_t IC_GetDuty(void);

#endif
