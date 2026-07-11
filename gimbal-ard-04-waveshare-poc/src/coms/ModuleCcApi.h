#ifndef ModuleCcApi_h
#define ModuleCcApi_h

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "../Define.h"

class ModuleCcApi {
private:


public:
    static bool powerup();
    static void depower();
    static bool issueGet(String ccapi);
    static bool issuePost(String ccapi, String body);
};

#endif