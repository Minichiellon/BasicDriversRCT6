#ifndef __PWM_H
#define __PWM_H

#include <stm32f10x.h>  
#include <stdio.h>
#include "stm32f10x_conf.h"

extern void PWM_Init(void);
extern void PWM_SetCompare1(uint16_t Compare);

#endif

