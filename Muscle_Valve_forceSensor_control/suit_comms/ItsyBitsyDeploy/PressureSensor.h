#ifndef __PRESSURESENSOR_H
#define __PRESSURESENSOR_H

#include <Arduino.h>

class PressureSensor {
    public:
        PressureSensor();
        PressureSensor(uint8_t altPin);
        //!! Deprecated unless we move to software-level processing.
        /** More detailed explanation:
         * We decided to move any threshold processing to hardware, whereas
         * once we fall within a specific voltage threshold, we will send a 
         * pulse to a previously assigned pin.
         * 
         * 
         */

        unsigned int read(),
             readAverage(uint16_t samples),
             readAverage(int mSeconds);
        bool isConnected();
        uint16_t updateFreq();
        bool init(),
             reachedTrigger();
        int sampleCount(int mSeconds);
    private: 
        uint8_t sensorPin = 2; //!! Change me
        int lastVal = 0;
        //!! See above.
        int _minThresh;
        int _maxThresh;
        // float buffer[1000]; // Assuming we want to sample max 1000ms, or a single second; also, can the 32U4 handle this?
        // static float cConstant; // Conversion constant for moving from Analog -> Digital 


};
#endif