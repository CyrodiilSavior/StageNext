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
    data.OilTemp = analogRead(TEMP_SENSOR);

    int raw = analogRead(THROTTLE_INPUT);
    data.RawThrottle = raw;
    data.ThrottlePercent = map(raw, 110, 750, 0, 100); // clamp or constrain if needed

    return data;
}

