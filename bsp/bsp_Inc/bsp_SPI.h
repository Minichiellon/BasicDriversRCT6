#ifndef __MYSPI_H
#define __MYSPI_H

#include "stm32f10x.h"                  // Device header

void SPI1_Init(void);
void SPI1_Start(void);
void SPI1_Stop(void);
uint8_t SPI1_SwapByte(uint8_t ByteSend);

#endif
