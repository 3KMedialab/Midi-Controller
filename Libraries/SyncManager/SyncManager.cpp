#include "SyncManager.h"

/*
* Constructor
*/
SyncManager::SyncManager(uint8_t barLength) 
{
    _actived = 0;
    _timeInBar = 1;
    _changedTimeInBar = 1;
    _lastSyncTimeStamp = 0;

    _barLength = barLength;
}

uint32_t SyncManager::getSyncTimeStamp()
{
    return _syncTimeStamp;
}

uint8_t SyncManager::isActive()
{
    return _actived;
}

uint8_t SyncManager::getTimeInBar()
{
    return _timeInBar;
}

void SyncManager::setBpm(uint8_t bpm)
{
    _bpm = bpm;
}

void SyncManager::activate()
{
    _actived = 1;
}

void SyncManager::deactivate()
{
    _actived = 0;
    _timeInBar = 1;
    _changedTimeInBar = 1;
    _lastSyncTimeStamp = 0;
}

void SyncManager::updateSyncStatus()
{
    _syncTimeStamp = micros();

    // if sync manager is enabled, calculate time in bar
    if (_actived)
    {
        // calculate the delay time between times in the bar       
        _delayBetweenTimes = MICROSECONDS_PER_MINUTE / _bpm;
        
        if (_syncTimeStamp - _lastSyncTimeStamp >= _delayBetweenTimes)
        {
            // Move to next time of the bar
            _timeInBar++;

            if (_timeInBar >= _barLength)
            {
                _timeInBar = 1;
            }

            // new time =  true
            _changedTimeInBar = 1;

            _lastSyncTimeStamp = _syncTimeStamp;        
        }

        else
        {
            // new time = false
            _changedTimeInBar = 0;
        }
    }    
}

uint8_t SyncManager::canStart()
{
    // if synchronisation is not actived, so sending data can start. Otherwise, sending data must wait for first time in bar 
    if (!_actived)
    {
        return 1;
    }
    
    if (_timeInBar == 1 && _changedTimeInBar == 1)
    {
        return 1;
    }
    
    return 0;       
}










