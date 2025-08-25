#ifndef PRESSURECONTROL_H
#define PRESSURECONTROL_H

#include <Arduino.h>
#include "input_data.h"
#include "config.h"
#include "gear_control.h"

class PressureControl {
    private:
      int sl1Pressure;
      int sl2Pressure;
      int sltPressure;
      GearControl *gearControl;
      int calculateSL1Pressure(int throttlePercent);
      int calculateSL2Pressure(int throttlePercent);
      int calculateSLTPressure(int throttlePct,int pressurePctIdle,int pressurePctWOT);
    public:
      PressureControl(GearControl *gc);
      ~PressureControl();
      void setPressureSolenoids(InputData input);
      int getSL1PressureSetting();
      int getSL2PressureSetting();
      int getSLTPressureSetting();
};

#endif