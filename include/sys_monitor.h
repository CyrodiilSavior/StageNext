#ifndef SYSMONITOR_H
#define SYSMONITOR_H

#include <Arduino.h>
#include "gear_control.h"
#include "pressure_control.h"
#include "input_data.h"

struct SystemState {
    InputData inputData;
    int CurrentGear;
    bool lockupMode;
    int sl1Pressure;
    int sl2Pressure;
    int sltPressure;
};

class SystemMonitor {
    private:
        GearControl *gearControl;
        PressureControl *pressureControl;
        String systemStateToJson(const SystemState& state);
    public:
        SystemMonitor(GearControl* gearControl, PressureControl* pressureControl);
        String captureState(InputData data);
};

#endif