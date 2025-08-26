#ifndef INPUTDATA_H
#define INPUTDATA_H

struct InputData {
  bool UpshiftRequested;
  bool DownshiftRequested;
  float vssHz;
  bool LockupMode;
  int  ThrottlePercent;
  long RawThrottle;
  int  OilTemp;
};

#endif