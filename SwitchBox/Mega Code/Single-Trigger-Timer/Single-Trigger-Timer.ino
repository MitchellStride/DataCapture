/*Lauch Sequence
 *    - Toggle 1 > Pot 1 > Button 1 > Toggle 2 > Button 2 >Timer Begin
 *    Code Update List
 *        - Configure code for Joysticks, Encoder, Sd card
 *        - test seven seg 
 *        - test code reiliability
 *        - add lcd instructions
 *        - make it only run once
 *        - take video once parts come in, use transistor and taser for launch.
 */

//#include <LiquidCrystal.h>
#include "SevSeg.h"

//Piezo Buzzer
#define beeper 12
//LEDs
#define redLED 0
#define yellowLED 1
#define greenLED 2
//Controls Switches
#define pushButton1 3
#define pushButton2 4
#define missleToggle1 5
#define missleToggle2 6
//#define rocker1 7
//#define rocker2 8
//#define rocker3 9
//Dip Switches
/*#define Dip1 3
#define Dip2 4
#define Dip3 5
#define Dip4 6
#define Dip5 7
#define Dip6 8
#define Dip7 9
#define Dip8 10*/
//Joysticks
//Poteniometers
#define pot1 A0
#define pot2 A1
//Encoder
//microSD

//Init LCD
/*#define LCD-RS 10
#define LCD-EN 11
#define LCD-D4 12
#define LCD-D5 13
#define LCD-D6 14
#define LCD-D7 15
LiquidCrystal lcd(LCD-RS, LCD-ED, LCD-D4, LCD-D5, LCD-D6, LCD-D7);*/

//Init Seven Seg
byte numDigits = 4;
byte digitPins[] = {2, 3, 4, 5};
byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
SevSeg TimerSeg;

int countDown;
unsigned long last_milliSeconds;

void setup(){ 
  //LEDs
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

//Controls Switches
  pinMode(pushButton1,INPUT_PULLUP);
  pinMode(pushButton2,INPUT_PULLUP);
  pinMode(missleToggle1,INPUT_PULLUP);
  pinMode(missleToggle2,INPUT_PULLUP);
  //pinMode(rocker1,INPUT_PULLUP);
  //pinMode(rocker2,INPUT_PULLUP);
  //pinMode(rocker3,INPUT_PULLUP);
  /*pinMode(Dip1,INPUT_PULLUP);
  pinMode(Dip2,INPUT_PULLUP);
  pinMode(Dip3,INPUT_PULLUP);
  pinMode(Dip4,INPUT_PULLUP);
  pinMode(Dip5,INPUT_PULLUP);
  pinMode(Dip6,INPUT_PULLUP);
  pinMode(Dip7,INPUT_PULLUP);
  pinMode(Dip8,INPUT_PULLUP);*/

  pinMode(pot1, INPUT);
  //pinMode(pot2, INPUT);
  
  TimerSeg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);
  TimerSeg.setBrightness(90);
  
  // Setup 100Hz timer (100ms)
  cli();  
  TCCR0A = 0; //set entire TCCR0A register to 0
  TCCR0B = 0; //same for TCCR0B
  TCNT0  = 0; //initialize counter value to 0
  OCR0A = 155; //set compare match register for 1khz increments
  TCCR0A |= (1 << WGM01); //turn on CTC mode
  TCCR0B |= (1 << CS02) | (1 << CS00); //Set CS01 and CS00 bits for 1024 prescaler  
  TIMSK0 |= (1 << OCIE0A); //enable timer compare interrupt
}

void loop(){
  LaunchSequence();
}

void LaunchSequence(){
 while(digitalRead(missleToggle1) == HIGH){}
 delay(20); //wait and debounce
 digitalWrite(redLED, HIGH);
 setSevSeg();
 digitalWrite(yellowLED, HIGH);
 tone(beeper, 1000, 50);
 delay(100);
 tone(beeper, 1000, 50);
 delay(100);
 tone(beeper, 1000, 50);
 while(digitalRead(missleToggle2) == HIGH){}
 digitalWrite(greenLED, HIGH);
 while(digitalRead(pushButton2) == HIGH){}
 tone(beeper, 1000, 50);
 LaunchTime();
 tone(beeper, 440, 1000);
 //Launch
}

void setSevSeg(){
  while(digitalRead(pushButton1) == HIGH){
    countDown = map(analogRead(pot1), 0, 1023, 5, 60);
    countDown*= 10;
    TimerSeg.setNumber(countDown, 2);
    TimerSeg.refreshDisplay();   // might need small delay here
    }
}

void LaunchTime(){
  sei();
  while(countDown != 0){
    TimerSeg.setNumber(countDown, 2);
    TimerSeg.refreshDisplay();
    
    if(countDown >= 30){
      if (countDown % 10 == 0)
        tone(beeper, 1000, 50); 
    } else if(countDown % 2 == 0){
      tone(beeper, 1000, 50);
    } 
  }
}

ISR(TIMER0_COMPA_vect){
  countDown--;
  if(countDown == 0)
    cli();
  }





