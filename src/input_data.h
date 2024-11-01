#ifndef INPUTDATA_H
#define INPUTDATA_H

struct InputData {
  bool UpshiftRequested;
  bool DownshiftRequested;
  bool LockupMode;
  int  PressureControl;
  int  OilTemp;
};

#endif