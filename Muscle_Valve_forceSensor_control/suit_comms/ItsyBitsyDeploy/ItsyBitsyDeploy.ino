#include "jointSensorPackage.h"
#include "packet.h"
#include "childController.h"
#include "PressureSensor.h"
#include "gaitProcessor.h"
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <ComDriverSpi.h>

jointSensorPackage attachedSensors = jointSensorPackage(0x28, 0x29, 12, 11, 10, 9, 7, 5);
childController itsyBitsy = childController();
packet curPacket = packet(); // Construct empty packet to be filled later.

void setup()
{
 Serial1.begin(115200);
 Serial1.setTimeout(10000);
 Serial.begin(115200);
 
//  while(!Serial);

 itsyBitsy = childController(attachedSensors, &Serial1, &Serial);
 itsyBitsy.init();

}

void loop()
{

    Serial.println("Loop!");
    curPacket = itsyBitsy.constructPacket();

    itsyBitsy.transmit(curPacket);
    
}

/**
 * Here's what we want to do.
 * @par 
 * We want our uC to init all sensors and confirm comms, grab 
 * values from our new pressure sensors, and then start to 
 * communicate those values over serial to be read by the
 * MEGA.
 * 
 * The MEGA will take in those comms, parse them such that they
 * turn back into packets, which will in-turn allow individual
 * indices of the packet to be processed.
 * 
 * However, I think I should implement this later, given that
 * we're doing the majority of our work on the tethered higher
 * level computer (laptop).
 * 
 */
