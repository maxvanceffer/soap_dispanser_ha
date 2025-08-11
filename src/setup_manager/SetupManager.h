#ifndef SETUP_MANAGER_H
#define SETUP_MANAGER_H

#include <WiFi.h>
#include <WebServer.h>
#include "../../IService.h"

class SetupManager: public IService {
public:
    SetupManager();

    String name() const override;
    Type type() const override;
    ServiceValue getValue(String key) const override;
    bool execute(String fnName, JsonVariant args) override;

    void begin();
    void loop();

private:
    WebServer _server;
    bool _completed = false;
    bool _clientConnected = false;
    unsigned long _lastInteraction = 0;
};

#endif // SETUP_MANAGER_H
