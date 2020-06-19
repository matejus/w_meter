#include <application.h>

#ifdef APP2

#define SENSOR_PIN BC_ADC_CHANNEL_A2   // AD channel measuring output from ACS712 sensor
#define BATTERY_PIN BC_ADC_CHANNEL_A3  // AD channel measuring voltage from battery dividing bridge

#define DB_RATIO (218.0/899.0)         // dividing ratio on bridge
#define ACS_CONST 0.185f               // converting sensors ratio .185V/A
#define ACS_ERR 0.014f                 // noise voltage on sensor

float _current;
float _battery;

// returns current in mA
float measure_acs712()
{
    float v, vdda;
    bc_adc_async_get_voltage(SENSOR_PIN, &v);
    bc_adc_get_vdda_voltage(&vdda);

    v = v - ACS_ERR - (vdda/2);
    float c = v / ACS_CONST;
    return c;
}

float measure_battery()
{
    float v;
    bc_adc_async_get_voltage(BATTERY_PIN, &v);
    return v / DB_RATIO;
}


void measure_event(bc_adc_channel_t channel, bc_adc_event_t event, void *param)
{
    if (channel==SENSOR_PIN)
    {
        _current = measure_acs712();
        lcd_write_amps(_current, _battery);
    }
    else if (channel==BATTERY_PIN)
    {
        _battery = measure_battery();
    }
}

void app_init()
{
    bc_adc_resolution_set(SENSOR_PIN, BC_ADC_RESOLUTION_12_BIT);
    bc_adc_oversampling_set(SENSOR_PIN, BC_ADC_OVERSAMPLING_256);
    bc_adc_set_event_handler(SENSOR_PIN, measure_event, NULL);
    
    bc_adc_resolution_set(BATTERY_PIN, BC_ADC_RESOLUTION_12_BIT);
    bc_adc_oversampling_set(BATTERY_PIN, BC_ADC_OVERSAMPLING_256);
    bc_adc_set_event_handler(BATTERY_PIN, measure_event, NULL);

    bc_led_pulse(&_led, 2000);
}



void application_task(void)
{
    bc_adc_async_measure(SENSOR_PIN);
    bc_adc_async_measure(BATTERY_PIN);

    bc_scheduler_plan_current_from_now(1000);
}
#endif