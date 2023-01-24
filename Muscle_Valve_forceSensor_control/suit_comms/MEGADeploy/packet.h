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
        float _a[12];
        unsigned int _b[6];
    public:
        packet() : _a({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}) {}
        packet(unsigned int b[6]) : _b({b[0],
                                        b[1],
                                        b[2],
                                        b[3],
                                        b[4],
                                        b[5],}) {} 
        packet(float a[12]) : _a({a[0],
                                a[1],
                                a[2],
                                a[3],
                                a[4],
                                a[5],
                                a[6],   
                                a[7],
                                a[8],
                                a[9],
                                a[10],
                                a[11]}) {}

        
        
        packet(imu::Quaternion quat1, imu::Quaternion quat2, float lc1, float lc2, float lc3, float time) //Inconsistent variable names!
        : _a({(float) quat1.w(), (float) quat1.x(), (float) quat1.y(),
              (float) quat1.z(), (float) quat2.w(), (float) quat2.x(), 
              (float) quat2.y(), (float) quat1.z(), lc1, lc2, lc3, time}){}

        float read(int i) 
        { 
            return _a[i];
        }
        
        unsigned int readInt(int i) { 
            return _b[i];
        }
        
        void put(int i, float toPut)
        { 
            if(isDigit(i)) _a[i] = toPut;
        }
        

};


#endif 