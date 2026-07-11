#include <Wire.h>
#include <lvgl.h>

#include "Define.h"

#include "disp/TouchDisplay_Ft3168Sh8601.h"
#include "sens/SensorOrientation.h"
#include "coms/ModuleWifi.h"
// #include "UartSrv.h"
#include "coms/I2cSrvPri.h"

uint64_t totalLoopPriCount = 0;
vector________t data = { 0, 0, 0 };
bool wire1HasBegun = false;

float oz = 0.0;

void runLoopTaskPri(void* pvParameters) {

  while (true) {

    if (wire1HasBegun) {
      SensorOrientation::read();
      vector________t orientation = SensorOrientation::getOrientation();
      // orientation.z = oz;
      // oz += 0.01;
      // UartSrv::sendData(orientation); // send data as fast as possible
      I2cSrvPri::sendData(orientation);
    }

    vTaskDelay(10);
    totalLoopPriCount++;

  }

}

void runLoopTaskSec(void* pvParameters) {

  while (true) {

    if (wire1HasBegun) {
      vector________t orientation = SensorOrientation::getOrientation();
      Serial.printf("{\"x\":%s,\"y\":%s,\"z\":%s}\n", String(orientation.x, 2), String(orientation.y, 2), String(orientation.z, 2));
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

  TouchDisplay_Ft3168Sh8601::touchDisplayBegin();
  delay(100);
  Serial.println("- touch display ready");

  // wire1HasBegun = Wire1.begin(GPIO_NUM___SDA1, GPIO_NUM___SCL1, 0);
  wire1HasBegun = false;
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
    I2cSrvPri::powerup();
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
