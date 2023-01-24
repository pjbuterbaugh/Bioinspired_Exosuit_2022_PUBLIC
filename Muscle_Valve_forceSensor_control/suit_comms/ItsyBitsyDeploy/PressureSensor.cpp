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
 this->sensorPin = &altPin;

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
    return false;
}