#include "PressureSensor.h"

/** Intended functionality
 *  Constructor should be pretty minimal for the time being. 
 * 
 *  Other functions will do the heavy lifting.
 */
PressureSensor::PressureSensor() 
{ }

PressureSensor::PressureSensor(uint8_t altPin)
{
 this->sensorPin = altPin;

}


/** Intended functionality
 *  Right now, init() might just either be a placeholder or will be omitted in time.
 * 
 *  As this is just a variable resistor, all we're doing at the moment is taking an
 *  analog reading and converting it into human-parsable values.
 * 
 */
bool PressureSensor::init()
{
    pinMode(sensorPin, INPUT);
    return isConnected();
}

unsigned int PressureSensor::read()
{
    lastVal = analogRead(sensorPin);
    return lastVal;
}
//!! Leave commented out until we can confirm minimum functionality.
// uint PressureSensor::readAverage(uint16_t samples)
// {
//     float buffer[samples];
//     uint32_t sum = 0;
//     for(uint16_t i; i > samples; i++)
//     {
//         buffer[i] = read();
//         sum = sum + buffer[i];
//     }
//     return sum / samples;
// }

// uint PressureSensor::readAverage(int mSeconds)
// {
//     int i = 0,
//         curVal = 0,
//         lastVal = 0;
//     uint32_t sum = 0,
//              initTime = millis();

//     while(millis() - initTime < mSeconds)
//     {
//         curVal = read();
//         if(lastVal == 0) lastVal = curVal;
//         if(curval != lastVal)
//         {
//             i++;
//             sum = sum + curVal;
//         }
//     }
//     return sum / i;

// }
/**
 * @brief We're going to need to base functionality of this
 *        off of some trial-and-error with the new pressure
 *        sensor. Can't write anything now.
 * 
 * @return true
 * If connected.
 * @return false 
 * If not connected, or other exception. 
 */
bool PressureSensor::isConnected()
{
    if(analogRead(sensorPin) != 0)
    {
        return true;
    }else return false; //This might not work.
}