# Watt Meter

Example how to measure power consumption using inductive current sensor.

Visit [Hackster.io](https://www.hackster.io/matejus/wattmetter-with-bigclown-a0ec12) to read the story.

The consumed power is measured using inductive currency sensor. The measured value is read by A/D channel as voltage on resitor (R = 340ohm) and have to be recalculated back to current. Because of the sensor is measuring AC currency the input value have to be measured more times to get peak current (5 periods of 50Hz - MEASURING_TIME = 100ms).

The peak current have to be recalculated to effective current. Then using the sensor ratio (RATIO = 1000) and electricity voltage (U = 230V) is possible to calculate the power consumption.

`P = U * I`

- where U is electricity voltage
- and I is calculated effective current

`I = Ip * SQRT(2)/2 * RATIO`

- where Ip is measured peak current
- and RATIO is sensor's induction ratio

`Ip = MAX(Um / R)` for 5 periods

- where U is measured voltage
- and R is resistance used to measure currency, placed between pins of sensor. Max available resistance is 3.3V/5mA=660ohm, but use smaller for sure.

Finally the measured currency and power consumption is written to the LCD display and sent over the radio to computer.

I had used 2 current sensors on L and N wire (named COIL_1 and COIL_2). the solution now can compare currency on both wires (L and N) and could be identical. When there is difference higher than measurement error then it is alerted on display.
If you want to use different ADC to measuring change the definitions at the begining of application.c file. You can change measuring period (5 sec by defaut) by modification of MEASURING_PERIOD constant.

> [!WARNING]
> You are playing with voltage of 230 V which is life threating. Be carefoul, keep everything isolated and dont touch the wires when plug is in socket!
