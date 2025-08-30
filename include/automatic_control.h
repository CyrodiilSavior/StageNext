
#ifndef ARDUINO_AUTOMATIC_CONTROL_H
#define ARDUINO_AUTOMATIC_CONTROL_H
#include <Arduino.h>
#include "gear_control.h"
#include "input_data.h"

class automatic_control  
{
	private:
		GearControl *gearControl;
		const int throttleBands[6] = {21, 28, 38, 76, 85, 100};
		// const int throttleBands[6] = {21, 21, 21, 21, 21, 21};
		// upshiftMapTPvsGEARvsHz
		// rows = throttle bands: 0-21, 21-28, 28-38, 38-76, 76-85, 85-100%
		// cols = 1→2, 2→3, 3→4, 4→5, 5→6
// upshiftMapTPvsGEARvsHz
// rows = throttle bands: 0-21, 21-28, 28-38, 38-76, 76-85, 85-100%
// cols = 1→2, 2→3, 3→4, 4→5, 5→6
		int upshiftMap[6][5] = {
			{150, 231, 427,  577,  691},   // 0-21%
			{210, 300, 480,  650,  775},   // 21-28%
			{240, 400, 620,  840,  900},   // 28-38%
			{260, 532, 854, 1150, 1000},   // 38-76%
			{290, 640, 980, 1350, 1050},   // 76-85%
			{300, 730,1080, 1500, 1100}    // 85-100%
		};


		// downshiftMapTPvsGEARvsHz
		// rows = throttle bands: 0-21, 21-28, 28-38, 38-76, 76-85, 85-100%
		// cols = 2→1, 3→2, 4→3, 5→4, 6→5
		int downshiftMap[6][5] = {
			{ 50, 131, 327,  477,  591},   // 0-21%   (gaps now ≥100)
			{110, 200, 380,  550,  675},   // 21-28%
			{140, 300, 500,  700,  800},   // 28-38%
			{160, 432, 730,  950,  880},   // 38-76%
			{190, 540, 800, 1200,  950},   // 76-85%
			{200, 600, 900, 1400, 1000}    // 85-100%
		};

		int getThrottleBand(int throttlePercent);
	public:
		automatic_control(GearControl *gc){this->gearControl = gc;};
		bool shouldUpshift(InputData inputData);
		bool shouldDownshift(InputData inputData);
		void applyAuto(InputData in);
};
#endif


   
// const int throttleBands[6] = {21, 28, 38, 76, 85, 100};

// int main()
// {
//     int gear = 1;
//     int throttle = 20;
//     for (int speed = 0; speed <= 100; speed++) {
//         bool upshifted = autoLogic(gear, throttle, speed);
//         if (upshifted) {
//             std::cout << "SHIFTED FROM " << gear << " AT " << speed << "mph\n";
//             if (gear < 5) {
//                 gear++;
//             } else {
//                 break;   
//             }
//         }
//     }
// }