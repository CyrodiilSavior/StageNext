#ifndef GEARCONTROL_H
#define GEARCONTROL_H

#include <Arduino.h>
#include "input_data.h"

#define MOMENTARY_COMMAND_UPSHIFT "Upshift"
#define MOMENTARY_COMMAND_DOWNSHIFT "Downshift"

class GearControl {
    private:
        int currentGear;
        
        unsigned long lastTimeCheck = 0;
        String lastMomentaryCommand = "";

        void setCurrentGear(int input);
        void outputSignalToSolenoids(int s1, int s2, int s3, int s4, int sR);
    public:
        GearControl();
        ~GearControl();
        void processInputData(InputData data);
        bool upshift();
        bool downshift();
        int getCurrentGear();
};

#endif