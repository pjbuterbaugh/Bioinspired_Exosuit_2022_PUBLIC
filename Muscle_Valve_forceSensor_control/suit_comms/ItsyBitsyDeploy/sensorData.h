#include "quaternion.h"

class sensorData {
 private:
  bool type,
       deadSensorCanary = 0; // Indicate if an exception occurred, such as a disconnection.
  float body[4]; // Would it be better to change the visibility of body/type to make things less complex down the line? Not great for encapsulation...

 public:
  sensorData() {}
  sensorData(imu::Quaternion quat) : type(0), body({quat.w(), quat.x(), quat.y(), quat.z()}) {}
  sensorData(long LCReading) : type(1), body({LCReading,0,0,0}) {}
  sensorData(bool isDead) : deadSensorCanary(isDead) {}
  //it is called Double but the data type is actually float
  float * toDoubleArr() // There's almost certainly a better way to do this.
  {
    float convertedBody[5] = {type, this->body[0], this->body[1], this->body[2], this->body[3]};
    return convertedBody;
  }
  bool canaryIsDead()
  {
    return deadSensorCanary;
  }

};