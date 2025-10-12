
#ifndef ARDUINO_AUTOMATIC_CONTROL_H
#define ARDUINO_AUTOMATIC_CONTROL_H
#include <Arduino.h>
#include "gear_control.h"
#include "input_data.h"

class automatic_control  
{
	private:
		GearControl *gearControl;
		// throttle bands: very-light, light, medium, WOT
		const int throttleBands[4] = {21, 38, 85, 100};

		// Upshift Map (4 bands X 5 shift points)
		int upshiftMap[4][5] = {
			{150, 260, 427,  577,  691},   // 0-21%
			{210, 360, 520,  720,  880},   // 22-38%
			{260, 532, 854, 1150, 1000},   // 38-85%
			{300, 730, 1080, 1500, 1100}    // 85-100%
		};

		// Downsift Map (3 bands X 5 shift points)
		int downshiftMap[4][5] = {
			{105, 140, 300,  430,  520},   // 0-21%
			{150, 240, 380,  620,  760},   // 22-38%
			{150, 400, 720, 1020,  920},   // 38-85%
			{180, 550, 900, 1350, 1050}    // 85-100%
		};

		// TCC Lockup Map (4 bands X 3 shift points) - Only operates in 4th 5th 6th gears
		int tccLockHz[4][3] = {
			{    480,   608,   720 },   // 0-21%
			{    608,   832,  1040 },   // 22-38%
			{    880,  1120,  1280 },   // 38-85%
			{   1040,  1280,  1440 }    // 85-100%
		};

		// tccUnlockHz[band][g-3]  // g = 3..6
		int tccUnlockHz[4][3] = {
			{    320,   440,   540 },   // 0-21%
			{    440,   600,   760 },   // 22-38%
			{    640,   800,   960 },   // 38-85%
			{    800,  1000,  1180 }    // 85-100% 
		};

		int getThrottleBand(int throttlePercent);
		bool applyTCC(int currentGear, int band, int hz);
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
