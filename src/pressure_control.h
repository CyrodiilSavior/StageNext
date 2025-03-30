#ifndef PRESSURECONTROL_H
#define PRESSURECONTROL_H

#include <Arduino.h>
#include "input_data.h"
#include "defines.h"
#include "gear_control.h"

class PressureControl {
    private:
      bool lockupState;
      int sl1Pressure;
      int sl2Pressure;
      int sltPressure;
      GearControl *gearControl;
      int calculateSL1Pressure(int throttlePercent);
      int calculateSL2Pressure(int throttlePercent);
      int calculateSLTPressure(int throttlePercent, int min, int max);
    public:
      PressureControl(GearControl *gc);
      ~PressureControl();
      void setPressureSolenoids(InputData input);
      void setLockup(InputData input);
      bool getLockupState();
      int getSL1PressureSetting();
      int getSL2PressureSetting();
      int getSLTPressureSetting();
};

#endif