/*Lauch Sequence
      - Toggle 1 > Pot 1 > Button 1 > Toggle 2 > Button 2 >Timer Begin
      Code Update List
          - Configure code for Joysticks, Encoder, Sd card
          - test seven seg
          - test code reiliability
          - add lcd instructions
          - make it only run once
          - take video once parts come in, use transistor and taser for launch.
*/

#include "Adafruit_LiquidCrystal.h"
#include "SevSeg.h"

//Piezo Buzzer
#define beeper 12
//LEDs
#define redLED 34
#define yellowLED 35
#define greenLED 36
//Controls Switches
#define pushButton1 37
#define pushButton2 38
#define missleToggle1 39
#define missleToggle2 40
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
//#define pot2 A1
//Encoder
//microSD

//I2C LCD
Adafruit_LiquidCrystal lcd(0);
byte StraightH[] = {0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x04};
byte TLCorner[] = {0x00, 0x00, 0x00, 0x07, 0x04, 0x04, 0x04, 0x04};
byte TRCorner[] = {0x00, 0x00, 0x00, 0x1C, 0x04, 0x04, 0x04, 0x04};
byte BLCorner[] = {0x04, 0x04, 0x04, 0x07, 0x00, 0x00, 0x00, 0x00};
byte BRCorner[] = {0x04, 0x04, 0x04, 0x1C, 0x00, 0x00, 0x00, 0x00};

byte progressFull[] = {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};
byte progressEmpty[] = {0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte progressEnd[] = {0x1F, 0x01, 0x01, 0x01F, 0x01, 0x01, 0x01, 0x01};

/*//Init LCD without i2c
  #define LCD-RS 22
  #define LCD-EN 23
  #define LCD-D4 24
  #define LCD-D5 25
  #define LCD-D6 26
  #define LCD-D7 27
  LiquidCrystal lcd(LCD-RS, LCD-EN, LCD-D4, LCD-D5, LCD-D6, LCD-D7);*/

//Init Seven Seg
byte numDigits = 4;
byte digitPins[] = {22, 23, 24, 25};
byte segmentPins[] = {26, 27, 28, 29, 30, 31, 32, 33};
SevSeg TimerSeg;

int countDown;
unsigned long last_milliSeconds;

void setup() {
  //LEDs
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  //Controls Switches
  pinMode(pushButton1, INPUT_PULLUP);
  pinMode(pushButton2, INPUT_PULLUP);
  pinMode(missleToggle1, INPUT_PULLUP);
  pinMode(missleToggle2, INPUT_PULLUP);
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

  //LCD
  lcd.begin(20, 4);
  lcd.createChar(0, StraightH);
  lcd.createChar(1, TLCorner);
  lcd.createChar(2, TRCorner);
  lcd.createChar(3, BLCorner);
  lcd.createChar(4, BRCorner);
  lcd.createChar(5, progressFull);
  lcd.createChar(6, progressEmpty);
  lcd.createChar(7, progressEnd);
  
  outlineLCD();
  lcd.setCursor(1, 0);
  lcd.print("Stride Electronics");
  lcd.setCursor(3, 2);
  lcd.print("Control Panel");
  

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

void loop() {
  LaunchSequence();
}

void LaunchSequence() {
  progressBar(0);
  while (digitalRead(missleToggle1) == HIGH) {}
  progressBar(1);
  delay(20); //wait and debounce
  digitalWrite(redLED, HIGH);
  setSevSeg();
  progressBar(2);
  digitalWrite(yellowLED, HIGH);
  tone(beeper, 1000, 50);
  delay(100);
  tone(beeper, 1000, 50);
  delay(100);
  tone(beeper, 1000, 50);
  while (digitalRead(missleToggle2) == HIGH) {}
  progressBar(3);
  digitalWrite(greenLED, HIGH);
  while (digitalRead(pushButton2) == HIGH) {}
  tone(beeper, 1000, 50);
  progressBar(4);
  LaunchTime();
  tone(beeper, 440, 1000);
  //Launch
}

void setSevSeg() {
  while (digitalRead(pushButton1) == HIGH) {
    countDown = map(analogRead(pot1), 0, 1023, 5, 60);
    countDown *= 10;
    TimerSeg.setNumber(countDown, 2);
    TimerSeg.refreshDisplay();   // might need small delay here
  }
}

void LaunchTime() {
  sei();
  while (countDown != 0) {
    TimerSeg.setNumber(countDown, 2);
    TimerSeg.refreshDisplay();

    if (countDown >= 30) {
      if (countDown % 10 == 0)
        tone(beeper, 1000, 50);
    } else if (countDown % 2 == 0) {
      tone(beeper, 1000, 50);
    }
  }
}

ISR(TIMER0_COMPA_vect) {
  countDown--;
  if (countDown == 0)
    cli();
}

void outlineLCD(){
  //TL Corner
  lcd.home();
  lcd.write(1);
  //TR Corner
  lcd.setCursor(19, 0);
  lcd.write(2);
  //BL Corner
  lcd.setCursor(0, 3);
  lcd.write(3);
  //BR Corner
  lcd.setCursor(19, 3);
  lcd.write(4);
  //Horizontal Lines
  for(int i = 1; i < 20; i++){
    lcd.setCursor(i, 0);
    lcd.write(0);
    lcd.setCursor(i, 3);
    lcd.write(0);
  }
  //Vertical Lines
  for(int j = 1; j < 4; j++){
    lcd.setCursor(0, j);
    lcd.print("l");
    lcd.setCursor(19, j);
    lcd.print("l");
  }
}


void progressBar(int progress){
switch (progress) {
    case 0:
        //Clear Screen
        lcd.clear();
        outlineLCD();
        //Step 0
        lcd.setCursor(6, 1);
        lcd.print("Flip TS1");
        //Progress Bar Setup
        lcd.setCursor(7, 2);
        lcd.write(5);
        lcd.setCursor(11, 2);
        lcd.write(7);
          for(int  x = 8; x < 11; x++){
          lcd.setCursor(x, 2);
          lcd.write(6);
         }
      break;
    case 1:    
        //Progress Update
        lcd.setCursor(8, 2);
        lcd.write(5);
        //Step 1
        lcd.setCursor(3, 1);
        lcd.print("Pot1 > PB1");
      break;
    case 2:    
        //Progress Update
        lcd.setCursor(9, 2);
        lcd.write(5);
        //Step 2
        lcd.setCursor(6, 1);
        lcd.print("Flip TS2");
      break;
    case 3:    
        //Progress Update
        lcd.setCursor(10, 2);
        lcd.write(5);
        lcd.setCursor(2, 1);
        lcd.print("!PB2 to launch!");
      break;
     case 4:  
        //Progress Complete
        lcd.setCursor(5, 2);
        lcd.write(5);
        lcd.setCursor(2, 1);
        lcd.print("Countdown!");
      break;
  }

}




