/*******************************************************************************
  SD4RotaryEncoder for KY-040
  https://github.com/sd4solutions/rotary-encoder-ky40/

  This code is provided by SD4Solutions di Domenico Spina

     Email me: info@sd4solutions.com
     https://github.com/sd4solutions
     Please leave my logo and my copyright then use as you want


  Encoder KY-040 has 3 PIN: CLK, DT, SW.

  It works by connecting the CLK pin of the encoder to external interrupt pin of arduino
  and others to digital input pins.

  Use:
  
    #include "SD4RotaryEncoder.h"
    //NB: clkpin must be external interrupt pin
    SD4RotaryEncoder encoder(clkpin,dtpin,swpin);


    void setup(){
        ... your code...
    }

    void loop(){

        encoder.idle(); //wait for read changes
        long value = encoder.getValue();
        short clk = encoder.getClk();
        short dt = encoder.getDt();
        short swc = encoder.getSwc();
        short dir = encoder.getDirection(); // -1 (counterclockwise) 0(initial value) +1 (clockwise)
        float freq = encoder.getFrequency(); //rotating frequency in Hz
        bool isValueChanged  = encoder.isValueChanged();
    
        if(swc)
        {
          encoder.resetValue(); 
        }
    
        ...
        
    }
    
          
 ******************************************************************************/

#include "Arduino.h"
#include "SD4RotaryEncoder.h"

//some init
static short SD4RotaryEncoder::_pinClk = -1;
static short SD4RotaryEncoder::_pinDt = -1;
static short SD4RotaryEncoder::_pinSwc = -1;
static short SD4RotaryEncoder::_clk = 0;
static short SD4RotaryEncoder::_dt = 0;
static short SD4RotaryEncoder::_swc = 0;
static short SD4RotaryEncoder::_direction = 0; //can be: -1 (counterclockwise) 0(initial value) +1 (clockwise)
static long  SD4RotaryEncoder::_value = 0;
static long  SD4RotaryEncoder::_oldvalue = 0;
static float SD4RotaryEncoder::_frequency = 0;
static float SD4RotaryEncoder::_millis = 0;
static bool  SD4RotaryEncoder::_trigger = false;

//contruct
//clk pin must be external interrupt, otherwise not works
SD4RotaryEncoder::SD4RotaryEncoder(short clk, short dt, short swc)
{
    //pin assignment
     SD4RotaryEncoder::_pinClk = clk;
     SD4RotaryEncoder::_pinDt = dt;
     SD4RotaryEncoder::_pinSwc = swc;   

    pinMode(SD4RotaryEncoder::_pinClk, INPUT_PULLUP);
    pinMode(SD4RotaryEncoder::_pinDt, INPUT_PULLUP);
    pinMode(SD4RotaryEncoder::_pinSwc, INPUT_PULLUP);
  
    attachInterrupt(digitalPinToInterrupt(SD4RotaryEncoder::_pinClk),&SD4RotaryEncoder::avrCallback,CHANGE);
};

short SD4RotaryEncoder::getClk(){
    return SD4RotaryEncoder::_clk;
};

short SD4RotaryEncoder::getDt(){
    return SD4RotaryEncoder::_dt;
};

short SD4RotaryEncoder::getSwc(){
    return !SD4RotaryEncoder::_swc;
};

short SD4RotaryEncoder::getDirection(){
   return SD4RotaryEncoder::_direction;
}
     
long SD4RotaryEncoder::getValue(){

     return SD4RotaryEncoder::_value;
};

float SD4RotaryEncoder::getFrequency(){
    return SD4RotaryEncoder::_frequency;
}

void SD4RotaryEncoder::avrCallback(){
    /*
     * We have 5 step per direction
     * 
       n  ck dt
       1   1  1  > idle
       2   0  1  > rotate clockwise
       3   0  0  > indeterminate
       4   1  0  > rotate counterclockwise 
       5   1  1  > idle

      Rotate SX
       n  ck dt
       1   1  1  > idle
       2   1  0  > change sign
       3   0  0  > indeterminate
       4   0  1  > change sign
       5   1  1  > idle          
     */
     
    volatile short clk = SD4RotaryEncoder::_clk = digitalRead(SD4RotaryEncoder::_pinClk);
    volatile short dt = SD4RotaryEncoder::_dt = digitalRead(SD4RotaryEncoder::_pinDt);
    volatile short step = (clk<<1) | dt;
    volatile float f = 0;

    if(!SD4RotaryEncoder::_millis) SD4RotaryEncoder::_millis = micros();
    
    if(step==2) SD4RotaryEncoder::_direction=1;
    if(step==3) SD4RotaryEncoder::_direction=-1;
    
    if(step==2 || step==3){
        SD4RotaryEncoder::_value+=(1*SD4RotaryEncoder::_direction);
        f = (micros()-SD4RotaryEncoder::_millis);
        if(f>500)
        {
           SD4RotaryEncoder::_frequency = 1/(f/1000000); //hz
        }
        
        SD4RotaryEncoder::_millis = 0;
    }   
}

void SD4RotaryEncoder::idle(){
    _swc = digitalRead(_pinSwc);   
    

    if(SD4RotaryEncoder::_oldvalue !=SD4RotaryEncoder::_value)
    {
        SD4RotaryEncoder::_trigger = true;
        SD4RotaryEncoder::_oldvalue = SD4RotaryEncoder::_value;
    }
    else
    {
      //  SD4RotaryEncoder::_frequency = 0;
        SD4RotaryEncoder::_trigger = false;
    }
}

void SD4RotaryEncoder::resetValue(){
   SD4RotaryEncoder::_value = SD4RotaryEncoder::_direction = 0;
}

bool SD4RotaryEncoder::isValueChanged(){
    return SD4RotaryEncoder::_trigger;
}
