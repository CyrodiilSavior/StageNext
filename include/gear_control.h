#ifndef GEARCONTROL_H
#define GEARCONTROL_H

#include <Arduino.h>
#include "input_data.h"
#include "config.h"

#define MOMENTARY_COMMAND_UPSHIFT "Upshift"
#define MOMENTARY_COMMAND_DOWNSHIFT "Downshift"

class GearControl {
    private:
        int currentGear;
        
        // For timing
        long lastUpshift = 0;
        long lastDownshift = 0;

        // For momentary switches
        unsigned long lastTimeCheck = 0;
        String lastMomentaryCommand = "";

        void setCurrentGear(int input);
        void outputSignalToSolenoids(int s1, int s2, int s3, int s4, int sR);

        bool lockupState;

    public:
        GearControl();
        void begin();
        void processShiftRequests();
        void processAutomaticUpdates(int throttlePercent);
        bool upshift();
        bool downshift();
        int getCurrentGear();
        long timeSinceLastShift();
        long timeSinceLastDownshift();
        long timeSinceLastUpshift();
        // For lockup shifts
        void setLockup(InputData input);
        void lockupTCC();
        void unlockTCC();
        bool getLockupState();
};

#endif