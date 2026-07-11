#ifndef ModuleWifi_h
#define ModuleWifi_h

#include <Arduino.h>
#include <WiFi.h>

#include "../Define.h"
#include "../ui/ModuleUi.h"

class ModuleWifi {

private:
    static bool connectToNetwork(network_t network);
    static void handleStationDisconnected(WiFiEvent_t event);

public:
    static bool powerup();
    static bool isConnected();
    static void depower();
    static void scanNetworks(network_t defaultNetwork);
    static String getAddress();
};

#endif