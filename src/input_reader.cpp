#include "input_reader.h"
#include "config.h"

InputReader::InputReader() {
  return;
}

InputData InputReader::read(float vssReading) {
    InputData data;    
    data.UpshiftRequested = digitalRead(BUTTON_UPSHIFT) == HIGH;
    data.DownshiftRequested = digitalRead(BUTTON_DOWNSHIFT) == HIGH;
    data.LockupMode = false;
    data.OilTemp = analogRead(TEMP_SENSOR);
    data.vssHz = vssReading;

    int raw = analogRead(THROTTLE_INPUT);
    data.RawThrottle = raw;
    data.ThrottlePercent = map(raw, 110, 750, 0, 100); // clamp or constrain if needed
    if (data.ThrottlePercent < 0) {
      data.ThrottlePercent = 0;
    }
    return data;
}
