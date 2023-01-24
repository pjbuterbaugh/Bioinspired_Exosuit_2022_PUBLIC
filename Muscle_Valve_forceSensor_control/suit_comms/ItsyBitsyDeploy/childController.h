#ifndef __CHILD_CONTROLLER_H
#define __CHILD_CONTROLLER_H

#include "Adafruit_BNO055.h"
#include <Arduino.h>
#include "HX711.h"
#include "packet.h"
// #include "avr/variants/standard/pins_arduino.h"
#include "jointSensorPackage.h"
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <USBAPI.h>

// To run on ItsyBitsy boards handling multiple sensors

class childController {
    private: 
        jointSensorPackage onboardPackage;
        HardwareSerial * _serialTransmit;
        Serial_ *_serialDebug;
        //SoftwareSerial serialOut = SoftwareSerial(10,11); // 10 RX, 11 TX
    
    public:
        childController() {}
        childController(jointSensorPackage &sensorPackage, HardwareSerial *serialTransmit, Serial_ *serialDebug);
        //void init(int LC0DPin, int LC0CPin, int LC1DPin, int LC1CPin);
        void init();
        bool transmit();
        bool transmit(packet aPacket);
        bool pong();
        void printData();
        packet constructPacket();
        //byte grabIMU();
        


    
};

#endif