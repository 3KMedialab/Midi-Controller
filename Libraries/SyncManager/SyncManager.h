#ifndef SyncManager_h
#define SyncManager_h

#include <Arduino.h>

#define MICROSECONDS_PER_MINUTE 60000000

class SyncManager
{
  public:   

  SyncManager(uint8_t barLength);

  uint32_t getSyncTimeStamp();
  uint8_t isActive();
  uint8_t getTimeInBar();

  void setBpm(uint16_t bpm);

  void updateSyncTime();
  void updateSyncStatus();
  void activate();
  void deactivate();
  uint8_t canStart();

  private:

    uint32_t _syncTimeStamp;   
    uint8_t _actived;
    uint8_t _timeInBar;
    uint16_t _bpm;
    uint8_t _barLength;
    uint8_t _changedTimeInBar;
    uint32_t _lastSyncTimeStamp;
    uint32_t _delayBetweenTimes;
};
#endif