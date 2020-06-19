//#define APP1
//#define APP2
#define APP3

#ifndef _APPLICATION_H
#define _APPLICATION_H

#ifndef VERSION
#define VERSION "0.1"
#endif

#ifndef APP_NAME
#ifdef APP1
#define APP_NAME "W-meter"
#endif
#ifdef APP2
#define APP_NAME "B-check"
#endif
#ifdef APP3
#define APP_NAME "A-meter"
#endif
#endif

#ifndef AUTHOR
#define AUTHOR "(C) 2020 Matej"
#endif 


#include <bcl.h>
#include <bc_module_lcd.h>
#include "lcd.h"


// shared properties
bc_led_t _led;
bc_button_t _button;

// declaration of app version dependent part
void app_init();


#endif  
// _APPLICATION_H
