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
- and R is resistance used to measure currency, placed between pins of sensor

Finally the measured currency and power consumption is written to the LCD display and sent over the radio to computer.