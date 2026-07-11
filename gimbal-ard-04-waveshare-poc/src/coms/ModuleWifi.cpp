#include "ModuleWifi.h"

#include <ArduinoJson.h>

#include "driver/adc.h"

void ModuleWifi::handleStationDisconnected(WiFiEvent_t event) {
    Serial.println("station was disconnected");
    ModuleUi::handleCcApiConnect(false);
}

void ModuleWifi::scanNetworks(network_t defaultNetwork) {

    network_t connectNetwork = NETWORK_INVAL;

    int ssidCount = WiFi.scanNetworks();
    int ssidIndex = 0;
    for (; ssidIndex < ssidCount; ssidIndex++) {
        String ssid = WiFi.SSID(ssidIndex);
        if (ssid == String(defaultNetwork.ssid)) {
            Serial.print("found defaultNetwork: ");
            Serial.print(defaultNetwork.ssid);
            Serial.println(", assigning connectNetwork ...");
            connectNetwork = defaultNetwork;
        }
        int32_t rssi = WiFi.RSSI(ssidIndex);
        Serial.print("found scanNetwork: ");
        Serial.println(ssid);
        // ModuleWifi::discoveredNetworks[ssidIndex] = { rssi };
        // ssid.toCharArray(ModuleWifi::discoveredNetworks[ssidIndex].key, 64);
    }

    if (connectNetwork.ssid != NETWORK_INVAL.ssid) {
        Serial.print("connecting to connectNetwork: ");
        Serial.print(connectNetwork.ssid);
        Serial.println(" ...");

        ModuleWifi::connectToNetwork(connectNetwork);
    }

    WiFi.scanDelete();  // free memory from WiFi.scanNetworks() (we keep our own network information)

}

bool ModuleWifi::connectToNetwork(network_t network) {

    WiFi.mode(WIFI_STA);
    WiFi.begin(network.ssid, network.pass);
    for (int i = 0; i < 30; i++) {
        delay(100);
        if (ModuleWifi::isConnected()) {
            Serial.print(network.ssid);
            Serial.println(", connection established ...");
            return true;
        } else {
            Serial.print(network.ssid);
            Serial.println(", waiting for connection ...");
        }
    }

    // connection must have failed
    ModuleWifi::depower();  // be sure the correct mode is set
    return false;
}

bool ModuleWifi::powerup() {
    WiFi.setAutoReconnect(false);
    WiFi.onEvent(ModuleWifi::handleStationDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    return true;
}


bool ModuleWifi::isConnected() {
    wifi_mode_t wifiMode = WiFi.getMode();
    return wifiMode == WIFI_AP || WiFi.isConnected();  // WiFi.isConnected() includes both WIFI_MODE_STA and WIFI_MODE_APSTA
}

void ModuleWifi::depower() {
    WiFi.softAPdisconnect(true);
    WiFi.disconnect(true);
    for (int i = 0; i < 10; i++) { // wait for actual disconnect
        delay(200);
        if (!ModuleWifi::isConnected()) {
            Serial.println("wifi, connection terminated ...");
            break;
        }
    }
    WiFi.mode(WIFI_OFF);
}

String ModuleWifi::getAddress() {
    wifi_mode_t wifiMode = WiFi.getMode();
    if (wifiMode == WIFI_STA) {
        return WiFi.localIP().toString();
    } else if (wifiMode == WIFI_AP) {
        return WiFi.softAPIP().toString();
    } else if (wifiMode == WIFI_OFF) {
        return "wifi off";
    } else {
        return String(wifiMode);
    }
}
