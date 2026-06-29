#include <Wire.h>
#include <lvgl.h>

#include "Define.h"

#include "TouchDisplay_Ft3168Sh8601.h"
#include "Orientation.h"
// #include "UartSrv.h"
#include "I2cSrvPri.h"

uint64_t totalLoopPriCount = 0;
vector________t data = { 0, 0, 0 };

float oz = 0.0;

void runLoopTaskPri(void* pvParameters) {

  while (true) {

    Orientation::read();

    vector________t orientation = Orientation::getOrientation();
    // orientation.z = oz;
    // oz += 0.01;
    // UartSrv::sendData(orientation); // send data as fast as possible
    I2cSrvPri::sendData(orientation);

    vTaskDelay(10);
    totalLoopPriCount++;

  }

}

void runLoopTaskSec(void* pvParameters) {

  while (true) {

    vector________t orientation = Orientation::getOrientation();
    Serial.printf("{\"x\":%s,\"y\":%s,\"z\":%s}\n", String(orientation.x, 2), String(orientation.y, 2), String(orientation.z, 2));

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

  Wire1.begin(GPIO_NUM___SDA1, GPIO_NUM___SCL1, 0);
  Orientation::powerup();
  delay(100);
  Serial.println("- orientation ready");

  // UartSrv::powerup();
  // delay(100);
  // Serial.println("- uart ready");

  I2cSrvPri::powerup();
  delay(100);
  Serial.println("- i2c ready");

  pinMode(GPIO_NUM____SW0, INPUT_PULLUP);
  pinMode(GPIO_NUM____SW1, INPUT_PULLUP);
  pinMode(GPIO_NUM____SW2, INPUT_PULLUP);
  delay(100);
  Serial.println("- switches ready");

  xTaskCreatePinnedToCore(runLoopTaskSec, "run-loop-sec", 10000, NULL, 2, NULL, 0); // run on secondary core, primary (re)renders LVGL
  xTaskCreatePinnedToCore(runLoopTaskPri, "run-loop-pri", 10000, NULL, 2, NULL, 0); // run on secondary core, primary (re)renders LVGL

}

void loop() {
  //delay(1000);
  //set_amoled_backlight(0xff);
  //delay(1000);
  //set_amoled_backlight(200);
  //delay(1000);
  //set_amoled_backlight(150);
  //delay(1000);
  //set_amoled_backlight(100);
  //delay(1000);
  //set_amoled_backlight(50);
  //delay(1000);
  //set_amoled_backlight(0);
}
