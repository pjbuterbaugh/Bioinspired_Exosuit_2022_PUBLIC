#ifndef __PACKET_H
#define __PACKET_H

#include <Arduino.h>
#include "quaternion.h"

/*!
* @brief Packet structure to store all of the sensor information from a microcontroller.
* 
* We need to store Quaternion data from two IMUs, as well as force data from 3 Load Cells.
* TODO: Create documentation on what each indice means for data transmission.
* TODO: Add two values for pressure sensing / step value.
* TODO: Optimize for speed and bandwidth by decreasing packet size.
*/
class packet {
    private: 
        float _a[11];
    public:
        packet() : _a({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}) {}
        packet(float a[11]) : _a({a[0],
                                a[1],
                                a[2],
                                a[3],
                                a[4],
                                a[5],
                                a[6],   
                                a[7],
                                a[8],
                                a[9],
                                a[10]}) {}
        
        packet(imu::Quaternion quat1, imu::Quaternion quat2, float lc1, float lc2, float lc3) //Inconsistent variable names!
        : _a({(float) quat1.w(), (float) quat1.x(), (float) quat1.y(),
              (float) quat1.z(), (float) quat2.w(), (float) quat2.x(), 
              (float) quat2.y(), (float) quat1.z(), lc1, lc2, lc3,}){}

        float read(int i) { return _a[i];}
        void put(int i, float toPut)
        { 
            if(isDigit(i)) _a[i] = toPut;
        }
        

};


#endif 