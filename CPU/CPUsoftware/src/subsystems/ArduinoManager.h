#ifndef _ARDUINO_MANAGER_H
#define _ARDUINO_MANAGER_H

// 0 REAL HW
// 1 simulator
// 2 use with Arduino connected (ie. automatically in night mode)
#define ARDUINO_DEBUG 2

//#define PRINT_DEBUG_INFO 1
// COMMENT no debug
// ANY NUMBER print all

#include <mutex>
#include <memory>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <condition_variable>
#include <termios.h>
#include <fcntl.h> 
#include <stdlib.h>

#if ARDUINO_DEBUG ==0
  #include <unistd.h>
  #include <termios.h>
  #include "log.h"
#endif

#include "minieuso_data_format.h"
#include "ConfigManager.h"

// coming from the .h of arduino 

#define X_HEADER_SIZE 4 // AA55AA55
#define X_SIPM_BUF_SIZE 128 // 64 channels, two byte
#define X_OTHER_SENSORS 8 // 4 channels, two byte
#define X_TOTAL_BUF_SIZE (X_SIPM_BUF_SIZE+X_OTHER_SENSORS)
#define X_TOTAL_BUF_SIZE_HEADER (X_HEADER_SIZE+X_SIPM_BUF_SIZE+X_OTHER_SENSORS+4) // packet number at begin and crc at end
#define X_DELAY 100 // ms
#define READ_ARDUINO_TIMEOUT  100 // it should be in ms now is in attempts to read the buffer

#define AVERAGE_DEPTH 1 // number of acquisitions to be averaged

/* for use with arduino readout functions */
#define DUINO "/dev/ttyUSB0"
#define BAUDRATE B9600
#define BUF_SIZE 14
#define FIFO_DEPTH 1
#define CHANNELS (X_OTHER_SENSORS+X_SIPM_BUF_SIZE)

/* for use with conditional variable */
#define WAIT_PERIOD 1 /* milliseconds */

/**
 * acquisition structure to store analog readout 
 */
typedef struct {
  float val [FIFO_DEPTH][CHANNELS];
} AnalogAcq;

/**
 * struct to store light levels for polling 
 */
typedef struct {
  float photodiode_data[N_CHANNELS_PHOTODIODE];  
  float sipm_data[N_CHANNELS_SIPM]; 
  float sipm_single; 
} LightLevel;

/**
 * class to handle the analog data acquisition (photodiodes and SiPMs) 
 * uses a simple Arduino to replace the AnalogManager module due to 
 * issues with power comsumption.
 */
class ArduinoManager {
public:
  /**
   * enum to specify the current light level status of the instrument
   */
  enum LightLevelStatus : uint8_t {
	  LIGHT_BELOW_NIGHT_THR = 0,
	  LIGHT_ABOVE_DAY_THR = 1,
	  LIGHT_UNDEF = 2,
  };

  LightLevelStatus current_lightlevel_status;
  std::shared_ptr<Config> ConfigOut;

  ArduinoManager();
  std::shared_ptr<LightLevel> ReadLightLevel();
  LightLevelStatus CompareLightLevel(std::shared_ptr<Config> ConfigOut);
  int ProcessAnalogData(std::shared_ptr<Config> ConfigOut);  
  int GetLightLevel();
  int AnalogDataCollect();

  /* handle instrument mode switching */
  int Notify();
  int Reset();
  
private:

  /*
   * for thread-safe access to the light_level 
   */
  std::mutex m_light_level;
  /*
   * light level stored here and accessed only with mutex protection
   */
  std::shared_ptr<LightLevel> light_level;
  /*
   * analog acquisition stored here
   */
  std::shared_ptr<AnalogAcq> analog_acq;

  /*
   * to notify the object of a mode switch
   */
  bool inst_mode_switch;
  /*
   * to handle mode switching in a thread-safe way
   */
  std::mutex m_mode_switch;
  /*
   * to wait for a mode switch
   */
  std::condition_variable cv_mode_switch;

  
  int SetInterfaceAttribs(int fd, int speed);
  int SerialReadOut(int fd);
  
};

#endif
/* _ARDUINO_MANAGER_H */
