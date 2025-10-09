
## 🧰 Hardware

- Arduino Nano / Uno / Mega (or equivalent)
- Custom MOSFET driver board for solenoid control
- Analog Signal Isolation module (DFRobot or equivalent)
- LM2917 or MAX9926 for speed signal conditioning
- Passive 2-wire VR speed sensor (e.g., factory Toyota/Isuzu output sensor)
- TPS sensor input (0–5V analog)

## 🧠 How it Works

### Solenoid Control

Solenoids are controlled based on throttle input and speed. `ShiftLogic` defines when gears change. `PressureControl` applies the appropriate PWM duty cycle to the pressure control solenoids (SLT/SLU).

### Throttle Input

The TPS is electrically isolated and read through an analog isolator. The 0–5V signal is interpreted for load-based shift control and SLT pressure tuning.

### Speed Input

A 2-wire VR sensor outputs AC pulses based on output shaft speed. These are converted to digital pulses using a MAX9926 signal conditioner. Pulse frequency is then translated into vehicle speed.

### Manual Mode (Optional)

You can define fixed gears using buttons or a rotary switch to override automatic logic.

Edit `config.h` to change pin mappings, pressure thresholds, shift points, etc.

```cpp
#define SOL_S1 3
#define SOL_S2 5
#define SOL_S3 6
#define SOL_S4 9
#define SOL_SLT 10
#define SOL_SLU 11

#define TPS_PIN A0
#define SPEED_PIN 2
