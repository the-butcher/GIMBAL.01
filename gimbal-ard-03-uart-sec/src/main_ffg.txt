#include <Arduino.h>
#include <SimpleFOC.h>

// #include "coms/I2cSrvSec.h"
#include "Define.h"
// #include "coms/NowSrv.h"
#include "coms/UartSrv.h"
#include "util/AlphaBeta.h"
#include "util/Gain.h"
#include "util/SignalUtil.h"
#include "util/ComsUtil.h"


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

float motorOffset = -1.672f; // command "O"
const float VELOCITY_LIMIT = PI * 2 * 2;

PIDController PID_angle_Y{ 10.000f, 0.010f, 0.000f, 10000.0f, VELOCITY_LIMIT }; // command "YP | YI | YD"
uint64_t lastRecvMicros;

float yawFilterA = 0.500f; // command "A"
float yawFilterB = 0.100f; // command "B"
float yawFilterG = 0.000f; // command "G"
AlphaBeta yawFilter(yawFilterA, yawFilterB, yawFilterG, false);

const float MAX_PREDICT_SECONDS = 0.100f; // max predict seconds to prevent runoff
float signalLatencySeconds = 0.025f; // seconds, command "S"

float kff = 1.1f; // command "K"
float lowPassFiltV = 0.020f; // command "L"

Commander command = Commander(Serial);
// void onMotor(char* cmd) { command.target(&motor, cmd); }
void onLpfV(char* cmd) {
    command.scalar(&lowPassFiltV, cmd);
    motor.LPF_velocity.Tf = lowPassFiltV;
}
void onKff(char* cmd) {
    command.scalar(&kff, cmd);
}
void onOffs(char* cmd) {
    command.scalar(&motorOffset, cmd);
}
void onYawA(char* cmd) {
    command.scalar(&yawFilterA, cmd);
    yawFilter.setGains(yawFilterA, yawFilterB, yawFilterG);
}
void onYawB(char* cmd) {
    command.scalar(&yawFilterB, cmd);
    yawFilter.setGains(yawFilterA, yawFilterB, yawFilterG);
}
void onYawG(char* cmd) {
    command.scalar(&yawFilterG, cmd);
    yawFilter.setGains(yawFilterA, yawFilterB, yawFilterG);
}
void onSls(char* cmd) {
    command.scalar(&signalLatencySeconds, cmd);
}
void onPidV(char* cmd) { command.pid(&motor.PID_velocity, cmd); }
void onPidY(char* cmd) { command.pid(&PID_angle_Y, cmd); }

bool focReady = false;
bool isLogVal = false;

void IRAM_ATTR handleBootButton() {
    isLogVal = !isLogVal;
}

float velocityCommand1;
float velocityCommand2;
float velocityCommandT;
float angleError;

void runLoopTaskFoc(void* pvParameters) {

    vTaskDelay(1); // give the task a chance to return during setup
    while (true) {

        if (focReady) {

            motor.loopFOC();

            // time elapsed since last value
            float dt = (micros() - lastRecvMicros) * 1e-6f;
            float predictSeconds = min(MAX_PREDICT_SECONDS, dt + signalLatencySeconds);

            float angleTarget = yawFilter.predict(predictSeconds) + motorOffset;
            float velocTarget = yawFilter.getVelocity();

            angleError = angleTarget - motor.shaft_angle;
            velocityCommand1 = PID_angle_Y(angleError);
            velocityCommand2 = velocTarget * kff;
            velocityCommandT = constrain(velocityCommand1 + velocityCommand2, -motor.velocity_limit, motor.velocity_limit);

            motor.move(velocityCommandT);
            // motor.move();

            command.run();

        } else {

            command.run();
            
            vTaskDelay(1);

        }

        

        totalLoopPriCount++;

    }

}

/**
 * read from uart as fast as possible, update yaw filter with new data
 */
void runLoopTaskRecv(void* pvParameters) {

    while (true) {

        // read as fast as possible
        UartSrv::readData();
        if (UartSrv::hasNewRecvData()) {
            uint64_t now = micros();
            float dt = (now - lastRecvMicros) * 1e-6f;
            lastRecvMicros = now;
            yawFilter.update(-UartSrv::getLastRecvData().z, dt);
        }

        vTaskDelay(1);

    }

}

void runLoopTaskVals(void* pvParameters) {

    while (true) {

        float dt = (micros() - lastRecvMicros) * 1e-6f;
        float predictSeconds = min(MAX_PREDICT_SECONDS, dt + signalLatencySeconds);

        float abr = -UartSrv::getLastRecvData().z + motorOffset;
        float ab0 = yawFilter.predict(0) + motorOffset;
        float abi = yawFilter.predict(predictSeconds) + motorOffset;

        float acc = yawFilter.getAcceleration();
        float vlc = yawFilter.getVelocity();

        if (isLogVal) {

            Serial.print(">abr:");
            Serial.println(String(abr, 3));
            Serial.print(">ab0:");
            Serial.println(String(ab0, 3));
            Serial.print(">abi:");
            Serial.println(String(abi, 3));

            // Serial.print(">fg2:");
            // Serial.println(String(valueFfg, 3));
            // Serial.print(">acc:");
            // Serial.println(String(acc, 3));

            // Serial.print(">pd2:");
            // Serial.println(String(valuePid, 3));
            // Serial.print(">vlp:");
            // Serial.println(String(vlc, 3));



            // // sensor.update();
            // float rad = motor.shaft_angle;
            // Serial.print(">rad:");
            // Serial.println(String(rad, 3));

            // Serial.print(">aer:");
            // Serial.println(String(angleError, 3));

            // Serial.print(">vl1:");
            // Serial.println(String(velocityCommand1, 3));

            // Serial.print(">vl2:");
            // Serial.println(String(velocityCommand2, 3));

            vTaskDelay(10);

        } else {

            // float primaryLoopFrequency = totalLoopPriCount * 1000.0f / millis(); // hZ
            // Serial.print(">plf:");
            // Serial.println(String(primaryLoopFrequency, 3));

            // Serial.println(String(UartSrv::totalRecvCount));

            Serial.printf("%s - %s\n", String(UartSrv::totalRecvCount), String(UartSrv::totalRecvCount * 1000.0 / (millis() - UartSrv::firstRecvMillis)));

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

    SignalUtil::powerup();
    SignalUtil::setColor(0, 255, 0); // green;
    
    vector________t rot = { 0.0f, 0.0f, 0.0f };
    uint32_t checksum = ComsUtil::calcCrcOfVector(rot);

    Serial.print("CRC-8 Checksum: 0x");
    Serial.println(checksum, HEX);

    // Serial.print("setup, core index: ");
    // Serial.print(xPortGetCoreID());
    // Serial.print(", core count: ");
    // Serial.println(SOC_CPU_CORES_NUM);

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
        motor.velocity_limit = VELOCITY_LIMIT; // 1 rps for the beginning https://docs.simplefoc.com/position_control_example

        motor.PID_velocity.P = 0.150;
        motor.PID_velocity.I = 0.001;
        motor.PID_velocity.D = 0.000;
        motor.LPF_velocity.Tf = lowPassFiltV; // https://docs.simplefoc.com/velocity_loop
        // motor.PID_velocity.limit = motor.current_limit;
        // motor.PID_velocity.output_ramp = 1000; // https://docs.simplefoc.com/position_control_example

        // estimated current control
        motor.controller = MotionControlType::velocity;
        motor.torque_controller = TorqueControlType::estimated_current;

        motor.target = 0.0;

        if (motor.init()) {

            delay(100);
            Serial.println("- motor ready");

            // command.add('M', onMotor, "my motor command");
            command.add('V', onPidV, "my pid v");
            command.add('Y', onPidY, "my pid y");
            command.add('A', onYawA, "my yaw gain a");
            command.add('B', onYawB, "my yaw gain b");
            command.add('G', onYawG, "my yaw gain g");
            command.add('O', onOffs, "my motor offset");
            command.add('L', onLpfV, "my low pass constant v");
            command.add('S', onSls, "my signal latency");
            command.add('K', onKff, "my ffg gain");

            delay(100);
            Serial.println("- command ready");

            if (motor.initFOC()) {

                delay(100);
                Serial.println("- foc ready");
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

    UartSrv::powerup();
    delay(1000);
    Serial.println("- uart ready");

    xTaskCreatePinnedToCore(runLoopTaskFoc, "run-loop-pri", 10000, NULL, 2, NULL, 1); // run on primary core
    Serial.println("- run-loop-foc");

    xTaskCreatePinnedToCore(runLoopTaskRecv, "run-loop-sec", 10000, NULL, 2, NULL, 0); // run on secondary core
    Serial.println("- run-loop-rec");

    xTaskCreatePinnedToCore(runLoopTaskVals, "run-loop-tri", 10000, NULL, 2, NULL, 0); // run on secondary core
    Serial.println("- run-loop-val");

    lastRecvMicros = micros();
    yawFilter.init(motorOffset); // seed with real starting angle

}


void loop() {

    vTaskDelay(1000);

}
