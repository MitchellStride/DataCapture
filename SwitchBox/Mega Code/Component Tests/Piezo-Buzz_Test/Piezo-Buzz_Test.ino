int piezoPin = 8;
int freq = 0;

void setup() {
 Serial.begin(9600);
 pinMode(piezoPin, OUTPUT);
 delay(1000);
 Serial.println("Ready to begin Piezo test...");
}
 
void loop() {
  Serial.println("Frequency = ");  
  Serial.println(freq); 
  tone(piezoPin, freq, 1000);
  delay(2000);
  freq = freq + 500;
   if(freq >= 10000)
     freq = 0;
}

  /*Tone needs 2 arguments, but can take three
    1) Pin#
    2) Frequency - this is in hertz (cycles per second) which determines the pitch of the noise made
    3) Duration - how long the tone plays
  */
