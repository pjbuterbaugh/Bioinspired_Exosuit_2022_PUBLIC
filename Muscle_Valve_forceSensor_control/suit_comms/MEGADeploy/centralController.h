#ifndef __CENTRALCONTROLLER_H
#define __CENTRALCONTROLLER_H

#include <Arduino.h>
#include "HardwareSerial.h"
#include "packet.h"
// #include <mega/pins_arduino.h>
#include "jointSensorPackage.h"

/** Notes
 * 
 * ! TO IMPLEMENT:
 * //TODO: Implement basic communications w/ child uCs 
 * //TODO: Implement packet structure for sending force, orientation, etc. data
 * //TODO: Implement code for RECEIVING data
 * 
 */


enum class jointPackages {
    package_0, package_1, PC
};

class centralController {
    private:
        HardwareSerial * _serialDebug;
        HardwareSerial * _jointPackage0;
        HardwareSerial * _jointPackage1;

        packet bufferPacketA = packet(),
               bufferPacketB = packet(),
               bufferPacketC = packet();

        String stringBufferA = "",
               stringBufferB = "";

        byte pcBuffer[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


    public:
        centralController();
        centralController(HardwareSerial * jointPackage0, HardwareSerial * jointPackage1);
        centralController(HardwareSerial * jointPackage0, HardwareSerial * jointPackage1, HardwareSerial * serialDebug);
        
        void init();
        void init(HardwareSerial * jointPackage0, HardwareSerial * jointPackage1);
        void listen();
        void listen(jointPackages package);
        void listenString();
        boolean calibrate();
        boolean ping(jointPackages package);
        packet readPackage(jointPackages package); 
        packet parseReading(String processBuffer); 
        packet returnBufferPacket(jointPackages package);   
        String readPackageString(jointPackages package);
        String returnBufferString(jointPackages package);
        bool transmit(packet aPacket, jointPackages toPackages);
        bool transmit(packet aPacket, packet bPacket, jointPackages toPackages);
        bool transmit(String contents, jointPackages toPackages);
        int pcBufferReturn(unsigned int i) { return int(pcBuffer[i]); }


};
#endif
