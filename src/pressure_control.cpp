#include <pressure_control.h>

PressureControl::PressureControl() {
  this->lockupState = false;
  this->pressureControl = 1023;
}

void PressureControl::setPressureSolenoids(InputData input) {
  this->pressureControl = 255-input.PressureControl/4;
  analogWrite(SOL_PWM_SL1, this->pressureControl);
  analogWrite(SOL_PWM_SL2, this->pressureControl);
  analogWrite(SOL_PWM_SLT, this->pressureControl);
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

int PressureControl::getPressureSetting() {
  return this->pressureControl;
}