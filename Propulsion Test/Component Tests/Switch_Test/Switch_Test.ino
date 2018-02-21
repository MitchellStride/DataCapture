const int Switch = 2;
bool state;
bool last_state = 1;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.print("Ready to test switch....");
  Serial.println();
}

void loop() {
  state = digitalRead(Switch);
  if(last_state != state){
    if(state = 0){
       digitalWrite(LED_BUILTIN, HIGH);
       Serial.print("Switch is ON - pulled low");
       Serial.println();
    }
   if(state = 1){
       digitalWrite(LED_BUILTIN, LOW);
       Serial.print("Switch is OFF");
       Serial.println();
   }
  }
 last_state = state;
}
