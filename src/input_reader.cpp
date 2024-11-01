#include "input_reader.h"
#include "defines.h"

InputReader::InputReader() {
  return;
}

InputReader::~InputReader() {
  return;
}

InputData InputReader::read() {
    InputData data;    
    data.UpshiftRequested = digitalRead(BUTTON_UPSHIFT) == HIGH;
    data.DownshiftRequested = digitalRead(BUTTON_DOWNSHIFT) == HIGH;
    data.LockupMode = digitalRead(BUTTON_LOCKUP) == HIGH;
    data.PressureControl = analogRead(POT_GLOBAL);
    data.OilTemp = analogRead(TEMP_SENSOR);
    return data;
}