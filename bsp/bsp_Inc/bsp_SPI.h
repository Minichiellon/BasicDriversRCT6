#ifndef __MYSPI_H
#define __MYSPI_H

#include "stm32f10x.h"                  // Device header

extern void SPI1_Init(void);
extern void SPI1_Start(void);
extern void SPI1_Stop(void);
extern uint8_t SPI1_SwapByte(uint8_t ByteSend);

#endif
