/**
 * Test MENU DISPLAY
 *  
 *  display SSD1306 I2C 4 PIN
 *    SDA: A4
 *    SCL: A5
 *  encoder rotativo KY-040 con pulsante
 *  
 *  Libreria per gestione menu: TCMENU
 */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET -1 //il pin reset: -1 per reset di arduino o altro pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

//encoder
#include "SD4RotaryEncoder.h"
SD4RotaryEncoder encoder(2,3,4);

void setup(){

  Serial.begin(57600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    return;
  }

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Display init");
  display.display();
  
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
    
    display.setCursor(0, 0);
    display.print("Value is : ");
    display.println(value);
    display.print("\nDirection is: ");
    display.println(dir);
    display.print("\nTriggger change: ");
    display.println(isValueChanged);
    display.print("\nFreq: ");
    display.println(freq);
    display.display();
    
    display.clearDisplay();

}
