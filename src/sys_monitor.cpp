#include "sys_monitor.h"
#include "ArduinoJson.h"
#include "defines.h"

SystemMonitor::SystemMonitor(GearControl* gearControl, PressureControl* pressureControl) {
  this->gearControl = gearControl;
  this->pressureControl = pressureControl;
  Serial.println(gearControl->getCurrentGear());
}

String SystemMonitor::captureState(InputData data) {
  SystemState state;
  state.inputData = data;
  state.CurrentGear = this->gearControl->getCurrentGear();
  state.lockupMode = this->pressureControl->getLockupState();
  state.pressureSetting = this->pressureControl->getPressureSetting();

  return this->systemStateToJson(state);
}

String SystemMonitor::systemStateToJson(const SystemState& state) {
    StaticJsonDocument<200> doc;

    doc["inputData"]["UpshiftRequested"] = state.inputData.UpshiftRequested;
    doc["inputData"]["DownshiftRequested"] = state.inputData.DownshiftRequested;
    doc["inputData"]["LockupMode"] = state.inputData.LockupMode;
    doc["inputData"]["PressureControl"] = state.inputData.PressureControl;
    doc["inputData"]["OilTemp"] = state.inputData.OilTemp;

    doc["pinState"]["SOL_S1"] = digitalRead(SOL_S1);
    doc["pinState"]["SOL_S2"] = digitalRead(SOL_S2);
    doc["pinState"]["SOL_S3"] = digitalRead(SOL_S3);
    doc["pinState"]["SOL_S4"] = digitalRead(SOL_S4);
    doc["pinState"]["SOL_SR"] = digitalRead(SOL_SR);

    doc["CurrentGear"] = state.CurrentGear;
    doc["PressureSetting"] = state.pressureSetting;
    doc["LockupState"] = state.lockupMode;

    String output;
    serializeJson(doc, output);
    return output;
}