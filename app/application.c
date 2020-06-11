#include <application.h>

#define R 340 // 2x680 ohm +-10% in parallel
#define U 230 // socket voltage DC
#define RATIO 1000 // sensor measuring ratio  

#define VDDA 3.3f   // reference voltage for measuring
#define ZERO 0.001  // zero voltage

#define RADIO_TOPIC "wattmeter"  // subtopic used to radio message 

#define MEASURING_TIME 100       // how long the voltage is measured
#define MEASURING_PERIOD 5000    // delay between 2 measurements

#define COIL_1 BC_ADC_CHANNEL_A2  // AD channel measuring voltage on coil 1
#define COIL_2 BC_ADC_CHANNEL_A3  // AD channel measuring voltage on coil 2

bc_led_t _led;
bc_button_t _button;


float measure_current(bc_adc_channel_t channel)
{
    uint16_t adc;
    uint16_t max = 0;
    bc_tick_t start = bc_tick_get();

    while (bc_tick_get()<start+MEASURING_TIME)
    {
        bc_adc_get_value(channel, &adc);
        if (adc>max)
            max = adc; 
    } 

    float voltMax = (max * VDDA) / 65536.f;
    if (voltMax<ZERO)
        voltMax = 0;        // to eliminate false induced current 

    float rms = sqrt(2)/2;

    float currentSensor = voltMax / R;
    float current = currentSensor * RATIO;
    float currentEff = current * rms;

    return currentEff;
}


void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    if (event==BC_BUTTON_EVENT_CLICK)
    {
        bc_radio_pairing_request(APP_NAME, VERSION);
    }
}

void lcd_init(){
    // Initialize LCD
    bc_module_lcd_init();

    bc_module_lcd_clear();
    bc_module_lcd_set_font(&bc_font_ubuntu_24);
    bc_module_lcd_draw_string(20, 25, APP_NAME, true);
    bc_module_lcd_set_font(&bc_font_ubuntu_13);
    bc_module_lcd_draw_string(25, 100, "(C) 2020 Matej", true);    
    bc_module_lcd_update();

    // Initialize LCD buttons
    static bc_button_t lcd_left;
    bc_button_init_virtual(&lcd_left, BC_MODULE_LCD_BUTTON_LEFT, bc_module_lcd_get_button_driver(), false);
    bc_button_set_event_handler(&lcd_left, button_event_handler, NULL);
    static bc_button_t lcd_right;
    bc_button_init_virtual(&lcd_right, BC_MODULE_LCD_BUTTON_RIGHT, bc_module_lcd_get_button_driver(), false);
    bc_button_set_event_handler(&lcd_right, button_event_handler, NULL);
}

void lcd_write(float current, float power, bool warning)
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

// Application initialization function which is called once after boot
void application_init(void)
{
    bc_log_init(BC_LOG_LEVEL_DEBUG, BC_LOG_TIMESTAMP_OFF);

    bc_radio_init(BC_RADIO_MODE_NODE_SLEEPING);
    
    bc_led_init(&_led, BC_GPIO_LED, false, false);
    bc_button_init(&_button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&_button, button_event_handler, NULL);
    
    bc_adc_init();
    bc_adc_resolution_set(COIL_1, BC_ADC_RESOLUTION_12_BIT);
    bc_adc_oversampling_set(COIL_1, BC_ADC_OVERSAMPLING_256);
    //bc_adc_set_event_handler(COIL_1, _adc_event_handler, NULL);
    
    bc_adc_resolution_set(COIL_2, BC_ADC_RESOLUTION_12_BIT);
    bc_adc_oversampling_set(COIL_2, BC_ADC_OVERSAMPLING_256);
    //bc_adc_set_event_handler(COIL_2, _adc_event_handler, NULL);

    lcd_init();
}

// Application task function (optional) which is called peridically if scheduled
void application_task(void)
{
    float c1, c2;
    float power;
    bool warning;

    bc_led_set_mode(&_led, BC_LED_MODE_ON);

    c1 = measure_current(COIL_1);
    c2 = measure_current(COIL_2);

    power = U * (c1 + c2) / 2;

    warning = abs((c1-c2)*1000)>1;

    char msg[100];
    sprintf(msg, "I=%.2fA    P=%.2fW     diff=%.3fmA", c2, power, (c1-c2)*1000);
    bc_log_debug("%s", msg);

    bc_led_set_mode(&_led, BC_LED_MODE_OFF);

    lcd_write((c1 + c2) / 2, power, warning);

    bc_scheduler_plan_current_from_now(MEASURING_PERIOD-2*MEASURING_TIME);
}

