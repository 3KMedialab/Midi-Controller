#ifndef SyncManager_h
#define SyncManager_h

#include <Arduino.h>

class SyncManager
{
  public:    

  uint32_t getSyncTimeStamp(); 
  uint16_t getBpm ();

  void setBpm(uint16_t bpm);

  void updateSyncTime();  

  private:

    uint32_t _syncTimeStamp;
    uint16_t _bpm;

};
#endif