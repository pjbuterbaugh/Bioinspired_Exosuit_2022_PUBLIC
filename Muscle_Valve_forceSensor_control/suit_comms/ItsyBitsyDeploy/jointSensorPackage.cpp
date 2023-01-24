#include "jointSensorPackage.h"

void jointSensorPackage::attach(HardwareSerial *serialTransmit, Serial_ *serialDebug)
{
    this->_serialTransmit = serialTransmit;
    this->_serialDebug = serialDebug;
}

//We are not using this function I suppose
float * jointSensorPackage::getData(sensorPacks aSensor)
{

    switch(aSensor){
        case sensorPacks::IMU_0:
            data = sensorData(IMU0.getQuat());
            return data.toDoubleArr();
            
        break;
        case sensorPacks::IMU_1:
            data = sensorData(IMU1.getQuat());
            return data.toDoubleArr();

        break;
        case sensorPacks::LC_0:
            data = sensorData(LC0.read());
            return data.toDoubleArr();
            
        break;
        case sensorPacks::LC_1:
            data = sensorData(LC1.read());
            return data.toDoubleArr();
            
        break;
        case sensorPacks::LC_2:
            data = sensorData(LC2.read());
            return data.toDoubleArr();
            
        break;
        default:
            return (float *) 1;

        break;
    }
    return (float *) 1;
}

void jointSensorPackage::printData()
{

    // !! Currently, the above code does not work. We'll figure it out.
    _serialDebug->println("Attempting to print data...");

    imu::Quaternion quat0 = IMU0.getQuat();

    #ifndef IS_MEGA
    
    imu::Quaternion quat1 = IMU1.getQuat();
    float LCRead0 = LC0.get_value(1);
    float LCRead1 = LC1.get_value(1);
    float LCRead2 = LC2.get_value(1); 
    
    #endif
    
    _serialDebug->println("Quaternions:");
    
    _serialDebug->print("IMU0 - W: ");
    _serialDebug->println(quat0.w());
    _serialDebug->print("IMU0 - X: ");
    _serialDebug->println(quat0.x());
    _serialDebug->print("IMU0 - Y: ");
    _serialDebug->println(quat0.y());
    _serialDebug->print("IMU0 - Z: ");
    _serialDebug->println(quat0.z());

    #ifndef IS_MEGA
    _serialDebug->print("IMU1 - W: ");
    _serialDebug->println(quat1.w());
    _serialDebug->print("IMU1 - X: ");
    _serialDebug->println(quat1.x());
    _serialDebug->print("IMU1 - Y: ");
    _serialDebug->println(quat1.y());
    _serialDebug->print("IMU1 - Z: ");
    _serialDebug->println(quat1.z());
    
    _serialDebug->println("------------------");
    _serialDebug->println("LC Force Average Values:");

    _serialDebug->print("LC0: ");
    _serialDebug->println(LCRead0);
    _serialDebug->print("LC1: ");
    _serialDebug->println(LCRead1);
    _serialDebug->print("LC2: ");
    _serialDebug->println(LCRead2);
    #endif


    
}

packet jointSensorPackage::constructPacket()
{
    // while(!LC0.is_ready() && !LC1.is_ready() && !LC2.is_ready());
    
    
    #ifdef IS_MEGA
    packet newPacket = packet(IMU0.getQuat(), 0); //I have some problem about this zero
    #else
    packet newPacket = packet(IMU0.getQuat(), IMU1.getQuat(), LC0.get_units(1), LC1.get_units(1), LC2.get_units(1));
    #endif

    return newPacket;
}

bool jointSensorPackage::init()
{
    bool failedInitIMU0 = 0,
         failedInitIMU1 = 0,
         failedInitLC0 = 0,
         failedInitLC1 = 0,
         failedInitLC2 = 0;

    #ifdef IS_DEBUGGING 
    _serialDebug->println("Attempting to initialize sensors with the following ports:");
    _serialDebug->print("IMU0 Address: ");
    _serialDebug->print(_IMU0_ADDR);
    _serialDebug->print('\n');
    _serialDebug->print("IMU1 Address: ");
    _serialDebug->print(_IMU1_ADDR);
    _serialDebug->print('\n');
    // Put other debug information here...
    #endif 

    #ifndef IS_MEGA
    IMU0 = Adafruit_BNO055(-1, _IMU0_ADDR);
    IMU1 = Adafruit_BNO055(-1, _IMU1_ADDR);
    
    LC0.begin(_LC0_P1, _LC0_P2);
    LC0.set_scale(LC0_scale);
    LC0.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
    _serialDebug->println("LC0 defined.");

    LC1.begin(_LC1_P1, _LC1_P2);
    LC1.set_scale(LC1_scale);
    LC1.tare();
    _serialDebug->println("LC1 defined.");

    LC2.begin(_LC2_P1, _LC2_P2);
    LC2.set_scale(LC2_scale);
    LC2.tare();
    _serialDebug->println("Defined on ports: ");
    _serialDebug->print(_LC2_P1);
    _serialDebug->print(_LC2_P2);
    _serialDebug->println("LC2 defined.");

    if(!IMU0.begin())
    {
        //Print message; We failed to init IMU0!
        _serialDebug->println("Failed to initialize IMU0!");
        failedInitIMU0 = 1;

    }else
    {
        IMU0.setExtCrystalUse(true);
        _serialDebug->println("IMU0 initialized.");
    }

    if(!IMU1.begin())
    {
        //Print message; We failed to init IMU1!
        _serialDebug->println("Failed to initialize IMU1!");
        failedInitIMU1 = 1;
    }else
    {
        IMU1.setExtCrystalUse(true);
        _serialDebug->println("IMU1 initialized.");

    } 

    if(LC0.is_ready() && LC1.is_ready() && LC2.is_ready())
    {
        _serialDebug->println("Load cell comms confirmed.");
    }else if(!LC0.is_ready())
    {
        _serialDebug->println("LC0 failed comms check.");
        failedInitLC0 = 1;
    }else if(!LC1.is_ready())
    {
        _serialDebug->println("LC1 failed comms check.");
        failedInitLC1 = 1;
    }else if(!LC2.is_ready())
    {
        _serialDebug->println("LC2 failed comms check.");
        failedInitLC2 = 1;
    }

    if(failedInitIMU0 || failedInitIMU1 || failedInitLC0 || failedInitLC1 || failedInitLC2)
    {
        return false;
    }else return true;

    #endif
    IMU0 = Adafruit_BNO055(-1, _IMU0_ADDR);

    if(!IMU0.begin())
    {
        //Print message; We failed to init IMU0!
        _serialDebug->println("Failed to initialize IMU0!");
        failedInitIMU0 = 1;

    }else
    {
        IMU0.setExtCrystalUse(true);
        _serialDebug->println("IMU0 initialized.");
    }

    if(failedInitIMU0)
    {
        return false;
    }else return true;


}

jointSensorPackage::attemptIMUReading(sensorPacks IMU, uint16_t duration)
{
    
}