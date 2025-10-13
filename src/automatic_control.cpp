#include "automatic_control.h"  

const int N = 50;
static int band = 0;
static int counter = 0;

int automatic_control::getThrottleBand(int tps) {
  if (band < this->NumBands-1 && tps > this->throttleBands[band]) {
    if (++counter >= N) { band++; counter = 0; }
  } else if (band > 0 && tps <= this->throttleBands[band-1]) {
    if (++counter >= N) { band--; counter = 0; }
  } else {
    counter = 0; // reset if not continuously past edge
  }
  if (band < 0) band = 0;
  if (band >= this->NumBands) band = this->NumBands - 1;
        Serial.println(band);

  return band;
}

bool automatic_control::applyTCC(int currentGear, int band, int hz) {
  bool tccLocked = this->gearControl->getLockupState();

  if (currentGear < 4) { 
    if (tccLocked) {
      this->gearControl->unlockTCC();
      return true; 
    }
    return false;
  }

  int col = currentGear - 4;
  int lockThr   = tccLockHz[band][col];
  int unlockThr = tccUnlockHz[band][col];

  if (tccLocked) {
    if (hz < unlockThr) {
      Serial.println("UNLOCKING");
      this->gearControl->unlockTCC();
      return true;
    }
  } else {
    if (hz > lockThr) {
      Serial.println("LOCKING");
      this->gearControl->lockupTCC();
      return true;
    }
  }
  return false;
}

uint32_t lastShiftMs = 0;
const uint32_t minHoldMs = 750;  // tune later

void automatic_control::applyAuto(InputData in) {
  uint32_t now = millis();
  if (now - lastShiftMs < minHoldMs) return;   // hold time

  int g = gearControl->getCurrentGear();
  int band = getThrottleBand(in.ThrottlePercent);
  int hz = in.vssHz;
  // bounds guard
  if (g < 1) g = 1; 
  if (g > 6) g = 6;

  bool lockupChange = this->applyTCC(g, band, hz);

  if(lockupChange) {
    lastShiftMs = now;
    return;
  }

  bool doUp = false, doDown = false;

  // Upshift check (only if g <= 5)
  if (g <= 5 && hz > upshiftMap[band][g-1]) doUp = true;

  // Downshift check (only if g >= 2)
  if (g >= 2) {
    int col = g - 2; // 2→1 col0, 3→2 col1, ... 6→5 col4
    if (hz < downshiftMap[band][col]) doDown = true;
  }

  // Arbitration: prefer DOWN (driver demand) or UP — pick one
  if (doDown) {
    Serial.println("DOWNSHIFTING");
    gearControl->downshift();
    lastShiftMs = now;
  } else if (doUp) {
    Serial.println("UPSHIFTING");
    gearControl->upshift();
    lastShiftMs = now;
  }
}
