#include <arduino.h>
//Servo library
#include <Servo.h>  

#ifndef MUSC_H
#define MUSC_H


class muscleController{
  public:
    
    //muscle pins
    const int lQuadPin = 13;
    const int lGlutePin = 10;
    const int lCalfPin = 12;
    const int rQuadPin = 6;
    const int rGlutePin = 9;
    const int rCalfPin = 8;
    
    
    //neutral servo angles
    const int left_calf_neutral = 80;  //servo 3: Neutral:100
    const int left_quad_neutral = 75;   //servo 2: Neutral: 90
    const int left_glute_neutral = 40;  //servo 1: neautrl: 80  
    const int right_calf_neutral = 55;  //servo 4: n: 95
    const int right_quad_neutral = 65;  //servo 5: 90
    const int right_glute_neutral = 77;  //servo 6: 99
    
    //fill servo angles
    const int left_calf_fill = 60;
    const int left_quad_fill = 60;
    const int left_glute_fill = 40;
    const int right_calf_fill = 55;
    const int right_quad_fill = 65;
    const int right_glute_fill = 77;
    
    //release servo angles
    const int left_calf_release = 120;
    const int left_quad_release = 115;
    const int left_glute_release = 110;
    const int right_calf_release = 125;
    const int right_quad_release = 115;
    const int right_glute_release = 125;
    
    
    //initial states for 
    int left_calf_state = 1;
    int right_calf_state = 1;
    int left_quad_state = 1;
    
    int right_quad_state = 1;
    int left_glute_state = 1;
    int right_glute_state = 1;
    
    //Gait percentage triggers
    int calf = 40;
    int glute = 92;
    int quad = 50;
    
    
    
    //trigger markers
    bool left_calf_fire = false;
    bool right_calf_fire = false;
    bool left_quad_fire = false;
    bool right_quad_fire = false;
    bool left_glute_fire = false;
    bool right_glute_fire = false;
    
    //fired markers, these are used so that a >= comparison can be implemented for firing timings to reduce potential bugs if an iteration is somehow skipped
    bool lc_fired = false;
    bool rc_fired = false;
    bool lq_fired = false;
    bool rq_fired = false;
    bool lg_fired = false;
    bool rg_fired = false;
    
    //contraction duration multipliers
    int lc_contract = 50;
    int rc_contract = 50;
    int lq_contract = 50;
    int rq_contract = 50;
    int lg_contract = 50;
    int rg_contract = 50;
    
    //extension durations
    int lc_extend = 65;
    int rc_extend = 65;
    int lq_extend = 65;
    int rq_extend = 65;
    int lg_extend = 65;
    int rg_extend = 65;
    
    //gait cycle counters
    int cycle_L = 0;
    int cycle_R = 50;
    int count = 0;
    
    //last state change variables to store timestamps of last state change
    unsigned long lc_last_state_change = 0;
    unsigned long rc_last_state_change = 0;
    unsigned long lq_last_state_change = 0;
    unsigned long rq_last_state_change = 0;
    unsigned long lg_last_state_change = 0;
    unsigned long rg_last_state_change = 0;
    
    //G% iteration reference
    unsigned long last_cycle = 0;
    
    //T stuff just tests for now
    int T = 20;
    long gpd = 20; //gait perecent duration
      
    
    //muscle servos
    Servo lQuad;
    Servo lGlute;
    Servo lCalf;
    Servo rQuad;
    Servo rGlute;
    Servo rCalf;
     
    /////
    muscleController(int calf_value, int quad_value, int glute_value, int _calf_dur, int _quad_dur, int _glute_dur);
    void walk(float _T);
    void cycleStateMachine(float T_update);
    void updateAction(int calf_update, int quad_update, int glute_update, int calf_dur, int quad_dur, int glute_dur);
    bool initialize();
    void gaitCycle(float gpd);
};


#endif
