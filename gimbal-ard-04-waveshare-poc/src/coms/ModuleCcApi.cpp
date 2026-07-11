#include "ModuleCcApi.h"


bool ModuleCcApi::powerup() {
    return true;
}
void ModuleCcApi::depower() {
    // nothing
}

bool ModuleCcApi::issueGet(String ccapi) {

    HTTPClient http;

    String endpoint = CCAPI______BASE_URL + ccapi;
    http.begin(endpoint);

    Serial.print("issue http get: ");
    Serial.print(endpoint);
    Serial.println(" ...");

    uint16_t httpResponseCode = http.GET();

    Serial.print("response code: ");
    Serial.println(httpResponseCode);

    http.end();

    return httpResponseCode == 200;

}
bool ModuleCcApi::issuePost(String ccapi, String body) {

    HTTPClient http;

    String endpoint = CCAPI______BASE_URL + ccapi;
    http.begin(endpoint);
    http.addHeader("Content-Type", "application/json");

    Serial.print("issue http post: ");
    Serial.print(endpoint);
    Serial.print(", body: ");
    Serial.print(body);
    Serial.println(" ...");

    uint16_t httpResponseCode = http.POST(body);

    Serial.print("response code: ");
    Serial.println(httpResponseCode);

    http.end();

    return httpResponseCode == 200;

}