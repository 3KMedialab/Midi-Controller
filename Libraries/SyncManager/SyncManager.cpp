#include "SyncManager.h"

uint32_t SyncManager::getSyncTimeStamp()
{
    return _syncTimeStamp;
}

uint16_t SyncManager::getBpm()
{
    return _bpm;
}

void SyncManager::setBpm(uint16_t bpm)
{
    _bpm = bpm;
}

void SyncManager::updateSyncTime()
{
    _syncTimeStamp = micros();
}