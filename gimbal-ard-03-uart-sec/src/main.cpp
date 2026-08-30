#include <Arduino.h>
#include <SimpleFOC.h>

// #include "coms/I2cSrvSec.h"
#include "Define.h"
#include "coms/NowSrv.h"
#include "util/Kalman.h"

const int PIN_BOOT = GPIO_NUM_0;
uint64_t totalLoopPriCount = 0;

MagneticSensorSPI sensor = MagneticSensorSPI(AS5048_SPI, GPIO_NUM_ENC_CS);
BLDCDriver3PWM driver = BLDCDriver3PWM(GPIO_NUM_MOT_M1, GPIO_NUM_MOT_M2, GPIO_NUM_MOT_M3, GPIO_NUM_MOT_EN);

// 9Ω line resistance :: datasheet :: https://drive.google.com/file/d/1Lx8z6-s6LNnDAv-dHjBgaFPU_OW8bgx9/view
// y-connection :: datasheet :: https://drive.google.com/file/d/1Lx8z6-s6LNnDAv-dHjBgaFPU_OW8bgx9/view
// 9 / 2 :: https://docs.simplefoc.com/phase_resistance
// BLDCMotor motor = BLDCMotor(7, 4.5f, 230.0f); // GM2804 - 9Ω / 2 // , 0.001f
BLDCMotor motor = BLDCMotor(11, 5.6f / 2.0f, 185.0F); // GM3506 - https://community.simplefoc.com/t/how-many-pole-pairs-does-a-gm3506-actually-have/4811/2
// KV was ~184 at 1V, ~189 at 2V, ~160 at 5V

float motorOffset = 0.630f;
float kalmanPredictInterval = 3.000f; // 1.5 intervals, 15 milliseconds ahead
float processNoise = 0.1280f;
float measNoiseStd = 0.0128f;
Commander command = Commander(Serial);
void doMotor(char* cmd) {
    command.motor(&motor, cmd);
}
void onOffs(char* cmd) {
    command.scalar(&motorOffset, cmd);
}
void onKlmn(char* cmd) {
    command.scalar(&kalmanPredictInterval, cmd);
}
void onPrcs(char* cmd) {
    command.scalar(&processNoise, cmd);
    Kalman::begin(0.01f, processNoise, measNoiseStd);
}
void onMeas(char* cmd) {
    command.scalar(&measNoiseStd, cmd);
    Kalman::begin(0.01f, processNoise, measNoiseStd);
}
void onPidV(char* cmd) { command.pid(&motor.PID_velocity, cmd); }
void onPidA(char* cmd) { command.pid(&motor.P_angle, cmd); }

bool focReady = false;
bool isLogVal = false;

void IRAM_ATTR handleBootButton() {
    isLogVal = !isLogVal;
}

void runLoopTaskFoc(void* pvParameters) {

    vTaskDelay(1); // give the task a chance to return during setup
    while (true) {

        if (focReady) {
            motor.loopFOC();

            float kalmanPredictLoop = min(1.0F, (millis() - Kalman::lastValueMillis) / 10.0F) + kalmanPredictInterval;
            motor.move(Kalman::predict(kalmanPredictLoop) + motorOffset);
        }

        command.run();

        totalLoopPriCount++;

    }

}

void runLoopTaskRecv(void* pvParameters) {

    while (true) {

        // read as fast as possible
        // if (I2cSrvSec::hasNewData()) {
        //     float readDataZ = I2cSrvSec::getLastReadData().z;
        //     Serial.println(String(readDataZ, 2));
        //     if (focReady) {
        //         motor.target = readDataZ;
        //     }
        // }
        if (NowSrv::hasNewRecvData()) {
            vector________t recvData = NowSrv::getLastRecvData();
            Kalman::update(-recvData.z);
        }

        vTaskDelay(1);

    }

}

void runLoopTaskVals(void* pvParameters) {

    while (true) {

        float kalmanPredictLoop = min(1.0F, (millis() - Kalman::lastValueMillis) / 10.0F) + kalmanPredictInterval;
        double z = Kalman::predict(0.0F) + motorOffset;
        double v = Kalman::predict(kalmanPredictLoop) + motorOffset;

        if (isLogVal) {

            Serial.print(">z:");
            Serial.println(String(z, 3));

            Serial.print(">v:");
            Serial.println(String(v, 3));

            sensor.update();
            float a = -sensor.getAngle();
            Serial.print(">a:");
            Serial.println(String(a, 3));

        }

        if (isLogVal) {
            vTaskDelay(50);
        } else {
            vTaskDelay(1000);
        }

    }

}

void setup(void) {

    Serial.begin(115200);
    delay(5000);
    Serial.println("- setup ...");

    pinMode(PIN_BOOT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_BOOT), handleBootButton, FALLING);

    // Serial.print("setup, core index: ");
    // Serial.print(xPortGetCoreID());
    // Serial.print(", core count: ");
    // Serial.println(SOC_CPU_CORES_NUM);

    // pinMode(LED_BUILTIN, OUTPUT);
    // digitalWrite(LED_BUILTIN, LOW); // ON
    // delay(1000);
    // digitalWrite(LED_BUILTIN, HIGH); // OFF

    // ======================================================================================================

    /**
     * Higher processNoise → filter trusts new measurements more, tracks fast changes better but is noisier.
     * Higher measNoiseStd → filter trusts the model more, smooths harder, reacts slower to real changes.
     * Start with measNoiseStd ≈ the actual noise level of your sensor, and sweep processNoise until the filtered output looks smooth but still tracks real acceleration changes.
     */
    Kalman::begin(0.01f, processNoise, measNoiseStd);
    delay(100);

    sensor.init();
    delay(100);
    motor.linkSensor(&sensor);
    delay(100);
    Serial.println("- sensor ready");

    driver.voltage_power_supply = 12.0;
    driver.voltage_limit = 11.1;
    if (driver.init()) {

        delay(100);
        motor.linkDriver(&driver);
        delay(100);
        Serial.println("- driver ready");

        // ======================================================================================================

        motor.voltage_sensor_align = 3; // Limits voltage (and therefore current) during motor alignment. Value in Volts.
        motor.voltage_limit = 11.1;
        motor.current_limit = 1.0;
        motor.velocity_limit = PI * 2 * 3; // 3 rpm https://docs.simplefoc.com/position_control_example

        motor.LPF_velocity.Tf = 0.10; // https://docs.simplefoc.com/velocity_loop

        motor.PID_velocity.P = 0.100;
        motor.PID_velocity.I = 0.100;
        motor.PID_velocity.D = 0.000;
        // motor.PID_velocity.output_ramp = 1000; // https://docs.simplefoc.com/position_control_example

        motor.P_angle.P = 38.000;
        motor.P_angle.I = 8.000;
        motor.P_angle.D = 0.010;

        // estimated current control
        motor.controller = MotionControlType::angle;
        motor.torque_controller = TorqueControlType::estimated_current;

        // motor.updateCurrentLimit(0.8); // A :: is set further up
        motor.target = 0.0;            // A - zero torque command to start

        // motor.useMonitoring(Serial);

        if (motor.init()) {

            delay(100);
            Serial.println("- motor ready");

            if (motor.initFOC()) {

                delay(100);
                Serial.println("- foc ready");

                // command.add('M', doMotor, "Motor");
                command.add('V', onPidV, "my pid v");
                command.add('A', onPidA, "my pid a");
                // command.add('L', onLtnc, "my latency");
                command.add('K', onKlmn, "my kalman interval");
                command.add('P', onPrcs, "my process noise");
                command.add('M', onMeas, "my measurement noise");
                command.add('O', onOffs, "my motor offset");

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

    // begin wire as "master"
    Wire.begin(SDA1, SCL1, 0);

    // UartSrv::powerup();
    // delay(1000);
    // Serial.println("- uart ready");

    // I2cSrvSec::powerup();
    NowSrv::powerup();
    delay(1000);
    // Serial.println("- i2c ready");
    Serial.println("- espnow ready");


    if (focReady) {
        // start motor task only when everything is ready
        xTaskCreatePinnedToCore(runLoopTaskFoc, "run-loop-pri", 10000, NULL, 2, NULL, 1); // run on primary core
        Serial.println("- run-loop-pri");
    }

    xTaskCreatePinnedToCore(runLoopTaskRecv, "run-loop-sec", 10000, NULL, 2, NULL, 0); // run on secondary core
    Serial.println("- run-loop-sec");

    xTaskCreatePinnedToCore(runLoopTaskVals, "run-loop-tri", 10000, NULL, 2, NULL, 0); // run on secondary core
    Serial.println("- run-loop-tri");
}


void loop() {

    vTaskDelay(1000);

}
