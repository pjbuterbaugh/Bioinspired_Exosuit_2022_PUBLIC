#ifndef __ROBOTMAP_H
#define __ROBOTMAP_H
/** 
 * Suit Map
 * 
 * Master file for specifying the value for each connection, component,
 * or otherwise significant variable representing IRL components
 * on the exo-suit. (Will) Connect to the rest of the project.
 * 
 */


// 
class RobotMap
{
    private: 
    public:
        // Serial
        /**
         * Serial0 (Serial)  --> Computer
         * Serial1 (Serial1) --> ItsyBitsy0 (unless debugging)
         * Serial2 (Serial2) --> ItsyBitsy1 (unless debugging)
         */


        // --- I2C ----
        static byte IMU0_ADDR = 0x28;
        static byte IMU1_ADDR = 0x29;

        // --- DIO ----
        // MV == Muscle Valve (Servo)
        // GP == Gait Processor
        static uint8_t MV0_PWM = 0,
                       MV1_PWM = 0,
                       MV2_PWM = 0,
                       MV3_PWM = 0,
                       MV5_PWM = 0,
                       MV6_PWM = 0,
                       GP_PULSEOUT = 0,
                       GP_PULSEIN = 0;

        // -- Analog --
        // FVR == Foot Variable Resistor / Foot switch
        static uint8_t FVR0_IN = 0,
                       FVR0_IN = 0; 
    protected:
};
#endif