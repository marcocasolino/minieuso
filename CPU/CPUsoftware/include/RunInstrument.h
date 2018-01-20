#ifndef _RUN_INSTRUMENT_H
#define _RUN_INSTRUMENT_H

#include "CamManager.h"
#include "LvpsManager.h"
#include "DataAcqManager.h"
#include "InputParser.h"

/* number of seconds CPU waits for other systems to boot */
#define BOOT_TIME 4

/* location of data files */
#define HOME_DIR "/home/software/CPU"
#define DONE_DIR "/home/minieusouser/DONE"
#define DATA_DIR "/home/minieusouser/DATA"

/* class to handle different instrument operation modes */
class RunInstrument {
public:
  CmdLineInputs * CmdLine;
  enum InstrumentMode : uint8_t {
    NIGHT = 0,
    DAY = 1,
    UNDEF = 2,
  };
  InstrumentMode current_mode;

  Config * ConfigOut;
  ZynqManager ZqManager;
  UsbManager UManager;
  CamManager CManager;
  ThermManager ThManager;
  LvpsManager Lvps;
  DataAcqManager DaqManager;
  
  RunInstrument(CmdLineInputs * CmdLine);
  int Start();
  //int Stop();
  
private:
  int StartUp();
  int LvpsSwitch();
  int HvpsSwitch();
  int DebugMode();
  int CheckSystems();
  //int SwitchMode();
  int Acquisition();
};

#endif
/* _RUN_INSTRUMENT_H */
