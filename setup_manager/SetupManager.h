#ifndef SETUP_MANAGER_H
#define SETUP_MANAGER_H

#include <WiFi.h>
#include <WebServer.h>
#include "../storage/StorageManager.h"
#include "../sleep_timer/SleepTimer.h"
#include "../screen/ScreenManager.h"
#include "../index_html.h"

class SetupManager {
public:
    SetupManager(StorageManager& storage, SleepTimer& sleepTimer, ScreenManager& screenManager);

    void begin();
    void loop();

private:
    StorageManager& _storage;
    SleepTimer& _sleepTimer;
    ScreenManager& _screenManager;
    WebServer _server;
    bool _completed = false;
    bool _clientConnected = false;
    unsigned long _lastInteraction = 0;
};

#endif // SETUP_MANAGER_H
