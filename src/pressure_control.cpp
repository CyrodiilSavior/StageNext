#include <pressure_control.h>

PressureControl::PressureControl(GearControl *gc) {
  this->gearControl = gc;
  this->lockupState = false;
  this->sl1Pressure = 0;
  this->sl2Pressure = 0;
  this->sltPressure = 0;
}

void PressureControl::setPressureSolenoids(InputData input) {

  this->sl1Pressure = this->calculateSL1Pressure(input.ThrottlePercent);
  this->sl2Pressure = this->calculateSL2Pressure(input.ThrottlePercent);
  this->sltPressure = this->calculateSLTPressure(input.ThrottlePercent, 30, 85);

  analogWrite(SOL_PWM_SL1, this->sl1Pressure);
  analogWrite(SOL_PWM_SL2, this->sl2Pressure);
  analogWrite(SOL_PWM_SLT, this->sltPressure);
}

void PressureControl::setLockup(InputData input) {
  this->lockupState = input.LockupMode;
  if (this->lockupState) {
    analogWrite(SOL_PWM_SLU, 1023);
  } else {
    analogWrite(SOL_PWM_SLU, 0);
  }
}

bool PressureControl::getLockupState() {
  return this->lockupState;
}

// For iteration 1 of this design, if 5th and 6th gear, go to 20% duty cycle (ON)
// 1st - 4th gear go to 80% duty cycle (OFF)
int PressureControl::calculateSL1Pressure(int throttlePercent) {
  if (this->gearControl->getCurrentGear() > 4) {
    return 0;
  }
  return 620;
}

// Reverse that for this solenoid
int PressureControl::calculateSL2Pressure(int throttlePercent) {
  if (this->gearControl->getCurrentGear() < 4) {
    return 0;
  }
  return 620;
}

// Calculates duty cycle for the SLT Line pressure solenoid. 
// Min is the minimum percent of possible pressure at lowest throttle
// Max is the maximum percent of possible pressure at highest throttle
// Eg: min=20, max=80 -> at 0% Throttle pressure will be at 20% (analogWrite 205) at 100% throttle it'll be 80% (analogWrite 818)
int PressureControl::calculateSLTPressure(int throttlePercent, int min, int max) {
  if(this->gearControl->getCurrentGear() >= 5) {
    min = 20;
    max = 20;
  }

  throttlePercent = constrain(throttlePercent, 0, 100);

  int dutyMin = (int)((1023.0f * min) / 100.0f);  // e.g. 20% → ~205
  int dutyMax = (int)((1023.0f * max) / 100.0f);  // e.g. 80% → ~818
  int dutyCycle = map(throttlePercent, 0, 100, dutyMax, dutyMin);
  
  dutyCycle = constrain(dutyCycle, min(dutyMin, dutyMax), max(dutyMin, dutyMax));
  return dutyCycle;
}

int PressureControl::getSL1PressureSetting() {
  return this->sl1Pressure;
}

int PressureControl::getSL2PressureSetting() {
  return this->sl2Pressure;
}

int PressureControl::getSLTPressureSetting() {
  return this->sltPressure;
}