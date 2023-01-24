#include "centralController.h"
// #define IS_DEBUGGING;

centralController::centralController() {
    
}

centralController::centralController(HardwareSerial *jointPackage0, HardwareSerial *jointPackage1)
{
    this->_jointPackage0 = jointPackage0;
     this->_jointPackage1 = jointPackage1;

}

centralController::centralController(HardwareSerial *jointPackage0, HardwareSerial *jointPackage1, HardwareSerial *serialDebug)
{
    this->_jointPackage0 = jointPackage0;
    this->_jointPackage1 = jointPackage1;
    this->_serialDebug = serialDebug;
}

void centralController::init(){
 //TODO: Implement "handshake" to confirm bidirectional comms.

//  _jointPackage0->begin(115200);
 if(_jointPackage0) _serialDebug->println("ItsyBitsy 0 Connection Established.");
//  _jointPackage1->begin(115200);
 if(_jointPackage1) _serialDebug->println("ItsyBitsy 1 Connection Established.");
 

}

void centralController::init(HardwareSerial * jointPackage0, HardwareSerial * jointPackage1){
 //TODO: Implement "handshake" to confirm bidirectional comms.

 this->_jointPackage0 = jointPackage0;
 this->_jointPackage1 = jointPackage1;

//  _jointPackage0->begin(115200);
//!! This crashes the Arduino. I have no idea why.
// if(_jointPackage0) _serialDebug->println("ItsyBitsy 0 Connection Established.");

//  _jointPackage1->begin(115200);
//  if(_jointPackage1) _serialDebug->println("ItsyBitsy 1 Connection Established.");
 

}

//!! Blocking code!
packet centralController::readPackage(jointPackages package)
{
    String bufferIn, 
           lastData, 
           lastDecimals, 
           lastInt = "";
    float bufferIndex[12],
           divisor = 0,
           newFloat = 0;
    unsigned int bucketLength[12];
    float initTime = millis();
    unsigned int knownLength = 0, 
                 decimalPoints = 0,
                 endOfValue = 0,
                 endOfInt = 0;

    switch(package)
    {
            case(jointPackages::package_0):
                bufferIn = _jointPackage0->readStringUntil('END');
                break;
            case(jointPackages::package_1):
                bufferIn = _jointPackage1->readStringUntil('END');
                break;
            default:
                _serialDebug->println("This should not happen!");
            break;
    }

    #ifdef IS_DEBUGGING
    _serialDebug->println(bufferIn);
    #endif

    unsigned int bufferLen = bufferIn.length();
for(uint8_t i = 0; i<11; i++)
    {
        endOfValue = bufferIn.indexOf(',', 0);
        endOfInt = bufferIn.indexOf('.', 0);
        lastData = bufferIn.substring(0, endOfValue + 1);
        
        #ifdef IS_DEBUGGING  
        _serialDebug->println(endOfInt);
        _serialDebug->println(lastData);
        #endif

        lastData.remove(0, endOfInt+1);
        lastInt = lastData;

        lastData.remove(0, lastData.indexOf('.'));
        // lastData.trim();
        decimalPoints = lastInt.length();

        #ifdef IS_DEBUGGING
        _serialDebug->println(lastInt);
        _serialDebug->println(decimalPoints);
        #endif

        newFloat = lastInt.toInt();
        divisor = pow(10, decimalPoints);

        
        bufferIndex[i] = (newFloat / divisor);

        #ifdef IS_DEBUGGING
        _serialDebug->println(newFloat);
        _serialDebug->println(bufferIndex[i]);
        #endif

        if(bufferIn.indexOf('-', 0) < 2)
        {
            bufferIndex[i] = bufferIndex[i] * -1;
        }

        bufferIn.remove(0, endOfValue + 2);
        
        #ifdef IS_DEBUGGING
        _serialDebug->println(bufferIn);
        #endif 
        // bufferIn.trim();
        
    }


    
    return packet(bufferIndex);

}

String centralController::readPackageString(jointPackages package)
{
    String bufferIn, 
           lastData, 
           lastDecimals, 
           lastInt = "";
    float bufferIndex[12],
           divisor = 0,
           newFloat = 0;
    unsigned int bucketLength[12];
    float initTime = millis();
    unsigned int knownLength = 0, 
                 decimalPoints = 0,
                 endOfValue = 0,
                 endOfInt = 0;
    byte intBuffer[6] = {0,0,0};

    switch(package)
    {
            case(jointPackages::package_0):
                bufferIn = _jointPackage0->readStringUntil('END');
                break;
            case(jointPackages::package_1):
                bufferIn = _jointPackage1->readStringUntil('END');
                break;
            case(jointPackages::PC):
                _serialDebug->readBytes(intBuffer, 6);


            break;
            
            default:
                _serialDebug->println("This should not happen!");
            break;
    }

    // #ifdef IS_DEBUGGING
    // _serialDebug->println(bufferIn);
    // #endif
  
    bufferIn.remove(0,3);
    bufferIn.remove(bufferIn.length() - 3, 3);

    // _serialDebug->print("readPackage Out: ");
    // _serialDebug->print(bufferIn);
    // _serialDebug->print('\n');
    return bufferIn;

}

packet centralController::parseReading(String processBuffer)
{
/**
 * TODO: Grab data from Serial bus when we get an interrupt indicating that data is coming our way, parse
 * ! Structure: 4 values (ints) representing ham, calf, glute, quad, percents, not necessary for both legs
 */

    

}

bool centralController::transmit(packet aPacket, jointPackages toPackage)
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


    switch(toPackage)
    {
        case jointPackages::package_0:
            _jointPackage0->print("BGN");

            for(int i = 0; i < sizeof(refArray) / sizeof(float); i++) 
            { 
            bytesOut = bytesOut + _jointPackage0->print(refArray[i],6);
            _jointPackage0->print('A');
            }

            _jointPackage0->print("END");
        break;
        
        case jointPackages::package_1:
            _jointPackage1->print("BGN");

            for(int i = 0; i < sizeof(refArray) / sizeof(float); i++) 
            { 
            bytesOut = bytesOut + _jointPackage1->print(refArray[i],6);
            _jointPackage1->print('A');
            }
            
            _jointPackage0->print("END");
        break;

        case jointPackages::PC:
            _serialDebug->print("BGN");

            for(int i = 0; i < sizeof(refArray) / sizeof(float); i++) 
            { 
            bytesOut = bytesOut + _serialDebug->print(refArray[i],6);


            if(i != ((sizeof(refArray) / sizeof(float)) - 1)) _serialDebug->print(',');
            }
            
            _serialDebug->print("END");
        break;

        default:
        _serialDebug->println("This should not happen!");
        break;
    }

    _serialDebug->println("Transmitted ");
    _serialDebug->print(bytesOut);
    _serialDebug->println(" bytes.");
    return (bytesOut !=0);
}

bool centralController::transmit(packet aPacket, packet bPacket, jointPackages toPackage)
{
    float transmitStarted = micros();
    size_t bytesOut = 0;
    float refArray[22];
    
    for(int i = 0; i<11; i++)
    {
        refArray[i] = aPacket.read(i);
        refArray[i + 11] = bPacket.read(i);
    }

    #ifdef IS_DEBUGGING
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
    _serialDebug->println(refArray[11]);
    _serialDebug->println(refArray[12]);
    _serialDebug->println(refArray[13]);
    _serialDebug->println(refArray[14]);
    _serialDebug->println(refArray[15]);
    _serialDebug->println(refArray[16]);
    _serialDebug->println(refArray[17]);
    _serialDebug->println(refArray[18]);
    _serialDebug->println(refArray[19]);
    _serialDebug->println(refArray[20]);
    _serialDebug->println(refArray[21]);
    _serialDebug->println("");
    #endif


    switch(toPackage)
    {
        case jointPackages::package_0:
            _jointPackage0->print("BGN");

            for(int i = 0; i < sizeof(refArray) / sizeof(float); i++) 
            { 
            bytesOut = bytesOut + _jointPackage0->print(refArray[i],6);
            _jointPackage0->print('A');
            }

            _jointPackage0->print("END");
        break;
        
        case jointPackages::package_1:
            _jointPackage1->print("BGN");

            for(int i = 0; i < sizeof(refArray) / sizeof(float); i++) 
            { 
            bytesOut = bytesOut + _jointPackage1->print(refArray[i],6);
            _jointPackage1->print('A');
            }
            
            _jointPackage0->print("END");
        break;

        case jointPackages::PC:
            // _serialDebug->print("BGN");

            for(int i = 0; i < sizeof(refArray) / sizeof(float); i++) 
            { 
            bytesOut = bytesOut + _serialDebug->print(refArray[i],6);
            if(i != ((sizeof(refArray) / sizeof(float)) - 1)) _serialDebug->print(',');
            }
            
            // _serialDebug->print("END");
        break;

        default:
        _serialDebug->println("This should not happen!");
        break;
    }   
    #ifdef IS_DEBUGGING
    _serialDebug->println("Transmitted ");
    _serialDebug->print(bytesOut);
    _serialDebug->println(" bytes.");
    #endif

    return (bytesOut !=0);
}

bool centralController::transmit(String contents, jointPackages toPackage)
{
    float transmitStarted = micros();
    size_t bytesOut = 0;

    switch(toPackage)
    {
        case jointPackages::package_0:
            _jointPackage0->print("BGN");
            //TODO: Implement

            // for(int i = 0; i < sizeof(refArray) / sizeof(float); i++) 
            // { 
            // bytesOut = bytesOut + _jointPackage0->print(refArray[i],6);
            // _jointPackage0->print('A');
            // }

            _jointPackage0->print("END");
        break;
        
        case jointPackages::package_1:
            _jointPackage1->print("BGN");
            //TODO: Implement

            // for(int i = 0; i < sizeof(refArray) / sizeof(float); i++) 
            // { 
            // bytesOut = bytesOut + _jointPackage1->print(refArray[i],6);
            // _jointPackage1->print('A');
            // }
            
            _jointPackage0->print("END");
        break;

        case jointPackages::PC:
            // _serialDebug->print("BGN");

            bytesOut = _serialDebug->println(contents);
            
            // _serialDebug->print("END");
        break;

        default:
        _serialDebug->println("This should not happen!");
        break;
    }   

    #ifdef IS_DEBUGGING
    _serialDebug->println("Transmitted ");
    _serialDebug->print(bytesOut);
    _serialDebug->println(" bytes.");
    _serialDebug->println("It took me ");
    _serialDebug->print(micros() - transmitStarted);
    _serialDebug->println(" microseconds to transmit this packet.");
    #endif

    return (bytesOut !=0);
}



// bool centralController::transmit(packet aPacket)
// {
//     float transmitStarted = micros();
//     size_t bytesOut = 0;
//     double refArray[12];
    
//     for(int i = 0; i<12; i++)
//     {
//         refArray[i] = aPacket.read(i);
//     }

//     _serialDebug->println("refArray:");
//     _serialDebug->println(refArray[0]);
//     _serialDebug->println(refArray[1]);
//     _serialDebug->println(refArray[2]);
//     _serialDebug->println(refArray[3]);
//     _serialDebug->println(refArray[4]);
//     _serialDebug->println(refArray[5]);
//     _serialDebug->println(refArray[6]);
//     _serialDebug->println(refArray[7]);
//     _serialDebug->println(refArray[8]);
//     _serialDebug->println(refArray[9]);
//     _serialDebug->println(refArray[10]);
//     _serialDebug->println(refArray[11]);
//     _serialDebug->println("");

//             _serialDebug->print("BGN");

//             for(int i = 0; i < sizeof(refArray) / sizeof(double); i++) 
//             { 
//             bytesOut = bytesOut + _jointPackage1->print(refArray[i],6);
//             _jointPackage1->print('A');
//             }


//     _serialDebug->println("Transmitted ");
//     _serialDebug->print(bytesOut);
//     _serialDebug->println(" bytes.");
//     return (bytesOut !=0);
// }

void centralController::listen()
{
    #ifdef IS_DEBUGGING
    _serialDebug->println("Started listening...");
    #endif

    float initTime = millis();
    String buffer0;
    String buffer1;

    float readInitA = 0;
    float readInitB = 0;

    float readTimeA = 0;
    float readTimeB = 0;
    
    if(_jointPackage0->find("BGN"))
    {
        #ifdef IS_DEBUGGING
        _serialDebug->println("Found packet on Serial 1!");
        #endif

        readInitA = micros();
        bufferPacketA = readPackage(jointPackages::package_0);
        readTimeA = micros() - readInitA;

        #ifdef IS_DEBUGGING
        _serialDebug->print('\n');
        _serialDebug->print("Time to read Package 0: ");
        _serialDebug->println(readTimeA);
        #endif
    }
    if(_jointPackage1->find("BGN"))
    {
        #ifdef IS_DEBUGGING
        _serialDebug->println("Found packet on Serial 2!");
        #endif 

        readInitB = micros();
        bufferPacketB = readPackage(jointPackages::package_1);
        readTimeB = micros() - readInitB;

        #ifdef IS_DEBUGGING
        _serialDebug->print('\n');
        _serialDebug->print("Time to read Package 1: ");
        _serialDebug->println(readTimeB);
        #endif
    }
}

void centralController::listen(jointPackages package)
{
    #ifdef IS_DEBUGGING
    _serialDebug->println("Started listening...");
    #endif

    float initTime = millis();
    String buffer0;
    String buffer1;

    float readInitA = 0;
    float readInitB = 0;

    float readTimeA = 0;
    float readTimeB = 0;
    
    switch(package)
    {
        case jointPackages::PC:
            if(_serialDebug->available() >= 6)
            {
                readPackageString(jointPackages::PC);
            }
        break;
        default:
        _serialDebug->println("This should not happen!");
        break;
    }
    
    
    if(_jointPackage0->find("BGN"))
    {
        #ifdef IS_DEBUGGING
        _serialDebug->println("Found packet on Serial 1!");
        #endif

        readInitA = micros();
        bufferPacketA = readPackage(jointPackages::package_0);
        readTimeA = micros() - readInitA;

        #ifdef IS_DEBUGGING
        _serialDebug->print('\n');
        _serialDebug->print("Time to read Package 0: ");
        _serialDebug->println(readTimeA);
        #endif
    }
    if(_jointPackage1->find("BGN"))
    {
        #ifdef IS_DEBUGGING
        _serialDebug->println("Found packet on Serial 2!");
        #endif 

        readInitB = micros();
        bufferPacketB = readPackage(jointPackages::package_1);
        readTimeB = micros() - readInitB;

        #ifdef IS_DEBUGGING
        _serialDebug->print('\n');
        _serialDebug->print("Time to read Package 1: ");
        _serialDebug->println(readTimeB);
        #endif
    }
}


void centralController::listenString()
{
    #ifdef IS_DEBUGGING
    _serialDebug->println("Started listening...");
    #endif

    float initTime = millis();
    String buffer0;
    String buffer1;

    float readInitA = 0;
    float readInitB = 0;

    float readTimeA = 0;
    float readTimeB = 0;

    if(_serialDebug->available() >= 6)
    {
        #ifdef IS_DEBUGGING
        //TODO: Add debug code here.
        #endif 

        _serialDebug->readBytes(pcBuffer, 6);
    }
    

    if(_jointPackage0->peek() == 'B')
    {
        #ifdef IS_DEBUGGING
        _serialDebug->println("Found packet on Serial 1!");
        #endif

        readInitA = micros();
        stringBufferA = readPackageString(jointPackages::package_0);
        readTimeA = micros() - readInitA;

        #ifdef IS_DEBUGGING
        _serialDebug->print('\n');
        _serialDebug->print("Time to read Package 0: ");
        _serialDebug->println(readTimeA);
        #endif
    }
    if(_jointPackage1->peek() == 'B')
    {
        #ifdef IS_DEBUGGING
        _serialDebug->println("Found packet on Serial 2!");
        #endif 

        readInitB = micros();
        stringBufferB = readPackageString(jointPackages::package_1);
        readTimeB = micros() - readInitB;

        #ifdef IS_DEBUGGING
        _serialDebug->print('\n');
        _serialDebug->print("Time to read Package 1: ");
        _serialDebug->println(readTimeB);
        #endif
    }

}

packet centralController::returnBufferPacket(jointPackages package)
{
    switch(package)
    {
        case jointPackages::package_0:
            return bufferPacketA;
            break;
        case jointPackages::package_1:
            return bufferPacketB;
            break;
        default:
            _serialDebug->println("This should not happen!");
            break;
    }
}

String centralController::returnBufferString(jointPackages package)
{
    switch(package)
    {
        case jointPackages::package_0:
            return stringBufferA;
            break;
        case jointPackages::package_1:
            return stringBufferB;
            break;
        default:
            _serialDebug->println("This should not happen!");
            break;
    }
}