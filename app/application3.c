#include <application.h>

#ifdef APP3

#define R 340       // 2x680 ohm +-1% in parallel
#define VDDA 3.3f   // ref voltage
#define COIL_1 BC_ADC_CHANNEL_A2  // AD channel measuring voltage on coil 1
#define COIL_2 BC_ADC_CHANNEL_A3  // AD channel for comparation measurement on coil2
#define RATIO 1000 // sensor measuring ratio  

float measure_current(bc_adc_channel_t channel)
{
    uint16_t adc;

    bc_adc_get_value(channel, &adc);
    float volt = (adc * VDDA) / 65536.0f;

    float current = RATIO * (volt / R);

    return current;
}

void app_init()
{
    bc_adc_resolution_set(COIL_1, BC_ADC_RESOLUTION_12_BIT);
    bc_adc_oversampling_set(COIL_1, BC_ADC_OVERSAMPLING_256);

    bc_adc_resolution_set(COIL_2, BC_ADC_RESOLUTION_12_BIT);
    bc_adc_oversampling_set(COIL_2, BC_ADC_OVERSAMPLING_256);

    bc_led_pulse(&_led, 2000);
}

// Application task function (optional) which is called peridically if scheduled
void application_task(void)
{
    float c1, c2;

    c1 = measure_current(COIL_1);
    c2 = measure_current(COIL_2);

    float c = c1-c2;

    lcd_write_amps(c, 0);

    bc_log_info("I=%3.2fA  ref=%3.2fA", c1, c2);

    bc_scheduler_plan_current_from_now(1000);
}


#endif