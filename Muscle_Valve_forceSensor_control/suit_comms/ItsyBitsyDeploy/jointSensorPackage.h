#ifndef __JOINT_SENSOR_PACKAGE_H
#define __JOINT_SENSOR_PACKAGE_H

#include "Adafruit_BNO055.h"
#include "HX711.h"
// #include <avr/variants/standard/pins_arduino.h>
#include <arduino.h>
#include "quaternion.h"
#include "packet.h"
#include "sensorData.h"
#include <Wire.h>
#include <HardwareSerial.h>
#include <USBAPI.h>

//TODO: Implement pressure sensors.

enum class sensorPacks 
{
    IMU_0, IMU_1, LC_0, LC_1, LC_2

};

class jointSensorPackage 
{
    private:
        Adafruit_BNO055 IMU0;
        Adafruit_BNO055 IMU1;
        HX711 LC0;
        HX711 LC1;
        HX711 LC2;
        sensorData data;
        HardwareSerial * _serialTransmit;
        Serial_ *_serialDebug;

        int LC0_scale = 1000;
        int LC1_scale = -1000;
        int LC2_scale = -1000;

        long fetchTime[5];

        //imu::Quaternion getQuat(sensorPacks aSensor); Changing visibility for debugging
        byte _IMU0_ADDR = 0;
        byte _IMU1_ADDR = 0; 
        uint8_t _LC0_P1 = 0, 
                _LC0_P2 = 0,
                _LC1_P1 = 0,
                _LC1_P2 = 0, 
                _LC2_P1 = 0, 
                _LC2_P2 = 0;
    public: 
        jointSensorPackage(byte IMU0_ADDR, byte IMU1_ADDR, uint8_t LC0_P1, 
                                    uint8_t LC0_P2, uint8_t LC1_P1, uint8_t LC1_P2,
                                    uint8_t LC2_P1, uint8_t LC2_P2, HardwareSerial *serialTransmit, 
                                    Serial_ *serialDebug) : _IMU0_ADDR(IMU0_ADDR),
                                    _IMU1_ADDR(IMU1_ADDR), _LC0_P1(LC0_P1), _LC0_P2(LC0_P2),
                                    _LC1_P1(LC1_P1), _LC1_P2(LC1_P2), _LC2_P1(LC2_P1), 
                                    _LC2_P2(LC2_P2), _serialTransmit(serialTransmit), _serialDebug(serialDebug) {}
        jointSensorPackage(byte IMU0_ADDR, byte IMU1_ADDR, uint8_t LC0_P1, 
                                    uint8_t LC0_P2, uint8_t LC1_P1, uint8_t LC1_P2, HardwareSerial *serialTransmit, 
                                    Serial_ *serialDebug) : _IMU0_ADDR(IMU0_ADDR), _IMU1_ADDR(IMU1_ADDR), 
                                    _LC0_P1(LC0_P1), _LC0_P2(LC0_P2), _LC1_P1(LC1_P1), _LC1_P2(LC1_P2),
                                    _serialTransmit(serialTransmit), _serialDebug(serialDebug) {}
        jointSensorPackage(byte IMU0_ADDR, byte IMU1_ADDR, uint8_t LC0_P1, 
                                    uint8_t LC0_P2, uint8_t LC1_P1, uint8_t LC1_P2,
                                    uint8_t LC2_P1, uint8_t LC2_P2) : _IMU0_ADDR(IMU0_ADDR),
                                    _IMU1_ADDR(IMU1_ADDR), _LC0_P1(LC0_P1), _LC0_P2(LC0_P2),
                                    _LC1_P1(LC1_P1), _LC1_P2(LC1_P2), _LC2_P1(LC2_P1), 
                                    _LC2_P2(LC2_P2) {}
        jointSensorPackage(byte IMU0_ADDR) : _IMU0_ADDR(IMU0_ADDR) {}
        jointSensorPackage() {}
        float * getData(sensorPacks aSensor);
        void printData();
        //imu::Quaternion getQuat(sensorPacks aSensor);
        packet constructPacket();
        bool init();
        void attach(HardwareSerial *serialTransmit, Serial_ *serialDebug);
        bool calibrate();
        /**
         * @brief
         * 
         * @param IMU IMU to Read
         * @return sensorData
         * 
         */
        sensorData attemptIMUReading(sensorPacks IMU, uint16_t duration);

};

#endif