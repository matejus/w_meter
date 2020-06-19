#include <lcd.h>

void lcd_init(){
    // Initialize LCD
    bc_module_lcd_init();

    bc_module_lcd_clear();
    bc_module_lcd_set_font(&bc_font_ubuntu_24);
    bc_module_lcd_draw_string(20, 25, APP_NAME, true);
    bc_module_lcd_set_font(&bc_font_ubuntu_13);
    bc_module_lcd_draw_string(25, 100, AUTHOR, true);    
    bc_module_lcd_update();
}

void lcd_write_watt(float current, float power, bool warning)
{
    bc_module_lcd_clear();

    char buffer[10];
    sprintf(buffer, "%05.1f W", power);
    bc_module_lcd_set_font(&bc_font_ubuntu_28);
    bc_module_lcd_draw_string(10, 30, buffer, true);

    sprintf(buffer, "~ %3.2f A", current);
    bc_module_lcd_set_font(&bc_font_ubuntu_15);
    bc_module_lcd_draw_string(40, 70, buffer, true);

    if (warning)
    {
        bc_module_lcd_draw_string(40,100,"!!!!!!!!!!!!", true);
    }

    bc_module_lcd_update();
}


void lcd_write_amps(float current, float voltage)
{
    char buffer[10];

    bc_module_lcd_clear();
    bc_module_lcd_set_font(&bc_font_ubuntu_15);
    bc_module_lcd_draw_string(15, 10, "Consumption", true);
    bc_module_lcd_draw_string(30, 70,   "Battery", true);

    bc_module_lcd_set_font(&bc_font_ubuntu_24);
    sprintf(buffer, "%05.1f mA", current*1000);
    bc_module_lcd_draw_string(10, 30, buffer, true);

    sprintf(buffer, "%3.2f V", voltage);
    bc_module_lcd_draw_string(20, 90, buffer, true);

    bc_module_lcd_update();
}