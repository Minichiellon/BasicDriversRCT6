#ifndef _BSPINC_CONF_H_
#define _BSPINC_CONF_H_

#include "bsp_led.h"            // LED代码文件
#include "bsp_key.h"            // KEY代码文件
#include "bsp_usart.h"          // 串口代码文件
#include "bsp_can.h"            // CAN代码文件
#include "bsp_tim.h"            // 定时器代码文件
#include "bsp_OLED.h"           // OLED代码文件
#include "bsp_IC.h"             // 输入捕获代码文件
#include "bsp_pwm.h"            // pwm代码文件
#include "bsp_ADC.h"            // ADC代码文件
#include "bsp_MPU6050.h"        // MPU6050代码文件
#include "bsp_W25Q64.h"         // W25Q64代码文件
#include "bsp_Flash.h"          // Flash代码文件
#include "bsp_w25qxx.h"         // w25qxx代码文件
#include "bsp_lcd_ili9341.h"    // Lcd代码文件

void bsp_init(void);
void bsp_test(void);

#endif
