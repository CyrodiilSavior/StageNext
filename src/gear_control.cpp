#include "gear_control.h"
#include "defines.h"

GearControl::GearControl() {
  this->setCurrentGear(1);
}

void GearControl::outputSignalToSolenoids(int s1, int s2, int s3, int s4, int sR) { 
  digitalWrite(SOL_S1, s1);
  digitalWrite(SOL_S2, s2);
  digitalWrite(SOL_S3, s3);
  digitalWrite(SOL_S4, s4);
  digitalWrite(SOL_SR, sR);
}

void GearControl::processInputData(InputData data) {
  unsigned long currentTime = millis(); // Get current time in milliseconds
  
  if (currentTime - lastTimeCheck >= 2000) {
    lastTimeCheck = currentTime;
    if (data.UpshiftRequested || this->lastMomentaryCommand == MOMENTARY_COMMAND_UPSHIFT) {
      this->lastMomentaryCommand = "";
      this->upshift();
    } 
    if (data.DownshiftRequested || this->lastMomentaryCommand == MOMENTARY_COMMAND_DOWNSHIFT) {
      this->lastMomentaryCommand = "";
      this->downshift();
    }
  } else {
    if (data.UpshiftRequested) {
      this->lastMomentaryCommand = MOMENTARY_COMMAND_UPSHIFT;
    } 
    if (data.DownshiftRequested) {
      this->lastMomentaryCommand = MOMENTARY_COMMAND_DOWNSHIFT;
    }
  }
  return;
}

void GearControl::setCurrentGear(int input) {
  this->currentGear = input;
  switch (input) {
  case 1:
    this->outputSignalToSolenoids(LOW, HIGH, HIGH, LOW, HIGH);
    break;
  case 2:
    this->outputSignalToSolenoids(HIGH, HIGH, HIGH, LOW, HIGH);
    break;
  case 3:
    this->outputSignalToSolenoids(HIGH, LOW, HIGH, LOW, HIGH);
    break;
  case 4:
    this->outputSignalToSolenoids(HIGH, LOW, LOW, LOW, HIGH);
    break;
  case 5:
    this->outputSignalToSolenoids(HIGH, LOW, LOW, HIGH, LOW);
    break;
  case 6:
    this->outputSignalToSolenoids(HIGH, HIGH, LOW, HIGH, LOW);
    break;
  }
}

int GearControl::getCurrentGear() {
  return this->currentGear;
}

bool GearControl::upshift() {
  if (this->currentGear < 6) {
    this->setCurrentGear(this->currentGear + 1);
    return true;
  }
  return false;
}

bool GearControl::downshift() {
  if (this->currentGear > 1) {
    this->setCurrentGear(this->currentGear - 1);
    return true;
  }
  return false;
}