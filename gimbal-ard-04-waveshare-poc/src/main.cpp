#include <Wire.h>
#include <lvgl.h>

#include "Define.h"

#include "disp/TouchDisplay.h"
#include "sens/SensorBno085.h"
// #include "sens/SensorRotEnc.h"
#include "coms/ModuleWifi.h"
#include "coms/UartSrv.h"
// #include "coms/I2cSrvPri.h"
// #include "coms/Nowsrv.h"

uint64_t totalLoopPriCount = 0;
vector________t data = { 0, 0, 0 };
bool wire1HasBegun = false;

/**
 * reads orientation sensor whenever a new message can be sent through uart
 */
void runLoopTaskPri(void* pvParameters) {

  uint64_t millisM = 1;
  uint64_t millisA;
  uint64_t millisB;

  vTaskDelay(1);
  while (true) {

    millisA = millis();

    if (wire1HasBegun) { // i2c (orientation ok && no previous message pending)
      if (SensorBno085::read()) {
        // TODO :: increment read count to determine frequency of orientation sensor read
        // vector________t orientation = SensorBno085::getOrientation();
        // UartSrv::sendData(orientation); // send data as fast as possible
        // I2cSrvPri::sendData(orientation);
        // NowSrv::sendData(orientation);
        UartSrv::sendData(SensorBno085::getOrientation());
      }
    } 
    taskYIELD();
    // else {
    //   vTaskDelay(1); // TODO :: find out if zero delay is possible, or if a small delay is needed to avoid starving other tasks
    // }
     
    millisB = millis();
    totalLoopPriCount++;

  }

}

/**
 * reads rotary encoder
 */
void runLoopTaskSec(void* pvParameters) {

  while (true) {

    if (wire1HasBegun) {
      // SensorRotEnc::read(); // read the rotary encoder
    }

    vTaskDelay(100);

  }

}

/**
 * reads orientation sensor every 1000ms to write it to serial
 */
void runLoopTaskTri(void* pvParameters) {

  while (true) {

    if (wire1HasBegun) {

      // TODO :: send new data (uart or i2c, will beed slipring protocol POC)
      // vector________t sendData = SensorOrientation::getOrientation();
      // Serial.printf("{\"x\":%s,\"y\":%s,\"z\":%s} - %s - %s\n", String(sendData.x, 2), String(sendData.y, 2), String(sendData.z, 2), String(NowSrv::totalSendCount), String(NowSrv::totalSendCount * 1000 / (millis() - NowSrv::firstSendMillis)));
      //Serial.printf("%s - %s - %s\n", String(SensorBno085::totalReadCount), String(SensorBno085::totalReadCount * 1000.0 / (millis() - SensorBno085::firstReadMillis)), String(SensorRotEnc::getPosition()));
      Serial.printf("%s - %s\n", String(SensorBno085::totalReadCount), String(SensorBno085::totalReadCount * 1000.0 / (millis() - SensorBno085::firstReadMillis)));

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

  TouchDisplay::touchDisplayBegin(); // will start the LVGL task on the primary core
  delay(100);
  Serial.println("- touch display ready");

  // initialize wire1 at 400mHz (default I2C is used for touch display, so we need a second I2C bus for orientation sensor)
  wire1HasBegun = Wire1.begin(GPIO_NUM___SDA1, GPIO_NUM___SCL1, 300000); //  , I2C_FREQ__WIRE1);
  if (wire1HasBegun) {
    delay(100);
    Serial.println("- wire1 ready");
  } else {
    Serial.println("! wire1 not ready");
  }

  if (SensorBno085::powerup()) {
    delay(100);
    Serial.println("- orientation ready");
  } else {
    Serial.println("! orientation not ready");
  }

  // if (SensorRotEnc::powerup()) {
  //   delay(100);
  //   Serial.println("- encoder ready");
  // } else {
  //   Serial.println("! encoder not ready");
  // }

   if (UartSrv::powerup()) {
    delay(100);
    Serial.println("- uart ready");
  } else {
    Serial.println("! uart not ready");
  }

  // if (wire1HasBegun) {
  //   // I2cSrvPri::powerup();
  //   NowSrv::powerup();
  //   delay(100);
  //   Serial.println("- i2c ready");
  // } else {
  //   Serial.println("! i2c not ready");
  // }

  ModuleWifi::powerup();
  delay(100);
  Serial.println("- wifi ready");

  // pinMode(GPIO_NUM____SW0, INPUT_PULLUP);
  // pinMode(GPIO_NUM____SW1, INPUT_PULLUP);
  // pinMode(GPIO_NUM____SW2, INPUT_PULLUP);
  // delay(100);
  // Serial.println("- switches ready");

  xTaskCreatePinnedToCore(runLoopTaskSec, "run-loop-sec", 10000, NULL, 2, NULL, 0); // run on secondary core, primary core (re)renders LVGL
  xTaskCreatePinnedToCore(runLoopTaskPri, "run-loop-pri", 10000, NULL, 2, NULL, 0); // run on secondary core, primary core (re)renders LVGL
  xTaskCreatePinnedToCore(runLoopTaskTri, "run-loop-tri", 10000, NULL, 2, NULL, 0); // run on secondary core, primary core (re)renders LVGL

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
