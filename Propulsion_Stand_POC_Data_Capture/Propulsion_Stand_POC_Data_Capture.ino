#include "SD.h"
#include <SPI.h>
#include "Adafruit_MAX31855.h"

//SD Card Setup
const int SDpin = 4;
String dataString =""; // holds the data to be written to the SD card
float sensorReading_Temp = 0.00; // value read from your first sensor
float sensorReading_Pressure = 0.00; // value read from your second sensor
File sensorData;

// Thermocouple H/W SPI instance
#define MAXCS   10
Adafruit_MAX31855 thermocouple(MAXCS);

// Setup startSwitch
const int startSwitch = 2;
volatile int testLength = 0; // 4x2 = 8 seconds of data
volatile int pressure;


void setup() {
cli();  
//set timer0 interrupt at 1kHz (1ms)
  TCCR0A = 0; //set entire TCCR0A register to 0
  TCCR0B = 0; //same for TCCR0B
  TCNT0  = 0; //initialize counter value to 0
  OCR0A = 249; //set compare match register for 1khz increments
  TCCR0A |= (1 << WGM01); //turn on CTC mode
  TCCR0B |= (1 << CS01) | (1 << CS00); //Set CS01 and CS00 bits for 64 prescaler  
  TIMSK0 |= (1 << OCIE0A); //enable timer compare interrupt

//set timer1 interrupt at ~0.5Hz (2s)
  TCCR1A = 0; //set entire TCCR1A register to 0
  TCCR1B = 0; //same for TCCR1B
  TCNT1  = 0; //initialize counter value to 0
  OCR1A = 31249; //set compare match register for 0.5hz increments
  TCCR1B |= (1 << WGM12); //turn on CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); //Set CS10 and CS12 bits for 1024 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt

//Configuring Pins
Serial.begin(9600);
Serial.print("Initializing SD card...");
  if (!SD.begin(SDpin)) 
    Serial.println("Card failed, or not present");
pinMode(SDpin, OUTPUT);
pinMode(startSwitch, INPUT_PULLUP);
pinMode(A0, INPUT);
Serial.println("card initialized.");
Serial.println("Ready to begin test...");
while(startSwitch == 1){}
Serial.println("Input Received, Test begin..");
sei();//allow interrupts
}

void loop() {
}

ISR(TIMER0_COMPA_vect){
  //read pressure
  pressure = analogRead(A0);
  pressure = map(pressure, 101, 920.7, 0, 2000);
   if (isnan(pressure)) 
    pressure = 0;
   sensorReading_Pressure = pressure;


  //read temperature
  double temp = thermocouple.readCelsius();
   if (isnan(temp)) 
    temp = 0;
   sensorReading_Temp = temp;
   
  //store data
  dataString = String(sensorReading_Temp) + "," + String(sensorReading_Pressure); // convert to CSV
  saveData(); // save to SD card
}

ISR(TIMER1_COMPA_vect){
testLength++;
  if(testLength == 4)
    cli();
    Serial.println("Test Complete! Extract Data");
//other code to end test
}

void saveData(){
  if(SD.exists("data.csv")){ // check the card is still there
     sensorData = SD.open("data.csv", FILE_WRITE);
      if (sensorData){
         sensorData.println(dataString);
         sensorData.close();
      }
  } 
  else{
    Serial.println("Error writing to file !");
  }
}


