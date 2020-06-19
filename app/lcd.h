#ifndef _LCD_H
#define _LCD_H


#include <bcl.h>
#include <bc_module_lcd.h>
#include "application.h"

void lcd_init();
void lcd_write_watt(float current, float power, bool warning);
void lcd_write_amps(float current, float voltage);

#endif
