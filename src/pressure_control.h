#ifndef PRESSURECONTROL_H
#define PRESSURECONTROL_H

#include <Arduino.h>
#include "input_data.h"
#include "defines.h"

class PressureControl {
    private:
      bool lockupState;
      int pressureControl;
    public:
      PressureControl();
      ~PressureControl();
      void setPressureSolenoids(InputData input);
      void setLockup(InputData input);
      bool getLockupState();
      int getPressureSetting();
};

#endif