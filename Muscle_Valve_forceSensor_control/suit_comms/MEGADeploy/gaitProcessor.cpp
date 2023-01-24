#include "gaitProcessor.h"


gaitProcessor::gaitProcessor(PressureSensor *footL, PressureSensor *footR)
{
    this->_footL = footL;
    this->_footR = footR;
}

gaitProcessor::gaitProcessor(PressureSensor *footL, PressureSensor *footR, HardwareSerial *serialDebug)
{
    this->_footL = footL;
    this->_footR = footR;
    this->_serialDebug = serialDebug;
}

bool gaitProcessor::init()
{
  bool _footLWasInit = false,
       _footRWasInit = false;
    
    if(_footL->init())
    {
      _footLWasInit = true;
      // _serialDebug->println("Successfully initialized left foot pressure sensor.");
      
      if(_footR->init())
      {
        _footRWasInit = true;
        // _serialDebug->println("Successfully initialized right foot pressure sensor.");
      }else;
        // _serialDebug->println("Error initializing right foot pressure sensor.");
    }else;
      // _serialDebug->println("Error initializing left foot pressure sensor.");
    
    
    if(_footLWasInit && _footRWasInit)
    {
        // _serialDebug->println("Pressure sensors ready.");
        return true;
    }else return false;
    
}

void gaitProcessor::setTriggerThresh(int minThresh, int maxThresh)
{
    this->_minThresh = minThresh;
    this->_maxThresh = maxThresh;
}


/**
 * @b <runProcessor>
 * 
 * @brief A non-blocking function enabling the gait processor to run 
 *        new instructions and calculations as frequently as it is
 *        called. Should be put in @code <void loop()>.
 */
void gaitProcessor::runProcessor()
{
    fsrLeftReading = _footL->read();
    fsrRightReading = _footR->read();

    fsrLeftVoltage = map(fsrLeftReading, 0, 1023, 0, 5000);
    fsrRightVoltage = map(fsrRightReading, 0, 1023, 0, 5000);

    // _serialDebug->println(fsrLeftVoltage);
    // _serialDebug->println(fsrRightVoltage);

    if(fsrLeftVoltage <= _maxThresh){
    last_left_tigure_state = left_tigure_state;
    left_tigure_state = LOW;
  }else{
    last_left_tigure_state = left_tigure_state;
    left_tigure_state = HIGH;
  }
  if(fsrRightVoltage <= _maxThresh){
    last_right_tigure_state = right_tigure_state;
    right_tigure_state = LOW;
  }else{
    last_right_tigure_state = right_tigure_state;
    right_tigure_state = HIGH;
  }

  // _serialDebug->println(left_tigure_state);
  // _serialDebug->println(right_tigure_state);

  if(((left_tigure_state == HIGH) && (last_left_tigure_state == LOW)) || ((right_tigure_state == HIGH) && (last_right_tigure_state == LOW))){
    if((left_tigure_state == HIGH) && (last_left_tigure_state == LOW)){
      curr_T = millis() - last_step_L_T;
      last_step_L_T = millis();
    }else{
      curr_T = millis() - last_step_R_T;
      last_step_R_T = millis();
    }
    if((curr_T >= 155) && (curr_T <= 5000)){ //fastest walking speed is 0.1441857426 sec, and we want to assume we are not slower than 5 sec
      T_array[T_array_index] = curr_T;
      if(T_array_index==(T_array_size-1)){
       T_array_index = 0;
      }else{
       T_array_index++;
      }
      //the code that get the average T (time taken) for a gait cycle
      T = 0.0;
      for (int i = 0; i < T_array_size; i++) {
       T = T + (float)T_array[i];
      }
      T = T/(float)T_array_size;
      //Serial.println(T);
    }
  }
}

void gaitProcessor::setSerialPort(HardwareSerial * serialDebug)
{
  this->_serialDebug = serialDebug;
}