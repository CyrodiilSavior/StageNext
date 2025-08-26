
#ifndef ARDUINO_AUTOMATIC_CONTROL_H
#define ARDUINO_AUTOMATIC_CONTROL_H
#include <Arduino.h>
#include "gear_control.h"

class automatic_control  
{
	private:
    // upshiftMapTPvsGEARvsMPH
    int upshiftMap[6][5] = {
    //   1→2  2→3  3→4  4→5  5→6
      {  7,  13,  20,  29,  55},   // 0-21%
      {  7,  14,  24,  35,  58},   // 21-28%
      { 10,  19,  32,  47,  62},   // 28-38%
      { 29,  50,  76, 108,  70},   // 38-76%
      { 34,  55,  84, 119,  75},   // 76-85%
      { 36,  57,  84, 119,  80}    // 85-100%
    };
		// downshiftMapTPvsGEARvsMPH
		//   2→1  3→2  4→3  5→4  6→5
		int downshiftMap[6][5] = {
			{  8, 14, 22, 32, 45},   // 0-21%
			{  9, 15, 24, 35, 48},   // 21-28%
			{ 11, 18, 28, 42, 50},   // 28-38%
			{ 13, 24, 36, 55, 55},   // 38-76%
			{ 15, 28, 44, 65, 60},   // 76-85%
			{ 18, 35, 55, 78, 65}    // 85-100%
		};

	public:
		automatic_control();

};
#endif


   
// const int throttleBands[6] = {21, 28, 38, 76, 85, 100};

// int getThrottleBand(int throttlePercent) {
//     for (int i = 0; i < 6; i++) {
//         if (throttlePercent <= throttleBands[i]) {
//             return i;  // matched this band
//         }
//     }
//     return 5; // default to last band (85-100%) if out of range
// }

// bool autoLogic(int gear, int throttle, int speed) {
//     int band = getThrottleBand(throttle);

//     if (speed > upshiftMap[band][gear-1]) {
//         return true;
//     } else {
//         return false;   
//     }
// }

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