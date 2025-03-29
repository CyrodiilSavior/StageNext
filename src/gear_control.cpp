#include "gear_control.h"
#include "defines.h"
#include "PinChangeInterrupt.h"

// Volatile flags set by ISRs
volatile bool upshiftRequested   = false;
volatile bool downshiftRequested = false;

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

void onUpshiftISR() {
  upshiftRequested = true;
}

void onDownshiftISR() {
  downshiftRequested = true;
}

void GearControl::begin() {
    // Configure pins as inputs with pullups
    pinMode(BUTTON_UPSHIFT,   INPUT_PULLUP);
    pinMode(BUTTON_DOWNSHIFT, INPUT_PULLUP);

    attachPCINT(digitalPinToPCINT(BUTTON_UPSHIFT),   onUpshiftISR,   RISING);
    attachPCINT(digitalPinToPCINT(BUTTON_DOWNSHIFT), onDownshiftISR, RISING);
}

void GearControl::processShiftRequests() {
  // Safely check and clear the volatile flags
  noInterrupts();
  bool ups = upshiftRequested;
  upshiftRequested = false;
  bool dns = downshiftRequested;
  downshiftRequested = false;
  interrupts();

  // Now handle the requests outside the critical section
  if (ups) {
    this->upshift();
  }
  if (dns) {
    this->downshift();
  }
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