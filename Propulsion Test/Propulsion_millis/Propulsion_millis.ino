#include "SD.h"
#include <SPI.h>
#include "Adafruit_MAX31855.h"

//SD Card Setup
const int SDpin = 4;
String dataString = ""; // holds the data to be written to the SD card
float sensorReading_Temp = 0.00; // value read from your first sensor
float sensorReading_Pressure = 0.00; // value read from your second sensor
float SAVEms = 0.00;
File sensorData;

// Thermocouple H/W SPI instance
#define MAXCS   10
Adafruit_MAX31855 thermocouple(MAXCS);

// Setup startSwitch
const int startSwitch = 2;

int pressure;
double temp;
unsigned long STARTms;

void setup() {
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

  while (startSwitch == 1) {}
  Serial.println("Input Received, Test begin..");
  STARTms = millis();
}

void loop() {
  if ((millis() - STARTms) >= 8000) { //Run test for 8s

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

    //save time stamp
    SAVEms = millis() - STARTms;

    //store data
    dataString = String(sensorReading_Temp) + "," + String(sensorReading_Pressure) + "," + String(SAVEms); // convert to CSV
    saveData(); // save to SD card
  }
}

  void saveData() {
    if (SD.exists("data.csv")) { // check the card is still there
      sensorData = SD.open("data.csv", FILE_WRITE);
      if (sensorData) {
        sensorData.println(dataString);
        sensorData.close();
      }
    }
    else {
      Serial.println("Error writing to file !");
    }
  }


