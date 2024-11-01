#include <Arduino.h>
#include "sys_monitor.h"
#include "defines.h"
#include "gear_control.h"
#include "input_reader.h"
#include "input_data.h"
#include "pressure_control.h"

/*
  Project STAGE4 - 6 Speed Prototype V1 (Manually Controlled Only)
  In theory this code will make an Aisin a760 work in an Isuzu. 
  It'll also lay the ground-work for building this platform for other
  aisin 30-40le equipt vehicles to have a seamless swap.
*/


SystemMonitor *sysMonitor;
GearControl *gearControl;
InputReader *inputReader;
PressureControl *pressureControl;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial.println("Begin - AISIN a760 Controller - Firmware version 0.1");
  pinMode(BUTTON_UPSHIFT, INPUT);
  pinMode(BUTTON_DOWNSHIFT, INPUT);
  pinMode(BUTTON_LOCKUP, INPUT);

  // pinMode(POT_SL1, INPUT);
  // pinMode(POT_SL2, INPUT);
  // pinMode(POT_SLT, INPUT);


  pinMode(SOL_S1, OUTPUT);
  pinMode(SOL_S2, OUTPUT);
  pinMode(SOL_S3, OUTPUT);
  pinMode(SOL_S4, OUTPUT);
  pinMode(SOL_SR, OUTPUT);

  pinMode(SOL_PWM_SL1, OUTPUT);
  pinMode(SOL_PWM_SL2, OUTPUT);
  pinMode(SOL_PWM_SLT, OUTPUT);
  pinMode(SOL_PWM_SLU, OUTPUT);

  pinMode(POT_GLOBAL, INPUT);

  pinMode(TEMP_SENSOR, INPUT);
  
  gearControl = new GearControl();
  pressureControl = new PressureControl();
  inputReader = new InputReader();
  sysMonitor = new SystemMonitor(gearControl, pressureControl);

  digitalWrite(13,0);
}

const unsigned long interval = 1000;  // Interval for serial output in milliseconds
unsigned long previousMillis = 0;

// Reads inputs, commands pressure controller, gear controller and reports state as JSON to serial
void loop() {
  InputData inputData = inputReader->read();
  pressureControl->setPressureSolenoids(inputData);
  pressureControl->setLockup(inputData);
  gearControl->processInputData(inputData);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      Serial.println(sysMonitor->captureState(inputData));
  }
}