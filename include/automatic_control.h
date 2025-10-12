
#ifndef ARDUINO_AUTOMATIC_CONTROL_H
#define ARDUINO_AUTOMATIC_CONTROL_H
#include <Arduino.h>
#include "gear_control.h"
#include "input_data.h"

class automatic_control  
{
	private:
		GearControl *gearControl;
		// throttle bands: very-light, light, light-mid, mid, heavy, WOT
		static constexpr int NumBands = 6;
		const int throttleBands[NumBands-1] = {21, 32, 45, 60, 85};

		// Upshift Map (6 bands X 5 shift points)
		int upshiftMap[NumBands][5] = {
			{150, 260, 427,  577,   691},   // 0-21%
			{210, 360, 520,  720,   880},   // 22-32%
			{240, 440, 680,  940,   960},   // 33-45%
			{260, 500, 780, 1080,  1040},   // 46-60%
			{280, 560, 854, 1200,  1120},   // 61-85%
			{300, 730, 1080, 1500, 1300}    // 86-100%
		};

		// Downshift Map (6 bands X 5 shift points)
		int downshiftMap[NumBands][5] = {
			{105, 140, 300,  430,   520},   // 0-21%
			{150, 240, 380,  620,   760},   // 22-32%
			{180, 340, 560,  820,   860},   // 33-45%
			{200, 420, 640,  940,   900},   // 46-60%
			{220, 500, 740, 1000,   980},   // 61-85%
			{240, 560, 880, 1300,  1100}    // 86-100%
		};

		// TCC Lockup Map (6 bands X 3 shift points) - Only operates in 4th 5th 6th gears
		int tccLockHz[NumBands][3] = {
			{    480,   608,   720 },   // 0-21%
			{    608,   832,  1040 },   // 22-32%
			{    704,   928,  1120 },   // 33-45%
			{    800,  1040,  1200 },   // 46-60%
			{    920,  1200,  1360 },   // 61-85%
			{   1040,  1280,  1440 }    // 86-100%
		};

		// tccUnlockHz[band][g-3]  // g = 3..6
		int tccUnlockHz[NumBands][3] = {
			{    320,   440,   540 },   // 0-21%
			{    440,   600,   760 },   // 22-32%
			{    560,   720,   880 },   // 33-45%
			{    640,   820,   960 },   // 46-60%
			{    760,   940,  1100 },   // 61-85%
			{    880,  1060,  1240 }    // 86-100% 
		};

		int getThrottleBand(int throttlePercent);
		bool applyTCC(int currentGear, int band, int hz);
	public:
		automatic_control(GearControl *gc){this->gearControl = gc;};
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
