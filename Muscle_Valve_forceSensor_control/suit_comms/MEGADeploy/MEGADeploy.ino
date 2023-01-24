/**
 * @file MEGADeploy.ino
 * @author Peter Buterbaugh (pjbuterbaugh@wpi.edu)
 * @brief Sketch file for the MEGA 2560, known in code as the <centralController>.
 * 
 * The following represents the current configuration of the suit:
 * - LC 2 @ UART 1 - Left Quad
 * - LC 2 @ UART 2 - Right Quad
 * - LC 3 @ UART 1 - Left Glute
 * - LC 3 @ UART 2 - Right Glute
 * - LC 1 @ UART 1 - Left Calf
 * - LC 1 @ UART 2 - Right Calf
 * 
 * @version ???
 * @date Built: 2022-03-20
 * 
 * 
 */

//TODO:
// - Document using Doxy
// - Add sensor exception handling

#define IS_MEGA // Flag to specify that this is, indeed, an Arduino MEGA-based board.
// #define IS_DEBUGGING

#include "jointSensorPackage.h"
#include "centralController.h"
#include "packet.h"
#include "PressureSensor.h"
#include "gaitProcessor.h"
#include <SoftwareSerial.h>
#include <Arduino.h>
#include "ComDriverSpi.h"
#include "MemoryFree.h"
#include "muscleController.h"
#include <Wire.h> 
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "imumaths.h"

uint8_t n = 0;
float T = 0;
String dataIn,
       tVal,
       receivedBufferA = "",
       receivedBufferB = "";
uint8_t i = 0;


Adafruit_BNO055 bno = Adafruit_BNO055(55);
centralController MEGA = centralController();
PressureSensor footLeft = PressureSensor(0);
PressureSensor footRight = PressureSensor(1);
muscleController muscles = muscleController(51, 35, 15, 65, 56, 50);
gaitProcessor mainProcessor = gaitProcessor(&footLeft, &footRight);
packet curPacket = packet(); // Construct empty packet to be filled
packet receivedPacketA = packet(),
       receivedPacketB = packet();

int lastCalf = 0,
    lastQuad = 0,
    lastGlute = 0,
    lastCalfDuration = 0,
    lastQuadDuration = 0,
    lastGluteDuration = 0;

float qW = 0,
      qX = 0,
      qY = 0,
      qZ = 0,
      lastTimeStamp = 0,
      loopQty = 0,
      cycles = 0,
      overallLoop = 0,
      runTime = 0,
      lastRun = 0,
      avgRun = 0;

//TODO: Implement ISRs *somewhere* to prepare the MEGA to parse readings from computer

void setup()
{
    if(!bno.begin()) Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!"); 

    //!! We're using Serial for debug/computer comms.
    Serial.begin(250000);
    
    //!! We're using Serial1, Serial2 for ItsyBitsy comms.
    Serial1.begin(115200);
    Serial2.begin(115200);


    #ifdef IS_DEBUGGING
    while(!Serial); // Wait until we establish a connection to the computer
    Serial.println("Debugging flag enabled.");
    Serial.print('\n');
    Serial.println("Init timestamp:");
    Serial.println(micros());
    Serial.print('\n');

    Serial.println(freeMemory());
    #endif

    MEGA = centralController(&Serial1, &Serial2, &Serial);
    
    mainProcessor.setSerialPort(&Serial);
    mainProcessor.init();
    muscles.initialize();
    bno.setExtCrystalUse(true); 
    lastTimeStamp = micros();

}

void loop()
{
    lastRun = micros();
    // Serial.print("Processing Frequency (Hz): ");
    // Serial.print(loopQty);
    // Serial.print('\n');
    // Serial.println(cycles);

    // Serial.print("Overall Average (Hz): ");
    // Serial.print(overallLoop / (micros() / 1000000), 4);
    // Serial.print(", ");
    // Serial.print(avgRun);
    // Serial.print('\n');

    // Serial.print("A: ");
    // Serial.print(receivedBufferA);
    // Serial.print('\n');

    // Serial.print("B: ");
    // Serial.print(receivedBufferB);
    // Serial.print('\n');
    
    if(micros() - (lastTimeStamp) >= (500))
    {
        loopQty = cycles * ((micros() - lastTimeStamp) / 1000000);
        cycles = 0;
        lastTimeStamp = micros();
    }else return;


    qW = float((bno.getQuat()).w());
    qX = float((bno.getQuat()).x());
    qY = float((bno.getQuat()).y());
    qZ = float((bno.getQuat()).z());

    if(lastCalf != MEGA.pcBufferReturn(0) || lastQuad != MEGA.pcBufferReturn(1) || lastGlute != MEGA.pcBufferReturn(2) || lastCalfDuration != MEGA.pcBufferReturn(3) || lastQuadDuration != MEGA.pcBufferReturn(4) || lastGluteDuration != MEGA.pcBufferReturn(5))
    {
        muscles.updateAction(MEGA.pcBufferReturn(0), MEGA.pcBufferReturn(1), MEGA.pcBufferReturn(2), MEGA.pcBufferReturn(3), MEGA.pcBufferReturn(4), MEGA.pcBufferReturn(5));

        lastCalf = MEGA.pcBufferReturn(0);
        lastQuad = MEGA.pcBufferReturn(1);
        lastGlute = MEGA.pcBufferReturn(2);
        lastCalfDuration = MEGA.pcBufferReturn(3);
        lastQuadDuration = MEGA.pcBufferReturn(4);
        lastGluteDuration = MEGA.pcBufferReturn(5);
        
    }

    // Serial.println(MEGA.pcBufferReturn(0));
    // Serial.println(MEGA.pcBufferReturn(1));
    // Serial.println(MEGA.pcBufferReturn(2));

    // Serial.println(footLeft.read());
    // Serial.println(footRight.read());

    dataIn = "";
    mainProcessor.runProcessor();
    MEGA.listenString();
    // MEGA.listen();
    T = mainProcessor.returnT();

    // Serial.print(T,5);
    // Serial.print('\n');
    muscles.walk(T);
    tVal = String(T);

    //bno.getQuat()

    
    // receivedPacketA = MEGA.returnBufferPacket(jointPackages::package_0);
    // receivedPacketB = MEGA.returnBufferPacket(jointPackages::package_1);
    receivedBufferA = MEGA.returnBufferString(jointPackages::package_0);
    receivedBufferB = MEGA.returnBufferString(jointPackages::package_1);

    if(receivedBufferA.length() < 1 || receivedBufferB.length() < 1)
    {
        return;
    }

    /*
     * {(ItsyBitsy Serial1 Packet, 11), (ItsyBitsy Serial2 Packet, 11), (MEGA IMU Quaternion, 4), T Value, Gait Cycle Percentage} 
     * 
     */
    dataIn.concat((String) (receivedBufferA + ", " + receivedBufferB + ", " + String(qW,6) + ", " + String(qX,6) + ", " + String(qY,6) + ", " + String(qZ,6) + ", " + tVal + ", " + float(muscles.cycle_L)));

    #ifdef IS_DEBUGGING
    Serial.print('\n');
    Serial.print(receivedPacketA.read(0), 8);
    Serial.print('\n');
    Serial.print(receivedPacketA.read(1), 8);
    Serial.println("");
    Serial.print(receivedPacketB.read(0), 8);
    Serial.print('\n');
    Serial.print(receivedPacketB.read(1), 8);
    #endif 
    
    // MEGA.transmit(receivedPacketA, receivedPacketB, jointPackages::PC);
    MEGA.transmit(dataIn, jointPackages::PC);

    // Serial.println("");

    cycles++;
    overallLoop++;
    runTime = micros() - lastRun;
    avgRun = runTime / overallLoop;
    // Serial.print("It took me ");
    // Serial.print(runTime);
    // Serial.print(" uS to run.");
    // Serial.print('\n');
}
