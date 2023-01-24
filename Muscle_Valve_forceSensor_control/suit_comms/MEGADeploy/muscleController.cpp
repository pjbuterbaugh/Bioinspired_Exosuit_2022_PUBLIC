#include "muscleController.h"

muscleController::muscleController(int calf_value, int quad_value, int glute_value, int _calf_dur, int _quad_dur, int _glute_dur){
  this->quad  = quad_value;
  this->calf = calf_value;
  this->glute = glute_value;
  this->lc_contract = _calf_dur;
  this->rc_contract = _calf_dur;
  this->lq_contract = _quad_dur;
  this->rq_contract = _quad_dur;
  this->lg_contract = _glute_dur;
  this->rg_contract = _glute_dur;

}

void muscleController::walk(float _T){

  if (_T > 2000){
    _T = 2000;
  }

  if (_T < 500){
    _T = 500;
  }

  cycleStateMachine(_T/202);
  gaitCycle(_T/202);
  
}

void muscleController::cycleStateMachine(float T_update){
  ///////////////////////////////////  LEFT CALF
    switch (left_calf_state){
          
      case 1: //muscle extended
      
        //waits for command to fire
        if(cycle_L >= calf && !lc_fired && !left_calf_fire){
  
          //fire left calf
          lCalf.write(left_calf_fill);
          
         
          
          //set fired signal true so the muscle doesn't fire again for the rest of the cycle
          lc_fired = true;
          left_calf_fire = true;
  
          //mark time for non-blocking delay in next state
          lc_last_state_change = millis();
          
          //set state to next state
          left_calf_state = 2;
        } 
      
      case 2: //MUSCLE_CONTRACTING:
      
        //non blocking delay
        if(millis() - lc_last_state_change >= lc_contract*T_update){
  
          //fill left calf
          lCalf.write(left_calf_release);
          
          //mark time for non-blocking delay in next state
          lc_last_state_change = millis();
          
          //set state to next state
          left_calf_state = 3;
        }
        
      case 3: // MUSCLE_EXTENDING:
      
        //non blocking delay for millis
        if(millis() - lc_last_state_change >= lc_extend){
          
          
          //reset trigger now that complete action has been completed:
          lc_fired = false;
          
          //set state to next state
          left_calf_state = 1;    
        }
    }

///////////////////////////////  LEFT QUAD
    switch (left_quad_state){
          
      case 1: //muscle extended
      
        //waits for command to fire
        if(cycle_L >= quad && !lq_fired && !left_quad_fire){
  
          //fire left quad
          lQuad.write(left_quad_fill);
          
          //set fired signal true so the muscle doesn't fire again for the rest of the cycle
          lq_fired = true;
          left_quad_fire = true;
          //mark time for non-blocking delay in next state
          lq_last_state_change = millis();
          
          //set state to next state
          left_quad_state = 2;
        } 
      
      case 2: //MUSCLE_CONTRACTING:
      
        //non blocking delay
        if(millis() - lq_last_state_change >= lq_contract*T_update){
  
          //contract left quad
          lQuad.write(left_quad_release);
          
          //mark time for non-blocking delay in next state
          lq_last_state_change = millis();
          
          //set state to next state
          left_quad_state = 3;
        }
        
      case 3: // MUSCLE_EXTENDING:
      
        //non blocking delay
        if(millis() - lq_last_state_change >= lq_extend){
          
          
          //reset trigger now that complete action has been completed:
          lq_fired = false;
          
          //set state to next state
          left_quad_state = 1;    
        }
    }


  switch (left_glute_state){
            
        case 1: //muscle extended
        
          //waits for command to fire
          if(cycle_L >= glute && !lg_fired && !left_glute_fire){
    
            lGlute.write(left_glute_fill);
    
            //set fired signal true so the muscle doesn't fire again for the rest of the cycle
            lg_fired = true;
            left_glute_fire = true;
    
            //mark time for non-blocking delay in next state
            lg_last_state_change = millis();
            
            //set state to next state
            left_glute_state = 2;
          } 
        
        case 2: //MUSCLE_CONTRACTING:
        
          //non blocking delay for 350 millis
          if(millis() - lg_last_state_change >= lg_contract*T_update){
    
            //fill left glute
            lGlute.write(left_glute_release);
            
            //mark time for non-blocking delay in next state
            lg_last_state_change = millis();
            
            //set state to next state
            left_glute_state = 3;
          }
          
        case 3: // MUSCLE_EXTENDING:
        
          //non blocking delay for 50 millis
          if(millis() - lg_last_state_change >= lg_extend){
            
            
            //reset trigger now that complete action has been completed:
            lg_fired = false;
            
            //set state to next state
            left_glute_state = 1;    
          }
      }
  
  ///////////////////////////////////  LEFT CALF
      switch (right_calf_state){
            
        case 1: //muscle extended
        
          //waits for command to fire
          if(cycle_R >= calf && !rc_fired && !right_calf_fire){
    
            //fire left calf
            rCalf.write(right_calf_fill);
            
           //set fired signal true so the muscle doesn't fire again for the rest of the cycle
            rc_fired = true;
            right_calf_fire = true;
    
            //mark time for non-blocking delay in next state
            rc_last_state_change = millis();
            
            //set state to next state
            right_calf_state = 2;
          } 
        
        case 2: //MUSCLE_CONTRACTING:
        
          //non blocking delay for 350 millis
          if(millis() - rc_last_state_change >= rc_contract*T_update){
    
            //fill left calf
            rCalf.write(right_calf_release);
            
            //mark time for non-blocking delay in next state
            rc_last_state_change = millis();
            
            //set state to next state
            right_calf_state = 3;
          }
          
        case 3: // MUSCLE_EXTENDING:
        
          //non blocking delay for 50 millis
          if(millis() - rc_last_state_change >= rc_extend){
            
            
            //reset trigger now that complete action has been completed:
            rc_fired = false;
            
            //set state to next state
            right_calf_state = 1;    
          }
      }
  
  ///////////////////////////////  LEFT QUAD
      switch (right_quad_state){
            
        case 1: //muscle extended
        
          //waits for command to fire
          if(cycle_R >= quad && !rq_fired && !right_quad_fire){
    
            //fire left quad
            rQuad.write(right_quad_fill);
            
            //set fired signal true so the muscle doesn't fire again for the rest of the cycle
            rq_fired = true;
            right_quad_fire = true;
            //mark time for non-blocking delay in next state
            rq_last_state_change = millis();
            
            //set state to next state
            right_quad_state = 2;
          } 
        
        case 2: //MUSCLE_CONTRACTING:
        
          //non blocking delay for 350 millis
          if(millis() - rq_last_state_change >= rq_contract*T_update){
    
            
            rQuad.write(right_quad_release);
            
            //mark time for non-blocking delay in next state
            rq_last_state_change = millis();
            
            //set state to next state
            right_quad_state = 3;
          }
          
        case 3: // MUSCLE_EXTENDING:
        
          //non blocking delay 
          if(millis() - rq_last_state_change >= rq_extend){
            
            
            //reset trigger now that complete action has been completed:
            rq_fired = false;
            
            //set state to next state
            right_quad_state = 1;    
          }
      }
  
  
  switch (right_glute_state){
            
        case 1: //muscle extended
        
          //waits for command to fire
          if(cycle_R >= glute && !rg_fired && !right_glute_fire){
    
            rGlute.write(right_glute_fill);
            
           
            
            //set fired signal true so the muscle doesn't fire again for the rest of the cycle
            rg_fired = true;
            right_glute_fire = true;
    
            //mark time for non-blocking delay in next state
            rg_last_state_change = millis();
            
            //set state to next state
            right_glute_state = 2;
          } 
        
        case 2: //MUSCLE_CONTRACTING:
        
          //non blocking delay for 350 millis
          if(millis() - rg_last_state_change >= rg_contract*T_update){
    
            
            rGlute.write(right_glute_release);
            
            //mark time for non-blocking delay in next state
            rg_last_state_change = millis();
            
            //set state to next state
            right_glute_state = 3;
          }
          
        case 3: // MUSCLE_EXTENDING:
        
          //non blocking delay for 50 millis
          if(millis() - rg_last_state_change >= rg_extend){
            
            
            //reset trigger now that complete action has been completed:
            rg_fired = false;
            
            //set state to next state
            right_glute_state = 1;    
          }
      }
}

void muscleController::updateAction(int calf_update, int quad_update, int glute_update, int calf_dur, int quad_dur, int glute_dur){
  this->quad = quad_update;
  this->calf = calf_update;
  this->glute = glute_update;
  this->lc_contract = calf_dur;
  this->rc_contract = calf_dur;
  this->lq_contract = quad_dur;
  this->rq_contract = quad_dur;
  this->lg_contract = glute_dur;
  this->rg_contract = glute_dur;
}

void muscleController::gaitCycle(float gpd){
  
  if(millis() - last_cycle >= gpd){
      
        last_cycle = millis();
        cycle_L++;
        cycle_R++;
        count++;
        
        //Overflow of gait percentage handling
        if (cycle_L > 100){
          
          //reset left cycle
          cycle_L = 0;
          
          //reset muscle firing variables 
          left_calf_fire = false;
          left_quad_fire = false;
          left_glute_fire = false;
          
          
        }
        if (cycle_R > 100){
          //reset right cycle
          cycle_R = 0;
          //reset muscle firing variables
          right_calf_fire = false;
          right_quad_fire = false;
          right_glute_fire = false;
        }
   }
}



bool muscleController::initialize(){
  //attach corresponding pins to servo objects
  lQuad.attach(lQuadPin);
  lGlute.attach(lGlutePin);
  lCalf.attach(lCalfPin);
  rQuad.attach(rQuadPin);
  rGlute.attach(rGlutePin);
  rCalf.attach(rCalfPin);

  //Set servos to neutral position for start
  lQuad.write(left_quad_neutral);
  lGlute.write(left_glute_neutral);
  lCalf.write(left_calf_neutral);
  rQuad.write(right_quad_neutral);
  rGlute.write(right_glute_neutral);
  rCalf.write(right_calf_neutral);
}
