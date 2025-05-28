#ifndef _BSPINC_CONF_H_
#define _BSPINC_CONF_H_

#include "bsp_led.h"            // LED�����ļ�
#include "bsp_key.h"            // KEY�����ļ�
#include "bsp_usart.h"          // ���ڴ����ļ�
#include "bsp_can.h"            // CAN�����ļ�
#include "bsp_tim.h"            // ��ʱ�������ļ�
#include "bsp_OLED.h"           // OLED�����ļ�
#include "bsp_IC.h"             // ���벶������ļ�
#include "bsp_pwm.h"            // pwm�����ļ�
#include "bsp_ADC.h"            // ADC�����ļ�
#include "bsp_MPU6050.h"        // MPU6050�����ļ�
#include "bsp_W25Q64.h"         // W25Q64�����ļ�
#include "bsp_Flash.h"          // Flash�����ļ�
#include "bsp_w25qxx.h"         // w25qxx�����ļ�
#include "bsp_lcd_ili9341.h"    // Lcd�����ļ�

void bsp_init(void);
void bsp_test(void);

#endif
