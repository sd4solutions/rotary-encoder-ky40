# rotary-encoder-ky40
A simple lib to drive rotary encoder ky40

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


