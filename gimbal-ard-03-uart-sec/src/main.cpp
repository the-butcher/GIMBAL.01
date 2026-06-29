#include <Arduino.h>
#include <SimpleFOC.h>

// #include "UartSrv.h"
#include "I2cSrvSec.h"

uint64_t totalLoopPriCount = 0;

MagneticSensorSPI sensor = MagneticSensorSPI(AS5048_SPI, GPIO_NUM_ENC_CS);
BLDCDriver3PWM driver = BLDCDriver3PWM(GPIO_NUM_MOT_M1, GPIO_NUM_MOT_M2, GPIO_NUM_MOT_M3, GPIO_NUM_MOT_EN);

// 9Ω line resistance :: datasheet :: https://drive.google.com/file/d/1Lx8z6-s6LNnDAv-dHjBgaFPU_OW8bgx9/view
// y-connection :: datasheet :: https://drive.google.com/file/d/1Lx8z6-s6LNnDAv-dHjBgaFPU_OW8bgx9/view
// 9 / 2 :: https://docs.simplefoc.com/phase_resistance
BLDCMotor motor = BLDCMotor(7, 4.5f, 230.0f); // 9Ω / 2 // , 0.001f

Commander command = Commander(Serial);
void doMotor(char* cmd) {
    command.motor(&motor, cmd);
}
void onPid(char* cmd) { command.pid(&motor.P_angle, cmd); }

bool focReady = false;

void runLoopTaskPri(void* pvParameters) {

    vTaskDelay(1); // give the task a chance to return during setup
    while (true) {

        motor.loopFOC();
        motor.move();
        command.run();

        totalLoopPriCount++;

    }

}

void runLoopTaskSec(void* pvParameters) {

    while (true) {

        // read as fast as possible
        // if (UartSrv::readData()) {
        //     Serial.println(UartSrv::lastReadData.z);
        //     digitalWrite(LED_BUILTIN, LOW); // ON
        //     if (focReady) {
        //         motor.target = UartSrv::lastReadData.z;
        //     }
        // }
        if (I2cSrvSec::hasNewData()) {
            float readDataZ = I2cSrvSec::getLastReadData().z;
            Serial.println(String(readDataZ, 2));
            if (focReady) {
                motor.target = readDataZ;
            }
        }

        // read sensor and update the internal variables
        // Serial.print(String(sensor.getAngle() * 180 / PI, 2));
        // Serial.print(", ");
        // Serial.println(String(sensor.getVelocity() * 180 / PI, 2));

        vTaskDelay(1);

    }

}

void setup(void) {

    Serial.begin(115200);
    delay(5000);
    Serial.println("- setup ...");
    // Serial.print("setup, core index: ");
    // Serial.print(xPortGetCoreID());
    // Serial.print(", core count: ");
    // Serial.println(SOC_CPU_CORES_NUM);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW); // ON
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH); // OFF

    // ======================================================================================================

    sensor.init();
    delay(100);
    motor.linkSensor(&sensor);
    delay(100);
    Serial.println("- sensor ready");

    driver.voltage_power_supply = 12;
    driver.voltage_limit = 6;
    if (driver.init()) {

        delay(100);
        motor.linkDriver(&driver);
        delay(100);
        Serial.println("- driver ready");

        // ======================================================================================================

        motor.voltage_sensor_align = 3;
        motor.voltage_limit = 6;

        // https://docs.simplefoc.com/velocity_loop
        motor.LPF_velocity.Tf = 0.05;

        motor.PID_velocity.P = 0.05;
        motor.PID_velocity.I = 1;
        motor.PID_velocity.D = 0.0003;

        // motor.P_angle.P = 30;
        // motor.P_angle.I = 12;
        // motor.P_angle.D = 0;

        // estimated current control
        motor.controller = MotionControlType::angle;
        motor.torque_controller = TorqueControlType::estimated_current;

        motor.updateCurrentLimit(0.8); // A
        motor.target = 0.0;            // A - zero torque command to start

        // motor.useMonitoring(Serial);

        if (motor.init()) {

            delay(100);
            Serial.println("- motor ready");

            if (motor.initFOC()) {

                delay(100);
                Serial.println("- foc ready");

                command.add('M', doMotor, "Motor");
                command.add('C', onPid, "my pid");
                delay(100);
                Serial.println("- command ready");

                focReady = true;


            } else {
                Serial.println("! foc fail");
            }

        } else {
            Serial.println("! motor fail");
        }

    } else {
        Serial.println("! driver fail");
    }

    // UartSrv::powerup();
    // delay(1000);
    // Serial.println("- uart ready");

    I2cSrvSec::powerup();
    delay(1000);
    Serial.println("- i2c ready");


    if (focReady) {
        // start motor task only when everything is ready
        xTaskCreatePinnedToCore(runLoopTaskPri, "run-loop-pri", 10000, NULL, 2, NULL, 1); // run on primary core
        Serial.println("- run-loop-pri");
    }

    xTaskCreatePinnedToCore(runLoopTaskSec, "run-loop-sec", 10000, NULL, 2, NULL, 0); // run on secondary core
    Serial.println("- run-loop-sec");

}


void loop() {

    vTaskDelay(1000);

}
