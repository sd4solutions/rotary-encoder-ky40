/**
 * Test simple with serial
 *    
 *  Rotary encoder KY-040
 *    CLK: 2
 *    DT:  3
 *    SWC: 4
 */
 
//encoder
#include "SD4RotaryEncoder.h"
SD4RotaryEncoder encoder(2,3,4);

void setup(){

  Serial.begin(9600);
  
}

void loop(){

    encoder.idle(); //wait for read changes
    
    long value = encoder.getValue();
    short swc = encoder.getSwc();
    short dir = encoder.getDirection(); // -1 (counterclockwise) 0(initial value) +1 (clockwise)
    float freq = encoder.getFrequency(); //rotating frequency in Hz
    bool isValueChanged  = encoder.isValueChanged();

    //when switch occour we reset value
    if(swc)
    {
      encoder.resetValue(); 
    }

    if(isValueChanged)
    {
        Serial.println();
        Serial.println("-- VALUES --");
        Serial.print("Value is : ");
        Serial.println(value);
        Serial.print("Direction is: ");
        Serial.println(dir);
        Serial.print("Triggger change: ");
        Serial.println(isValueChanged);
        Serial.print("Freq: ");
        Serial.println(freq);
        Serial.println("-- END VALUES --");
        Serial.println();
    }

}
