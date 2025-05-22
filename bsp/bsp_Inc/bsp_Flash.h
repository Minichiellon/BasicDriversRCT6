#ifndef __Flash_H
#define __Flash_H

#include "stm32f10x.h"                  // Device header

extern uint32_t Flash_ReadWord(uint32_t Address);
extern uint16_t Flash_ReadHalfWord(uint32_t Address);
extern uint8_t Flash_ReadByte(uint32_t Address);

extern void Flash_EraseAllPages(void);
extern void Flash_ErasePage(uint32_t PageAddress);

extern void Flash_ProgramWord(uint32_t Address, uint32_t Data);
extern void Flash_ProgramHalfWord(uint32_t Address, uint16_t Data);

#endif
