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
    analogWrite(SOL_PWM_SLU, 255);
  } else {
    analogWrite(SOL_PWM_SLU, 0);
  }
}

bool PressureControl::getLockupState() {
  return this->lockupState;
}

// For iteration 1 of this design, if 5th and 6th gear, go to 0% duty cycle (ON)
// 1st - 4th gear go to 80% duty cycle (OFF)
int PressureControl::calculateSL1Pressure(int throttlePercent) {
  if (this->gearControl->getCurrentGear() > 4) {
    return 255;
  }
  return 0;
}

// Reverse that for this solenoid
int PressureControl::calculateSL2Pressure(int throttlePercent) {
  if (this->gearControl->getCurrentGear() <= 4) {
    return 255;
  }
  return 0;
}

/****************************************************************************************
 *  calculateSLTByte
 *
 *  Aisin A760 →  SLT solenoid is **normally open**.
 *  ──────────────────────────────────────────────────────────────────────────────────────
 *  • More current (higher PWM duty) holds the valve open, BLEEDING line pressure away.
 *  • Less current (lower duty) lets the valve close, RAISING line pressure.
 *
 *  Therefore:        **pressure  ↑  as  duty  ↓**
 *
 *  API
 *  ──────────────────────────────────────────────────────────────────────────────────────
 *  uint8_t calculateSLTPressure(
 *            int throttlePct,           // 0‒100   (% throttle opening)
 *            int pressurePctIdle,       // % of max pressure at 0% throttle
 *            int pressurePctWOT);       // % of max pressure at 100% throttle
 *
 *      • The two "pressurePct" values are expressed in *human* terms:
 *          0   =   valve fully open  (minimum line pressure)
 *          100 =   valve fully shut  (maximum line pressure)
 *      • The function converts those percentages into an 8‑bit duty cycle
 *        where 0=100% pressure  and 255=0% pressure.
 *      • It returns the ready‑to‑use analogWrite byte (0‒255).
 *
 *  SPECIAL BEHAVIOUR
 *  ──────────────────────────────────────────────────────────────────────────────────────
 *  • Once the transmission is in 5th or 6th gear *and* 300ms have elapsed
 *    since the last up‑shift, both pressure targets are clamped to 90%
 *    (=> a fixed baseline duty) to prevent 6th‑gear slip on flat throttle.
 *
 *  EXAMPLE CALL‑SITE
 *  ──────────────────────────────────────────────────────────────────────────────────────
 *      // want 20% pressure at idle throttle, 80% at WOT:
 *      uint8_t sltByte = calculateSLTByte(input.ThrottlePercent, 20, 80);
 *      analogWrite(SOL_PWM_SLT, sltByte);
 *
 ****************************************************************************************/
int PressureControl::calculateSLTPressure(int throttlePct,int pressurePctIdle,int pressurePctWOT) {
    throttlePct = 90;
    // Steady‑state 5th / 6th → lock both ends at 80% pressure (≈ 20% duty)
    if (gearControl->getCurrentGear() >= 5 &&
        gearControl->timeSinceLastUpshift() > 300) {
        pressurePctIdle = pressurePctWOT = 80;
    }

    throttlePct      = constrain(throttlePct,      0, 100);
    pressurePctIdle  = constrain(pressurePctIdle,  0, 100);
    pressurePctWOT   = constrain(pressurePctWOT,   0, 100);

    // Convert "% pressure" → "% duty"
    //   pressure 0%  → duty 100%
    //   pressure 100%→ duty   0%
    uint8_t dutyIdle = static_cast<uint8_t>((255.0f * (100 - pressurePctIdle)) / 100.0f);
    uint8_t dutyWOT  = static_cast<uint8_t>((255.0f * (100 - pressurePctWOT )) / 100.0f);

    // Map throttle 0→100%  to dutyIdle→dutyWOT  (linear ramp)
    int duty = map(throttlePct, 0, 100, dutyIdle, dutyWOT);

    return duty;   // 0‑255 → feed straight into analogWrite()
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