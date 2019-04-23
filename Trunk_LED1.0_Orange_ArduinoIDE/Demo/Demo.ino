/*
  Trunk LED Strip

  Custom software for chineese automotive trunk light strips using an Arduino, compatable, or my Tailgate Light Bar controller.
  These chineese "Trunk LED Strips" you can find on eBay most are simple adressable LEDs (WS2812B) wired in sets of 3 to handel 12v see 
  included documentation for more info. This software is a custom program to give a better look, annimations, features like 
  modes for brakes & Turn Signal, running oter lights. And bringing endless possibilities now with an OpenSource controler & 
  software. Please see the included documentation for more. 
  
  DEMO PROGRAM 


  The circuit:
  - (see schematic) 

  created Jan 3 2018
  by Brandon Sheaffer
  http://www.dattechweb.com/trunk_led_strip

  Copyright 2019 DatMedia Co.

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation 
  files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, 
  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the 
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE 
  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <FastLED.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN     12
#define NUM_LEDS    24
#define LED_TYPE    WS2812B
#define COLOR_ORDER GBR
#define BRIGHTNESS  100
#define HTIME 50


CRGB leds[NUM_LEDS];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    delay(1000);
    LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
//  LEDS.addLeds<LED_TYPE, LED_PIN, CLOCK_PIN COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop(){
  rightSlide();
  leftSlide();
  rightBlink();
  leftBlink();
  runing();
  brake();
  brakeRight();
  brakeLeft();
  reverse();
  reverseRight();
  reverseLeft();
  reverseBrake();
  reverseBrakeRight();
  reverseBrakeLeft();
  hazerd();
  rainbow(20);
  
}

void rightSlide(){    //---------------------------------Right Slide---------------------------------------------------------------------------------------------------------------
  FastLED.clear();
  for(int A=2; A>=1; A--) {
    leds[11] = CRGB(35,0,255); 
    leds[12] = CRGB(35,0,255);
      
      for(int i=14;i<24;i++){
      leds[i].setRGB(35,0,255);
      FastLED.show();
      delay(50);
    }
    
    leds[11] = CRGB(35,0,255);  
    leds[12] = CRGB(35,0,255);
       for(int i=14;i<NUM_LEDS;i++){
         leds[i].setRGB(0,0,0);
         FastLED.show();
         delay(50);
     }
  }

}


void leftSlide(){   //-------------------------------------Left Slide--------------------------------------------------------------------------------------------------------------
  FastLED.clear();  
  delay(250);
  for(int A=2; A>=1; A--) {
    leds[11] = CRGB(35,0,255); 
    leds[12] = CRGB(35,0,255);; 
      
      for(int i=9;i>-1;i--){
      leds[i].setRGB(35,0,255);
      FastLED.show();
      delay(50);
    }
    
    leds[11] = CRGB(35,0,255); 
    leds[12] = CRGB(35,0,255);  
       for(int i=9;i>-1;i--){
         leds[i].setRGB(0,0,0);
         FastLED.show();
         delay(50);
     }
  }
}

void rightBlink(){     //---------------------------------Right Blink--------------------------------------------------------------------------------------------------------------
  FastLED.clear();
  for(int A=2; A>=1; A--) {
      for(int i=14;i<24;i++){
      leds[i].setRGB(35,0,255);
      FastLED.show();
      
    }
    delay(500);
      
       for(int i=14;i<NUM_LEDS;i++){
         leds[i].setRGB(0,0,0);
         FastLED.show();
         
     }
   delay(500);
  }
}
  
void leftBlink(){        //---------------------------------Left Blink-------------------------------------------------------------------------------------------------------------
  FastLED.clear();
  for(int A=2; A>=1; A--) {
    for(int i=9;i>-1;i--){
      leds[i].setRGB(35,0,255);
      FastLED.show();
    }
    delay(500);
     
    for(int i=9;i>-1;i--){
      leds[i].setRGB(0,0,0);
      FastLED.show();
     }
     delay(500);
  }
}

void runing(){        //-------------------------------------Running---------------------------------------------------------------------------------------------------------------
  FastLED.clear();
  fill_solid(leds, NUM_LEDS, CRGB(0,0,50));
  FastLED.show();
  delay(2000);
}

void brake(){        //---------------------------------------Brake----------------------------------------------------------------------------------------------------------------
  FastLED.clear();
  fill_solid(leds, NUM_LEDS, CRGB(0,0,255));
  FastLED.show();
  delay(2000);
}

void brakeRight(){   //---------------------------------------Brake Right----------------------------------------------------------------------------------------------------------
  FastLED.clear();
  fill_solid(leds, NUM_LEDS, CRGB(0,0,255));
  FastLED.show();
  
  for(int A=2; A>=1; A--) {
    for(int i=18;i<24;i++){
      leds[i].setRGB(35,0,255);
      FastLED.show();  
    }
    delay(500);
       
    for(int i=18;i<24;i++){
      leds[i].setRGB(0,0,255);
      FastLED.show();  
    }
      delay(500);
  }
  FastLED.show();
}

void brakeLeft(){    //---------------------------------------Brake Left-----------------------------------------------------------------------------------------------------------
  FastLED.clear();
  fill_solid(leds, NUM_LEDS, CRGB(0,0,255));
  FastLED.show();
  
  for(int A=2; A>=1; A--) {
    for(int i=0;i<6;i++){
      leds[i].setRGB(35,0,255);
      FastLED.show();  
    }
    delay(500);
       
    for(int i=0;i<6;i++){
      leds[i].setRGB(0,0,255);
      FastLED.show();  
    }
    delay(500);
  }
  FastLED.show(); 
}

void reverse(){       //------------------------------------Reverse----------------------------------------------------------------------------------------------------------------
  FastLED.clear();
  fill_solid(leds, NUM_LEDS, CRGB(255,255,255));
  FastLED.show();
  delay(2000);
}
void reverseBrake(){  //------------------------------Reverse Brake----------------------------------------------------------------------------------------------------------------
  FastLED.clear();
  leds[0] = CRGB(255,255,255);
  leds[1] = CRGB(255,255,255);
  leds[2] = CRGB(255,255,255);
  leds[3] = CRGB(255,255,255);
  leds[4] = CRGB(255,255,255);
  leds[5] = CRGB(255,255,255);
  leds[6] = CRGB(255,255,255);
  leds[7] = CRGB(255,255,255);
  leds[8] = CRGB(0,0,255);
  leds[9] = CRGB(0,0,255);
  leds[10] = CRGB(0,0,255);
  leds[11] = CRGB(0,0,255);
  leds[12] = CRGB(0,0,255);
  leds[13] = CRGB(0,0,255);
  leds[14] = CRGB(0,0,255);
  leds[15] = CRGB(0,0,255);
  leds[16] = CRGB(255,255,255);
  leds[17] = CRGB(255,255,255);
  leds[18] = CRGB(255,255,255);
  leds[19] = CRGB(255,255,255);
  leds[20] = CRGB(255,255,255);
  leds[21] = CRGB(255,255,255);
  leds[22] = CRGB(255,255,255);
  leds[23] = CRGB(255,255,255);
  FastLED.show();
  delay(3000);
}

void reverseRight(){  //-----------------------------------Reverse Right-----------------------------------------------------------------------------------------------------------
  FastLED.clear();
  fill_solid(leds, NUM_LEDS, CRGB(255,255,255));
  FastLED.show();
  
  for(int A=2; A>=1; A--) {
    for(int i=18;i<24;i++){
      leds[i].setRGB(35,0,255);
      FastLED.show();  
    }
    delay(500);
       
       for(int i=18;i<24;i++){
         leds[i].setRGB(255,255,255);
         FastLED.show();  
     }
     delay(500);
  }
  FastLED.show();
}


 void reverseLeft(){  //-------------------------------------Reverse Left----------------------------------------------------------------------------------------------------------
  FastLED.clear();
fill_solid(leds, NUM_LEDS, CRGB(255,255,255));
  FastLED.show();
  
  for(int A=2; A>=1; A--) {
    for(int i=0;i<6;i++){
      leds[i].setRGB(35,0,255);
      FastLED.show();  
    }
    delay(500);
       
       for(int i=0;i<6;i++){
         leds[i].setRGB(255,255,255);
         FastLED.show();  
     }
     delay(500);
  }
  FastLED.show();
}

void reverseBrakeRight(){  //------------------------------Reverse Brake Right-----------------------------------------------------------------------------------------------------
  FastLED.clear();
  leds[0] = CRGB(255,255,255);
  leds[1] = CRGB(255,255,255);
  leds[2] = CRGB(255,255,255);
  leds[3] = CRGB(255,255,255);
  leds[4] = CRGB(255,255,255);
  leds[5] = CRGB(255,255,255);
  leds[6] = CRGB(255,255,255);
  leds[7] = CRGB(255,255,255);
  leds[8] = CRGB(0,0,255);
  leds[9] = CRGB(0,0,255);
  leds[10] = CRGB(0,0,255);
  leds[11] = CRGB(0,0,255);
  leds[12] = CRGB(0,0,255);
  leds[13] = CRGB(0,0,255);
  leds[14] = CRGB(0,0,255);
  leds[15] = CRGB(0,0,255);
  leds[16] = CRGB(255,255,255);
  leds[17] = CRGB(255,255,255);
  leds[18] = CRGB(255,255,255);
  leds[19] = CRGB(255,255,255);
  leds[20] = CRGB(255,255,255);
  leds[21] = CRGB(255,255,255);
  leds[22] = CRGB(255,255,255);
  leds[23] = CRGB(255,255,255);
  FastLED.show();
  
  for(int A=2; A>=1; A--) {
    for(int i=20;i<24;i++){
      leds[i].setRGB(35,0,255);
      FastLED.show();  
    }
    delay(500);
       
       for(int i=20;i<24;i++){
         leds[i].setRGB(255,255,255);
         FastLED.show();  
     }
     delay(500);
  }
  FastLED.show();
}


 void reverseBrakeLeft(){  //-----------------------------Reverse Brake Left-------------------------------------------------------------------------------------------------------
  FastLED.clear();
  leds[0] = CRGB(255,255,255);
  leds[1] = CRGB(255,255,255);
  leds[2] = CRGB(255,255,255);
  leds[3] = CRGB(255,255,255);
  leds[4] = CRGB(255,255,255);
  leds[5] = CRGB(255,255,255);
  leds[6] = CRGB(255,255,255);
  leds[7] = CRGB(255,255,255);
  leds[8] = CRGB(0,0,255);
  leds[9] = CRGB(0,0,255);
  leds[10] = CRGB(0,0,255);
  leds[11] = CRGB(0,0,255);
  leds[12] = CRGB(0,0,255);
  leds[13] = CRGB(0,0,255);
  leds[14] = CRGB(0,0,255);
  leds[15] = CRGB(0,0,255);
  leds[16] = CRGB(255,255,255);
  leds[17] = CRGB(255,255,255);
  leds[18] = CRGB(255,255,255);
  leds[19] = CRGB(255,255,255);
  leds[20] = CRGB(255,255,255);
  leds[21] = CRGB(255,255,255);
  leds[22] = CRGB(255,255,255);
  leds[23] = CRGB(255,255,255);
  FastLED.show();
  
  for(int A=2; A>=1; A--) {
    for(int i=0;i<4;i++){
      leds[i].setRGB(35,0,255);
      FastLED.show();  
    }
    delay(500);
       
       for(int i=0;i<4;i++){
         leds[i].setRGB(255,255,255);
         FastLED.show();  
     }
     delay(500);
  }
  FastLED.show();
}

void hazerd(){      //--------------------------------------hazerd-----------------------------------------------------------------------------------------------------------------
  FastLED.clear();
  for(int A=5; A>=1; A--) {

    leds[0] = CRGB(0,0,0);
    leds[23] = CRGB(0,0,0);
    
    leds[9] = CRGB(35,0,255);
    leds[10] = CRGB(35,0,255);
    leds[11] = CRGB(35,0,255);
    leds[12] = CRGB(35,0,255);
    leds[13] = CRGB(35,0,255);
    leds[14] = CRGB(35,0,255);
    FastLED.show();
    delay(HTIME);
///////////////////////////////
    leds[8] = CRGB(35,0,255);
    leds[9] = CRGB(35,0,255);
    leds[10] = CRGB(35,0,255);
    leds[11] = CRGB(0,0,0);
    leds[12] = CRGB(0,0,0);
    leds[13] = CRGB(35,0,255);
    leds[14] = CRGB(35,0,255);
    leds[15] = CRGB(35,0,255);
    FastLED.show();
    delay(HTIME);
///////////////////////////////
    leds[7] = CRGB(35,0,255);
    leds[8] = CRGB(35,0,255);
    leds[9] = CRGB(35,0,255);
    leds[10] = CRGB(0,0,0);
    leds[13] = CRGB(0,0,0);
    leds[14] = CRGB(35,0,255);
    leds[15] = CRGB(35,0,255);
    leds[16] = CRGB(35,0,255);
    FastLED.show();
    delay(HTIME);
///////////////////////////////
    leds[6] = CRGB(35,0,255);
    leds[7] = CRGB(35,0,255);
    leds[8] = CRGB(35,0,255);
    leds[9] = CRGB(0,0,0);
    leds[14] = CRGB(0,0,0);
    leds[15] = CRGB(35,0,255);
    leds[16] = CRGB(35,0,255);
    leds[17] = CRGB(35,0,255);
    FastLED.show();
    delay(HTIME);
///////////////////////////////
    leds[5] = CRGB(35,0,255);
    leds[6] = CRGB(35,0,255);
    leds[7] = CRGB(35,0,255);
    leds[8] = CRGB(0,0,0);
    leds[15] = CRGB(0,0,0);
    leds[16] = CRGB(35,0,255);
    leds[17] = CRGB(35,0,255);
    leds[18] = CRGB(35,0,255);
    FastLED.show();
    delay(HTIME);
///////////////////////////////
    leds[4] = CRGB(35,0,255);
    leds[5] = CRGB(35,0,255);
    leds[6] = CRGB(35,0,255);
    leds[7] = CRGB(0,0,0);
    leds[16] = CRGB(0,0,0);
    leds[17] = CRGB(35,0,255);
    leds[18] = CRGB(35,0,255);
    leds[19] = CRGB(35,0,255);
    FastLED.show();
    delay(HTIME);
///////////////////////////////
    leds[3] = CRGB(35,0,255);
    leds[4] = CRGB(35,0,255);
    leds[5] = CRGB(35,0,255);
    leds[6] = CRGB(0,0,0);
    leds[17] = CRGB(0,0,0);
    leds[18] = CRGB(35,0,255);
    leds[19] = CRGB(35,0,255);
    leds[20] = CRGB(35,0,255);
    FastLED.show();
    delay(HTIME);
///////////////////////////////
    leds[2] = CRGB(35,0,255);
    leds[3] = CRGB(35,0,255);
    leds[4] = CRGB(35,0,255);
    leds[5] = CRGB(0,0,0);
    leds[18] = CRGB(0,0,0);
    leds[19] = CRGB(35,0,255);
    leds[20] = CRGB(35,0,255);
    leds[21] = CRGB(35,0,255);
    FastLED.show();
    delay(HTIME);
///////////////////////////////
    leds[1] = CRGB(35,0,255);
    leds[2] = CRGB(35,0,255);
    leds[3] = CRGB(35,0,255);
    leds[4] = CRGB(0,0,0);
    leds[19] = CRGB(0,0,0);
    leds[20] = CRGB(35,0,255);
    leds[21] = CRGB(35,0,255);
    leds[22] = CRGB(35,0,255);
    FastLED.show();
    delay(HTIME);
///////////////////////////////
    leds[0] = CRGB(35,0,255);
    leds[1] = CRGB(35,0,255);
    leds[2] = CRGB(35,0,255);
    leds[3] = CRGB(0,0,0);
    leds[20] = CRGB(0,0,0);
    leds[21] = CRGB(35,0,255);
    leds[22] = CRGB(35,0,255);
    leds[23] = CRGB(35,0,255);
    FastLED.show();
    delay(HTIME);
///////////////////////////////
    leds[0] = CRGB(35,0,255);
    leds[1] = CRGB(35,0,255);
    leds[2] = CRGB(0,0,0);
    leds[11] = CRGB(35,0,255);
    leds[12] = CRGB(35,0,255);
    leds[21] = CRGB(0,0,0);
    leds[22] = CRGB(35,0,255);
    leds[23] = CRGB(35,0,255);
    FastLED.show();
    delay(HTIME);
///////////////////////////////
    leds[0] = CRGB(35,0,255);
    leds[1] = CRGB(0,0,0);
    leds[10] = CRGB(35,0,255);
    leds[11] = CRGB(35,0,255);
    leds[12] = CRGB(35,0,255);
    leds[13] = CRGB(35,0,255);
    leds[22] = CRGB(0,0,0);
    leds[23] = CRGB(35,0,255);
    FastLED.show();
    delay(HTIME);
  }
}

void rainbow(uint8_t wait) {       //---------------------------Rainbow------------------------------------------------------------------------------------------------------------
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
  uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
  //wave-------------
  //for(int i = 0; i < NUM_LEDS-1; i++) { 
  //leds[i] = CRGB::Red; 
  //leds[i+1] = CRGB::Red;
  //FastLED.delay(33); 
 // leds[i] = CRGB::Black; 
  //leds[i+1] = CRGB::Black;


 
  
