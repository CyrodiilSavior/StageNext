#include "automatic_control.h"  

int automatic_control::getThrottleBand(int throttlePercent) {
  for (int i = 0; i < 6; i++) {
      if (throttlePercent <= throttleBands[i]) {
          return i;  // matched this band
      }
  }
  return 5; // default to last band (85-100%) if out of range
}

bool automatic_control::shouldUpshift(InputData inputData) {
  int band = this->getThrottleBand(inputData.ThrottlePercent);
  int captured = inputData.vssHz;
  if (captured > upshiftMap[band][gearControl->getCurrentGear()-1]) {
    Serial.println("UPSHIFTING");
    this->gearControl->upshift();
      return true;
  } else {
      return false;   
  }
}

bool automatic_control::shouldDownshift(InputData inputData) {
  int band = this->getThrottleBand(inputData.ThrottlePercent);
  int captured = inputData.vssHz;
  if (captured < downshiftMap[band][gearControl->getCurrentGear()-1] && inputData.vssHz < 100 && gearControl->getCurrentGear() != 1) {
    Serial.println("DOWNSHIFTING");
    this->gearControl->downshift();
      return true;
  } else {
      return false;   
  }
}