#include <Wire.h>
#include <lvgl.h>

#include "Define.h"

#include "disp/TouchDisplay.h"
#include "sens/SensorOrientation.h"
#include "coms/ModuleWifi.h"
// #include "UartSrv.h"
// #include "coms/I2cSrvPri.h"
#include "coms/Nowsrv.h"

uint64_t totalLoopPriCount = 0;
vector________t data = { 0, 0, 0 };
bool wire1HasBegun = false;

/**
 * reads orientation sensor whenever a new message can be sent through espnow
 */
void runLoopTaskPri(void* pvParameters) {

  uint64_t millisM = 1;
  uint64_t millisA;
  uint64_t millisB;
  while (true) {

    millisA = millis();

    if (wire1HasBegun) { // i2c (orientation ok && no previous message pending)
      SensorOrientation::read();
      vector________t orientation = SensorOrientation::getOrientation();
      // UartSrv::sendData(orientation); // send data as fast as possible
      // I2cSrvPri::sendData(orientation);
      NowSrv::sendData(orientation);
    }

    millisB = millis();

    vTaskDelay(max(millisM, 11 - (millisB - millisA)));
    totalLoopPriCount++;

  }

}

/**
 * reads orientation sensor every 1000ms to write it to serial
 */
void runLoopTaskSec(void* pvParameters) {

  while (true) {

    if (wire1HasBegun) {
      vector________t sendData = SensorOrientation::getOrientation();
      Serial.printf("{\"x\":%s,\"y\":%s,\"z\":%s} - %s - %s\n", String(sendData.x, 2), String(sendData.y, 2), String(sendData.z, 2), String(NowSrv::totalSendCount), String(NowSrv::totalSendCount * 1000 / (millis() - NowSrv::firstSendMillis)));
    }

    // Serial.print("totalLoopPriCount: ");
    // Serial.println(String(totalLoopPriCount));
    vTaskDelay(1000);

  }

}

void setup() {

  Serial.begin(115200);
  delay(2000);
  Serial.println("- serial ready");

  TouchDisplay::touchDisplayBegin();
  delay(100);
  Serial.println("- touch display ready");

  wire1HasBegun = Wire1.begin(GPIO_NUM___SDA1, GPIO_NUM___SCL1, 0);
  // wire1HasBegun = false;
  if (wire1HasBegun) {
    SensorOrientation::powerup();
    delay(100);
    Serial.println("- orientation ready");
  } else {
    Serial.println("! orientation not ready");
  }

  // UartSrv::powerup();
  // delay(100);
  // Serial.println("- uart ready");

  if (wire1HasBegun) {
    // I2cSrvPri::powerup();
    NowSrv::powerup();
    delay(100);
    Serial.println("- i2c ready");
  } else {
    Serial.println("! i2c not ready");
  }

  ModuleWifi::powerup();
  delay(100);
  Serial.println("- wifi ready");

  // pinMode(GPIO_NUM____SW0, INPUT_PULLUP);
  // pinMode(GPIO_NUM____SW1, INPUT_PULLUP);
  // pinMode(GPIO_NUM____SW2, INPUT_PULLUP);
  // delay(100);
  // Serial.println("- switches ready");

  xTaskCreatePinnedToCore(runLoopTaskSec, "run-loop-sec", 10000, NULL, 2, NULL, 0); // run on secondary core, primary (re)renders LVGL
  xTaskCreatePinnedToCore(runLoopTaskPri, "run-loop-pri", 10000, NULL, 2, NULL, 0); // run on secondary core, primary (re)renders LVGL

}

void loop() {
  //delay(1000);
  //touchDisplaySetBrightness(0xff);
  //delay(1000);
  //touchDisplaySetBrightness(200);
  //delay(1000);
  //touchDisplaySetBrightness(150);
  //delay(1000);
  //touchDisplaySetBrightness(100);
  //delay(1000);
  //touchDisplaySetBrightness(50);
  //delay(1000);
  //touchDisplaySetBrightness(0);
}
