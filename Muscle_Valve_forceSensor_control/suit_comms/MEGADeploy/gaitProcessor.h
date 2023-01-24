#include "PressureSensor.h"
#include "HardwareSerial.h"
#include <USBAPI.h>
#include <arduino.h>
#include "ComDriverSpi.h"

#ifndef __GAITPROCESSOR_H
#define __GAITPROCESSOR_H

/** Intended functionality
 * gaitProcessor should take in pressure sensor instances representing
 * the left and right feet, and proceed to determine step timing based
 * on PressureSensor::reachedTrigger(). Perhaps we could use this in 
 * tandem with the IMUs to perform some gait processing suitside?
 * 
 * So, an interrupt with our CS topology will likely go something like this:
 * 
 * ItsyBitsy polls and processes sensor data to send to MEGA, if a trigger is detected, send that over DIO...?
 * We need to draw this one out.
 */

// enum class BOARDSTATUS
// {
//     RECEIVER, TRANSMITTER
// };

class gaitProcessor {
    private:
        uint8_t _pulsePin = 0;
        
        PressureSensor * _footL;
        PressureSensor * _footR;
        
        HardwareSerial * _serialDebug;
        
        int _minThresh = 0,
            _maxThresh = 1600,
            fsrLeftReading = 0,
            fsrRightReading = 0,
            fsrLeftVoltage = 0,
            fsrRightVoltage = 0;

        static const int T_array_size = 5;

        unsigned int last_left_tigure_state,
            last_right_tigure_state,
            left_tigure_state,
            right_tigure_state,
            curr_T = 0,
            T_array[T_array_size],
            T_array_index;

        unsigned long last_step_L_T = 0,
                      last_step_R_T = 0;
        
        float T;
    public:
        gaitProcessor(PressureSensor *footL, PressureSensor *footR); //Likely pass by reference or point
        gaitProcessor(PressureSensor *footL, PressureSensor *footR, HardwareSerial *serialDebug);
        void setSerialPort(HardwareSerial *serialDebug); //this doesn't work with non primitive datatypes! (right?)
        // void sendPulse();
        void setInterrupts(bool enable);
        void setTriggerThresh(int minThresh, int maxThresh);
        bool init();
        void runProcessor();
        float returnT() 
        { 
            return T; 
        }

    
};
#endif 
