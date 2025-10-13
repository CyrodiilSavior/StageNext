// On/Off Solenoids
#define SOL_S1 8
#define SOL_S2 4
#define SOL_S3 5
#define SOL_S4 6
#define SOL_SR 7

// PWM Solenoids
#define SOL_PWM_SL1 9 // Clutch pressure control & Accumulator back pressure control
#define SOL_PWM_SL2 10 // Brake pressure control 
#define SOL_PWM_SLT 11 // Line pressure control & Accumulator back pressure control 
#define SOL_PWM_SLU 3 // Lock-up clutch pressure control

// Manual Inputs
#define REVERSE_SW 12
#define MANUAL_SW A3
#define BUTTON_UPSHIFT A1
#define BUTTON_DOWNSHIFT A2

// Automatic Inputs
#define THROTTLE_INPUT A4
#define VSS_INPUT 2

#define TEMP_SENSOR A0

#ifndef CONFIG_H
#define CONFIG_H
#endif
