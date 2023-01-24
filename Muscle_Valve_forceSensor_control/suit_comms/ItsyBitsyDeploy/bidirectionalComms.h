#ifndef __BIDIRECTIONAL_COMMS
#define __BIDIRECTIONAL_COMMS

#include "jointSensorPackage.h"

enum class commsTarget
{
    UC, COMP
};

class bidirectionalComms
{
    private:
        Serial_
    public:
        bidirectionalComms();
        bidirectionalComms(commsTarget receiver)
        bool init();
        void transmit();


};
#endif