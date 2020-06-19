#include <application.h>

#ifdef APP1

#define R 340 // 2x680 ohm +-3% in parallel
#define U 230 // socket voltage DC
#define RATIO 1000 // sensor measuring ratio  

#define VDDA 3.3f   // reference voltage for measuring
#define ZERO 0.001  // zero voltage

#define RADIO_TOPIC "wattmeter"  // subtopic used to radio message 

#define MEASURING_TIME 100       // how long the voltage is measured
#define MEASURING_PERIOD 5000    // delay between 2 measurements

#define COIL_1 BC_ADC_CHANNEL_A2  // AD channel measuring voltage on coil 1
#define COIL_2 BC_ADC_CHANNEL_A3  // AD channel measuring voltage on coil 2

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

    float voltMax = (max * VDDA) / 65536.0f;
    if (voltMax<ZERO)
        voltMax = 0;        // to eliminate false induced current 

    float rms = sqrt(2)/2;

    float currentSensor = voltMax / R;
    float current = currentSensor * RATIO;
    float currentEff = current * rms;

    return currentEff;
}

void app_init()
{
    bc_adc_resolution_set(COIL_1, BC_ADC_RESOLUTION_12_BIT);
    bc_adc_oversampling_set(COIL_1, BC_ADC_OVERSAMPLING_256);
    
    bc_adc_resolution_set(COIL_2, BC_ADC_RESOLUTION_12_BIT);
    bc_adc_oversampling_set(COIL_2, BC_ADC_OVERSAMPLING_256);
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

    bc_log_debug("I=%.2fA    P=%.2fW     diff=%.3fmA", c2, power, (c1-c2)*1000);

    bc_led_set_mode(&_led, BC_LED_MODE_OFF);

    lcd_write_watt((c1 + c2) / 2, power, warning);

    bc_scheduler_plan_current_from_now(MEASURING_PERIOD-2*MEASURING_TIME);
}
#endif
