#include <application.h>



void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    if (event==BC_BUTTON_EVENT_CLICK)
    {
        bc_radio_pairing_request(APP_NAME, VERSION);
    }
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

    lcd_init();

    app_init();
}

