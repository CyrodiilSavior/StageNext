#include <Arduino.h>
#include "sys_monitor.h"
#include "config.h"
#include "gear_control.h"
#include "automatic_control.h"
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
automatic_control *autoControl;
InputReader *inputReader;
PressureControl *pressureControl;

// For VSS
volatile unsigned long pulseCount = 0;
unsigned long lastMillis = 0;
const unsigned long sampleInterval = 100; // ms (0.1s)
float vssReading = 0.0;

// For serial output 
const unsigned long interval = 1000;  // Interval for serial output in milliseconds
unsigned long previousMillis = 0;

void vssISR() {
  pulseCount++;
}

float readVSS() {
  // Take SpeedSensor sample
  unsigned long now = millis();
  if (now - lastMillis >= sampleInterval) {
    noInterrupts();
    unsigned long count = pulseCount;
    pulseCount = 0;
    interrupts();

    // Frequency in Hz = pulses / (sampleInterval in seconds)
    vssReading = (count * 1000.0) / sampleInterval;
    lastMillis = now;
  }
}

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial.println("Begin - AISIN a760 Controller - Firmware version 0.1");
  pinMode(BUTTON_LOCKUP, INPUT);

  pinMode(SOL_S1, OUTPUT);
  pinMode(SOL_S2, OUTPUT);
  pinMode(SOL_S3, OUTPUT);
  pinMode(SOL_S4, OUTPUT);
  pinMode(SOL_SR, OUTPUT);

  pinMode(SOL_PWM_SL1, OUTPUT);
  pinMode(SOL_PWM_SL2, OUTPUT);
  pinMode(SOL_PWM_SLT, OUTPUT);
  pinMode(SOL_PWM_SLU, OUTPUT);

  pinMode(THROTTLE_INPUT, INPUT);
  pinMode(VSS_INPUT, INPUT_PULLUP);

  pinMode(TEMP_SENSOR, INPUT);
  
  gearControl = new GearControl();
  pressureControl = new PressureControl(gearControl);
  inputReader = new InputReader();
  sysMonitor = new SystemMonitor(gearControl, pressureControl);

  gearControl->begin();
  autoControl = new automatic_control(gearControl);

  attachInterrupt(digitalPinToInterrupt(VSS_INPUT), vssISR, RISING);
}


// Reads inputs, commands pressure controller, gear controller and reports state as JSON to serial
void loop() {
  readVSS();
  InputData inputData = inputReader->read(vssReading);
  pressureControl->setPressureSolenoids(inputData);
  // gearControl->processShiftRequests();
  // autoControl->shouldUpshift(inputData);
  // autoControl->shouldDownshift(inputData);
  autoControl->applyAuto(inputData);
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      // Serial.println(sysMonitor->captureState(inputData));
  }
}