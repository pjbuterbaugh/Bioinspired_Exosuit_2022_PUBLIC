#include "childController.h"

childController::childController(jointSensorPackage &sensorPackage, HardwareSerial *serialTransmit, Serial_ *serialDebug)
{
    this->onboardPackage = sensorPackage;
    this->_serialTransmit = serialTransmit;
    this->_serialDebug = serialDebug;

}

void childController::init()
{
    onboardPackage.attach(_serialTransmit, _serialDebug);
    onboardPackage.init();
    _serialDebug->println("Successfully initialized."); //TODO: Fix me! Shouldn't print unless init comes back true!
    
}

bool childController::transmit()
{
    float transmitStarted = micros();
    packet aPacket = onboardPackage.constructPacket();
    size_t bytesOut = 0;
    double refArray[11];

    for(int i = 0; i<11; i++)
    {
        refArray[i] = aPacket.read(i);
    }


    _serialDebug->println("refArray:");
    _serialDebug->println(refArray[0]);
    _serialDebug->println(refArray[1]);
    _serialDebug->println(refArray[2]);
    _serialDebug->println(refArray[3]);
    _serialDebug->println(refArray[4]);
    _serialDebug->println(refArray[5]);
    _serialDebug->println(refArray[6]);
    _serialDebug->println(refArray[7]);
    _serialDebug->println(refArray[8]);
    _serialDebug->println(refArray[9]);
    _serialDebug->println(refArray[10]);
    //_serialDebug->println(refArray[11]);
    _serialDebug->println("");

    _serialTransmit->print("BGN");

    for(int i = 0; i < sizeof(refArray) / sizeof(double); i++) 
    {
        bytesOut = bytesOut + _serialTransmit->print(refArray[i], 6);
        _serialTransmit->print('A');
    }

    _serialTransmit->print("END");   

    _serialDebug->println("Transmitted ");
    _serialDebug->print(bytesOut);
    _serialDebug->println(" bytes.");
    return (bytesOut !=0);
}

bool childController::transmit(packet aPacket)
{
    float transmitStarted = micros();
    size_t bytesOut = 0;
    float refArray[11];

    for(int i = 0; i<11; i++)
    {
        refArray[i] = aPacket.read(i);
    }

    _serialDebug->println("refArray:");
    _serialDebug->println(refArray[0]);
    _serialDebug->println(refArray[1]);
    _serialDebug->println(refArray[2]);
    _serialDebug->println(refArray[3]);
    _serialDebug->println(refArray[4]);
    _serialDebug->println(refArray[5]);
    _serialDebug->println(refArray[6]);
    _serialDebug->println(refArray[7]);
    _serialDebug->println(refArray[8]);
    _serialDebug->println(refArray[9]);
    _serialDebug->println(refArray[10]);
    _serialDebug->println("");

    _serialTransmit->print("BGN");

    for(int i = 0; i < sizeof(refArray) / sizeof(float); i++) 
    {
        bytesOut = bytesOut + _serialTransmit->print(refArray[i], 6);
        if(i != ((sizeof(refArray) / sizeof(float)) - 1)) _serialTransmit->print(", ");
        _serialDebug->print("For loop count: ");
        _serialDebug->print(i);
        _serialDebug->print('\n');
    
    }

     _serialTransmit->print("END");   

    _serialDebug->println("Transmitted ");
    _serialDebug->print(bytesOut);
    _serialDebug->println(" bytes.");
    return (bytesOut !=0);
}

bool childController::pong()
{
    //TODO: Implement basic functionality.
    
    // double initTime = millis();
    // _serial->listen();
    // _serial->println("Listening...");
    // if(_serialDebug->read() == 0x06)
    // {
    //     _serial->println("Pong! Received data from MEGA.");
    //     _serial->write(0x06);
    //     return true;
    // }else if(millis() - initTime >= 5000)
    // {
    //     _serial->println("Did not receive transmission.");
    //     return false;
    // }
    //_serialDebug->listen();
    _serialDebug->println(_serialTransmit->read());
    return true;
}

void childController::printData()
{
    onboardPackage.printData();
}

packet childController::constructPacket()
{
    return onboardPackage.constructPacket();
}