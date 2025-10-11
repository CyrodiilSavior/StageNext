#include "automatic_control.h"  

const int bandEdges[5] = {21,28,38,76,85};
const int N = 50;
static int band = 0;
static int counter = 0;

int automatic_control::getThrottleBand(int tps) {
  if (band < 5 && tps > bandEdges[band]) {
    if (++counter >= N) { band++; counter = 0; }
  } else if (band > 0 && tps <= bandEdges[band-1]) {
    if (++counter >= N) { band--; counter = 0; }
  } else {
    counter = 0; // reset if not continuously past edge
  }
  return band;
}

bool automatic_control::applyTCC(int currentGear, int band, int hz) {
  bool tccLocked = this->gearControl->getLockupState();

  if (currentGear < 3) { if (tccLocked) tccLocked = false; return; }

  int col = currentGear - 3;
  int lockThr   = tccLockHz[band][col];
  int unlockThr = tccUnlockHz[band][col];

  if (tccLocked) {
    if (hz < unlockThr /* or big tip-in event */) {
      // PRIORITIZE: unlock before allowing a downshift
      Serial.println("UNLOCKING");
      this->gearControl->unlockTCC();
      return true;
      // start a small TCC hold timer if you want (e.g., 300 ms)
    }
  } else {
    if (hz > lockThr /* and no inhibits */) {
      Serial.println("LOCKING");
      this->gearControl->lockupTCC();
      return true;
    }
  }
  return false;
}



// int automatic_control::getThrottleBand(int throttlePercent) {
//   for (int i = 0; i < 6; i++) {
//       if (throttlePercent <= throttleBands[i]) {
//           return i;  // matched this band
//       }
//   }
//   return 5; // default to last band (85-100%) if out of range
// }

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

uint32_t lastShiftMs = 0;
const uint32_t minHoldMs = 500;  // tune later

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
