volatile int pressure;

void setup() {
pinMode(A0, INPUT);
Serial.begin(9600);
 while (!Serial) delay(1);
  Serial.println("Pressure test");
  delay(500);
}

void loop() {
   if (isnan(pressure)) {
    pressure = 0;
   }
   pressure = analogRead(A0);
   //pressure = map(pressure, 101, 920.7, 0, 2000);
      if (isnan(pressure)) {
        Serial.println("Something wrong with sensor!");
   } else {
     Serial.print("P = "); 
     Serial.println(pressure);
   }

   delay(1000);
}
