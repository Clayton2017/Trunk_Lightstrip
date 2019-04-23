/*
  Trunk LED Strip

  Custom software for chineese automotive trunk light strips using an Arduino, compatable, or my Tailgate Light Bar controller.
  These chineese "Trunk LED Strips" you can find on eBay most are simple adressable LEDs (WS2812B) wired in sets of 3 to handel 12v see 
  included documentation for more info. This software is a custom program to give a better look, annimations, features like 
  modes for brakes & Turn Signal, running oter lights. And bringing endless possibilities now with an OpenSource controler & 
  software. Please see the included documentation for more. 


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


#include <Arduino.h>
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#define LED_PIN      12         //Data Pin
#define NUM_LEDS     24         //Amout Of LEDS On Strip
#define LED_TYPE     WS2812B    //LED Chipset Type
#define COLOR_ORDER  GBR        //LED Order
#define BRIGHTNESS   100        //Brightness
#define HTIME        50         /*Hazerd Delay Variable Between Steps (ms)
//                                Change This Based On Your Particular Signals 
//                                Animation Does Not Automaticly Adjust Like The Others */

//--In Pins--------------------------------------------------------------------
const int rightPIN = 8;         //Right Turn Signal Detect
const int leftPIN  = 7;         //Left Turn Signal Detect
const int brakePIN = 9;         //Brake Detect
const int revPIN   = 10;        //Reverse Light Detect

//--States---------------------------------------------------------------------
int rightState     = 0;         //Right Light Status
int leftState      = 0;         //Left Light Status
int brakeState     = 0;         //Brake Light Status
int reverseState   = 0;         //Reverse Light Status

//--Counters-------------------------------------------------------------------
int hazcounter     = 0;         //Counter For Hazerd Function
int rtcounter      = 0;         //Counter For Right Function
int lfcounter      = 0;         //Counter For Left Function
int brcounter      = 0;         //Counter For Brake Right Function
int blcounter      = 0;         //Counter For Brake Left Function
int rrcounter      = 0;         //Counter For Reverse Right Function
int rlcounter      = 0;         //Counter For Reverse Left Function
int rbrcounter     = 0;         //Counter For Reverse Brake Right Function
int rblcounter     = 0;         //Counter For Reverse Brake Left Function

//--Void Declarations----------------------------------------------------------
void right();
void rtoff();
void left();
void lfoff();
void brake();
void brakeRight();
void broff();
void brakeLeft();
void bloff();
void reverse();
void reverseBrake();
void reverseRight();
void rroff();
void reverseLeft();
void rloff();
void reverseBrakeRight();
void rbroff();
void reverseBrakeLeft();
void rbloff();
void hazerd();
void hazerdoff();
void runing();
void readState();
void bootShows();

//--EEPROM---------------------------------------------------------------------
int ShowOnBoot  = 0;
byte ShowBoot;

//--Adafruit-------------------------------------------------------------------
CRGB leds[NUM_LEDS];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, LED_PIN, NEO_GRB + NEO_KHZ800);

//===========================================================================START===============================================
//===============================================================================================================================
void setup() {
  Serial.println("Starting!");                 //Dev & Troubleshoot
  delay(1000);
  LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);           //Setting Brigtness ForLEDs
  Serial.begin(9600);                          //inatallize Serial Moniter at 9600 Baud

  //EEPROM.write(ShowOnBoot,0);                  //EEPROM lightshow on boot (0 = off - 1 = enabled) 
  //                                             Uncomment to write be sure to re-comment after set to reduce EEPROM write wear
  //                                             Change value of "ShowOnBoot" to enable/disable

  bootShows();
  
  pinMode(rightPIN, INPUT_PULLUP);             //Set rightPin As Input / Low = Active / 20K Builtin Pullup Resistor Enabled
  pinMode(leftPIN,  INPUT_PULLUP);             //Set leftPin As Input  / Low = Active / 20K Builtin Pullup Resistor Enabled
  pinMode(brakePIN, INPUT_PULLUP);             //Set brakePin As Input / Low = Active / 20K Builtin Pullup Resistor Enabled
  pinMode(revPIN,   INPUT_PULLUP);             //Set revPin As Input   / Low = Active / 20K Builtin Pullup Resistor Enabled
  FastLED.show();
  delay(2000);
}

void loop(){ //==============================================================MAIN LOOP===========================================
  Serial.println("Main Program");                //Dev & Troubleshoot
  (hazcounter = 0);                              //Reset Hazard Animation Loop Counter
  (rtcounter  = 0);                              //Reset Right Turn Signal 
  (lfcounter  = 0);
  (brcounter  = 0);
  (blcounter  = 0); 
  (rrcounter  = 0);
  (rlcounter  = 0); 
  (rbrcounter = 0);
  (rblcounter = 0);

  readState();

  // Check if only right turn signal is on====================================================
  if (rightState == LOW && leftState == HIGH && brakeState == HIGH && reverseState == HIGH) {   //Fix Flashing?
    FastLED.clear();
    FastLED.show();
    right();
    rtoff();
  }

  // check if only left turn signal is on=====================================================
  if (rightState == HIGH && leftState == LOW && brakeState == HIGH && reverseState == HIGH) {   //Fix Flashing?
    left();
    lfoff();
  } 

  // check if the brake light is on===========================================================
  if (rightState == HIGH && leftState == HIGH && brakeState == LOW && reverseState == HIGH) {
    brake();
  } 

  // check if the brake light & right is on===================================================
  if (rightState == LOW && leftState == HIGH && brakeState == LOW && reverseState == HIGH) {   // make smooth transition to just turn sig   //Fix Flashing?
    brakeRight();
    broff();
  } 

  // check if the brake light & left is on====================================================
  if (rightState == HIGH && leftState == LOW && brakeState == LOW && reverseState == HIGH) {   // make smooth transition to just turn sig   //Fix Flashing?
    brakeLeft();
    bloff();
  } 

  // check if the reverse light is on=========================================================
  if (rightState == HIGH && leftState == HIGH && brakeState == HIGH && reverseState == LOW) {
    reverse();
  } 

  // check if the reverse light & brake is on==================================================
  if (rightState == HIGH && leftState == HIGH && brakeState == LOW && reverseState == LOW) {
    reverseBrake();
  } 

  // check if the reverse light & right is on=================================================
  if (rightState == LOW && leftState == HIGH && brakeState == HIGH && reverseState == LOW) {   // make smooth transition to just turn sig   //Fix Flashing?
    reverseRight();
    rroff();
  } 

  // check if the reverse light & Left is on======================================== ==========
  if (rightState == HIGH && leftState == LOW && brakeState == HIGH && reverseState == LOW) {   // make smooth transition to just turn sig   //Fix Flashing?
    reverseLeft();
    rloff();
  } 
 
  // check if the reverse light & brake & right is on===========================================
  if (rightState == LOW && leftState == HIGH && brakeState == LOW && reverseState == LOW) {   // Check Transisitons  //Fix Flashing?
    reverseBrakeRight();
    rbroff();
  } 

  // check if the reverse light & brake & right is on===========================================
  if (rightState == HIGH && leftState == LOW && brakeState == LOW && reverseState == LOW) {   // Check Transiitons   //Fix Flashing?
    reverseBrakeLeft();
    rbloff();
  } 

  // check if only the hazerds are on===========================================================
  if (rightState == LOW && leftState == LOW && brakeState == HIGH && reverseState == HIGH) {
    hazerd();
    hazerdoff();
  } 
  
  // check if the brake light & hazerds are on================================================
  if (rightState == LOW && leftState == LOW && brakeState == LOW && reverseState == HIGH) {    //Make animation??
    hazerd();
    hazerdoff(); 
  } 

  // check if the reverse light & hazerds are on================================================
  if (rightState == LOW && leftState == LOW && brakeState == HIGH && reverseState == LOW) {    //Make animation??
    hazerd();
    hazerdoff();
  } 

  // check if the reverse light & brakes & hazerds are on=======================================
  if (rightState == LOW && leftState == LOW && brakeState == LOW && reverseState == LOW) {    //Make animation??
    hazerd();
    hazerdoff();
  }

  // Always Running Lights======================================================================
  if (rightState == HIGH && leftState == HIGH && brakeState == HIGH && reverseState == HIGH) {
    // turn LED on:
    runing();
  } 
}

//===========================================================================SUB LOOPS===========================================
//===============================================================================================================================

void right(){      //--------------------------------------Right Turn------------------------------------------------------------
  Serial.println("Right Turn Signal");           //Dev & Troubleshoot
  leds[14] = CRGB(35,0,255);
  leds[15] = CRGB(35,0,255);
  leds[16] = CRGB(35,0,255);
  leds[17] = CRGB(35,0,255);
  leds[18] = CRGB(35,0,255);
  leds[19] = CRGB(35,0,255);
  leds[20] = CRGB(35,0,255);
  leds[21] = CRGB(35,0,255);
  leds[22] = CRGB(35,0,255);
  leds[23] = CRGB(35,0,255);
  FastLED.show();
  delay(200);
}

void rtoff(){                                    //========Right Animation code==============================
  Serial.println(rtcounter);
  delay(75);
  fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
  FastLED.show();
  
  // check if only the right turn signal is on==================
  readState();
  if (rightState == LOW && leftState == HIGH) { 
    rtcounter = 0;
    right();
    rtoff();
  } 
    else if (rtcounter >= 10){
      rtcounter == 0;
      Serial.println("Right turn off Returning to main loop!");      //Dev & Troubleshoot
  } 
    else {
      //Serial.println("Mills +1");                                    //Dev & Troubleshoot uncomment to use
      (rtcounter ++);
      rtoff();
    }
}

void left(){       //--------------------------------------Left Turn-------------------------------------------------------------
  Serial.println("Left Turn Signal");            //Dev & Troubleshoot
  FastLED.clear();
  leds[0] = CRGB(35,0,255);
  leds[1] = CRGB(35,0,255);
  leds[2] = CRGB(35,0,255);
  leds[3] = CRGB(35,0,255);
  leds[4] = CRGB(35,0,255);
  leds[5] = CRGB(35,0,255);
  leds[6] = CRGB(35,0,255);
  leds[7] = CRGB(35,0,255);
  leds[8] = CRGB(35,0,255);
  leds[9] = CRGB(35,0,255);
  FastLED.show();
  delay(200);
}

void lfoff(){                                    //========Left Animation code===============================
  Serial.println(lfcounter);
  delay(75);
  fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
  FastLED.show();
  
  // check if only the left turn signal is on==================
  readState();
  if (rightState == HIGH && leftState == LOW) { 
    lfcounter = 0;
    left();
    lfoff();
  } 
    else if (lfcounter >= 10){
      lfcounter == 0;
      Serial.println("Left turn off Returning to main loop!");       //Dev & Troubleshoot
  } 
    else {
      (lfcounter ++);
      lfoff();
    }
}

void brake(){      //--------------------------------------Brake-----------------------------------------------------------------
  Serial.println("Brake");                       //Dev & Troubleshoot
  FastLED.clear();
  fill_solid(leds, NUM_LEDS, CRGB(0,0,255));
  FastLED.show();
}

void brakeRight(){ //--------------------------------------Brake Right-----------------------------------------------------------
  Serial.println("Brake + Right");               //Dev & Troubleshoot
  FastLED.clear();
  fill_solid(leds, NUM_LEDS, CRGB(0,0,255));
  leds[17] = CRGB(0,0,0);
  leds[18] = CRGB(35,0,255);
  leds[19] = CRGB(35,0,255);
  leds[20] = CRGB(35,0,255);
  leds[21] = CRGB(35,0,255);
  leds[22] = CRGB(35,0,255);
  leds[23] = CRGB(35,0,255);
  FastLED.show();
  delay(200);
}

void broff(){                                    //========Right Brake Animation code========================
  Serial.println(brcounter);
  delay(75);
  leds[17] = CRGB(0,0,0);
  leds[18] = CRGB(0,0,0);
  leds[19] = CRGB(0,0,0);
  leds[20] = CRGB(0,0,0);
  leds[21] = CRGB(0,0,0);
  leds[22] = CRGB(0,0,0);
  leds[23] = CRGB(0,0,0);
  FastLED.show();
  
  // check if the Right turn signal & Brake is on==================
  readState();
  if (rightState == LOW && leftState == HIGH && brakeState == LOW && reverseState == HIGH) { 
    brcounter = 0;
    brakeRight();
    broff();
  } 
    else if (brcounter >= 10){
      brcounter == 0;
      Serial.println("Right & Brake off returning to main loop!");     //Dev & Troubleshoot
  }
    else {
      (brcounter ++);
      broff();
  }
}

void brakeLeft(){   //-------------------------------------Brake Left------------------------------------------------------------
  Serial.println("Brake + Left");            //Dev & Troubleshoot
  FastLED.clear();
  fill_solid(leds, NUM_LEDS, CRGB(0,0,255));
  leds[0] = CRGB(35,0,255);
  leds[1] = CRGB(35,0,255);
  leds[2] = CRGB(35,0,255);
  leds[3] = CRGB(35,0,255);
  leds[4] = CRGB(35,0,255);
  leds[5] = CRGB(35,0,255);
  leds[6] = CRGB(0,0,0);
  FastLED.show();
  delay(200);
}

void bloff(){                                    //========Brake Left Animation code=========================
  Serial.println(blcounter);
  delay(75);
  leds[0] = CRGB(0,0,0);
  leds[1] = CRGB(0,0,0);
  leds[2] = CRGB(0,0,0);
  leds[3] = CRGB(0,0,0);
  leds[4] = CRGB(0,0,0);
  leds[5] = CRGB(0,0,0);
  leds[6] = CRGB(0,0,0);
  FastLED.show();
  
  // Check If The Left Turn Signal & Brake Are On==================
  readState();
  if (rightState == HIGH && leftState == LOW && brakeState == LOW && reverseState == HIGH) { 
    blcounter = 0;
    brakeLeft();
    bloff();
  } 
    else if (blcounter >= 10){
      blcounter == 0;
      Serial.println("Left & Brake off returning to main loop!");     //Dev & Troubleshoot
  }
    else {
      (blcounter ++);
      bloff();
  } 
}

void reverse(){      //------------------------------------Reverse---------------------------------------------------------------
  Serial.println("Reverse");                        //Dev & Troubleshoot
  FastLED.clear();
  fill_solid(leds, NUM_LEDS, CRGB(255,255,255));
  FastLED.show();
}

void reverseBrake(){ //------------------------------------Reverse Brake---------------------------------------------------------
  Serial.println("Reverse & Brake");               //Dev & Troubleshoot
  FastLED.clear();
  leds[0]  = CRGB(255,255,255);
  leds[1]  = CRGB(255,255,255);
  leds[2]  = CRGB(255,255,255);
  leds[3]  = CRGB(255,255,255);
  leds[4]  = CRGB(255,255,255);
  leds[5]  = CRGB(255,255,255);
  leds[6]  = CRGB(255,255,255);
  leds[7]  = CRGB(255,255,255);
  leds[8]  = CRGB(0,0,255);
  leds[9]  = CRGB(0,0,255);
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
}

void reverseRight(){ //------------------------------------Reverse Right---------------------------------------------------------
  Serial.println("Reverse + Right");            //Dev & Troubleshoot
  FastLED.clear();
  fill_solid(leds, NUM_LEDS, CRGB(255,255,255));
  leds[17] = CRGB(0,0,0);
  leds[18] = CRGB(35,0,255);
  leds[19] = CRGB(35,0,255);
  leds[20] = CRGB(35,0,255);
  leds[21] = CRGB(35,0,255);
  leds[22] = CRGB(35,0,255);
  leds[23] = CRGB(35,0,255);
  FastLED.show();
  delay(200);
}

void rroff(){                                    //========Reverse Right Animation code======================
  Serial.println(rrcounter);
  delay(75);
  leds[17] = CRGB(0,0,0);
  leds[18] = CRGB(0,0,0);
  leds[19] = CRGB(0,0,0);
  leds[20] = CRGB(0,0,0);
  leds[21] = CRGB(0,0,0);
  leds[22] = CRGB(0,0,0);
  leds[23] = CRGB(0,0,0);
  FastLED.show();
  
  // Check If The Right Turn signal & Reverse Are On==================
  readState();
  if (rightState == LOW && leftState == HIGH && brakeState == HIGH && reverseState == LOW) { 
    rrcounter = 0;
    reverseRight();
    rroff();
  } 
    else if (rrcounter >= 10){
      rrcounter == 0;
      Serial.println("Right & Reverse returning to main loop!");     //Dev & Troubleshoot
  }
    else {
      (rrcounter ++);
      rroff();
  }
}

void reverseLeft(){  //------------------------------------Reverse Left----------------------------------------------------------
  Serial.println("Reverse + Left");            //Dev & Troubleshoot
  FastLED.clear();
  fill_solid(leds, NUM_LEDS, CRGB(255,255,255));
  leds[0] = CRGB(35,0,255);
  leds[1] = CRGB(35,0,255);
  leds[2] = CRGB(35,0,255);
  leds[3] = CRGB(35,0,255);
  leds[4] = CRGB(35,0,255);
  leds[5] = CRGB(35,0,255);
  leds[6] = CRGB(0,0,0);
  FastLED.show();
  delay(200);
}

void rloff(){                                    //========Reverse Left Animation code=======================
  Serial.println(rlcounter);
  delay(75);
  leds[0] = CRGB(0,0,0);
  leds[1] = CRGB(0,0,0);
  leds[2] = CRGB(0,0,0);
  leds[3] = CRGB(0,0,0);
  leds[4] = CRGB(0,0,0);
  leds[5] = CRGB(0,0,0);
  leds[6] = CRGB(0,0,0);
  FastLED.show();
  
  // Check If The Left Turn signal & Reverse Are On==================
  readState();
  if (rightState == HIGH && leftState == LOW && brakeState == HIGH && reverseState == LOW) { 
    rlcounter = 0;
    reverseLeft();
    rloff();
  } 
    else if (rlcounter >= 10){
      rlcounter == 0;
      Serial.println("Right & Reverse returning to main loop!");     //Dev & Troubleshoot
  }
    else {
      (rlcounter ++);
      rloff();
  }
}

void reverseBrakeRight(){  //------------------------------Reverse Brake Right---------------------------------------------------
  Serial.println("Reverse & Brake & Right");        //Dev & Troubleshoot
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
  leds[20] = CRGB(35,0,255);
  leds[21] = CRGB(35,0,255);
  leds[22] = CRGB(35,0,255);
  leds[23] = CRGB(35,0,255);
  FastLED.show();
  delay(100);
}

void rbroff(){                                   //========Reverse Brake Right Animation code================
  Serial.println(rbrcounter);
  delay(75);
  leds[20] = CRGB(0,0,0);
  leds[21] = CRGB(0,0,0);
  leds[22] = CRGB(0,0,0);
  leds[23] = CRGB(0,0,0);
  FastLED.show();
  
  // Check If The Right Turn signal & Reverse Are On==================
  readState();
  if (rightState == LOW && leftState == HIGH && brakeState == LOW && reverseState == LOW) { 
    rbrcounter = 0;
    reverseBrakeRight();
    rbroff();
  } 
    else if (rbrcounter >= 10){
      rbrcounter == 0;
      Serial.println("Reverse, Brake & Right off returning to main loop!");     //Dev & Troubleshoot
  }
    else {
      (rbrcounter ++);
      rbroff();
  }
}

void reverseBrakeLeft(){   //------------------------------Reverse Brake Left----------------------------------------------------
  Serial.println("Reverse & Brake & Left");        //Dev & Troubleshoot
  leds[0]  = CRGB(35,0,255);
  leds[1]  = CRGB(35,0,255);
  leds[2]  = CRGB(35,0,255);
  leds[3]  = CRGB(35,0,255);
  leds[4]  = CRGB(255,255,255);
  leds[5]  = CRGB(255,255,255);
  leds[6]  = CRGB(255,255,255);
  leds[7]  = CRGB(255,255,255);
  leds[8]  = CRGB(0,0,255);
  leds[9]  = CRGB(0,0,255);
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
  delay(100);
}

void rbloff(){                                   //========Reverse Brake Left Animation code=================
  Serial.println(rblcounter);
  delay(75);
  leds[0] = CRGB(0,0,0);
  leds[1] = CRGB(0,0,0);
  leds[2] = CRGB(0,0,0);
  leds[3] = CRGB(0,0,0);
  FastLED.show();
  
  // Check If Reverse, Brake & Left Signals Are On==================
  readState();
  if (rightState == HIGH && leftState == LOW && brakeState == LOW && reverseState == LOW) { 
    rblcounter = 0;
    reverseBrakeLeft();
    rbloff();
  } 
    else if (rblcounter >= 10){
      rblcounter == 0;
      Serial.println("Reverse, Brake & Left Signals are off returning to main loop!");     //Dev & Troubleshoot
  }
    else {
      (rblcounter ++);
      rbloff();
  }
}

void hazerd(){       //------------------------------------hazerd----------------------------------------------------------------
  Serial.println("Hazerd");                                  //Dev & Troubleshoot
  FastLED.clear();
  for(int A=1; A>=1; A--) {

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
    //leds[11] = CRGB(35,0,255);
    //leds[12] = CRGB(35,0,255);
    leds[21] = CRGB(0,0,0);
    leds[22] = CRGB(35,0,255);
    leds[23] = CRGB(35,0,255);
    FastLED.show();
    delay(HTIME);
  ///////////////////////////////
    leds[0] = CRGB(35,0,255);
    leds[1] = CRGB(0,0,0);
    //leds[10] = CRGB(35,0,255);
    //leds[11] = CRGB(35,0,255);
    //leds[12] = CRGB(35,0,255);
    //leds[13] = CRGB(35,0,255);
    leds[22] = CRGB(0,0,0);
    leds[23] = CRGB(35,0,255);
    FastLED.show();
    delay(HTIME);
  ///////////////////////////////
    fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
    FastLED.show();
  }
}

void hazerdoff(){                                //========Hazerd Animation code=============================
  Serial.println(hazcounter);
  delay(1);
  
  // check if both turn signals are on==================
  rightState = digitalRead(rightPIN);
  leftState = digitalRead(leftPIN);
  if (rightState == LOW && leftState == LOW) { 
    hazcounter = 0;
    hazerd();
    hazerdoff();
  } 
    else if (hazcounter >= 750){
      hazcounter == 0;
      Serial.println("Hazerds off. Returning to main loop!");     //Dev & Troubleshoot
  } 
    else {
      //Serial.println("Mills +1");                   //Dev & Troubleshoot uncomment to use
      (hazcounter ++);
      hazerdoff();
    }


}

void rainbow(int wait) { //--------------------------------Rainbow---------------------------------------------------------------
  Serial.println("Rainbow Effect");    
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void runing(){       //------------------------------------Running---------------------------------------------------------------
  Serial.println("Running");                     //Dev & Troubleshoot
  FastLED.clear();
  fill_solid(leds, NUM_LEDS, CRGB(0,0,50));
  FastLED.show();
}

void readState(){    //------------------------------------Read States-----------------------------------------------------------
  rightState   = digitalRead(rightPIN);
  leftState    = digitalRead(leftPIN);
  brakeState   = digitalRead(brakePIN);
  reverseState = digitalRead(revPIN);
}

void bootShows(){    //------------------------------------Boot Show?------------------------------------------------------------
  ShowBoot  = EEPROM.read(ShowOnBoot);
  Serial.println("Boot Show is ");              //Dev & Troubleshoot

  if (ShowBoot == 0)
  {
    Serial.print("disabled ");                  //Dev & Troubleshoot
  }
  else if (ShowBoot == 1) 
  {
    Serial.print("enabled");                    //Dev & Troubleshoot
    Serial.print(ShowBoot);
    rainbow(30);
  }
}
