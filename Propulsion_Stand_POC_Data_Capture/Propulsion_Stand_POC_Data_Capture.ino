// Setup startSwitch
const int startSwitch = 2;
int testLength = 0 // 4x2 = 8 seconds of data

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

//Wait to start data capture.
pinMode(startSwitch, INPUT_PULLUP);
while(startSwitch == 1){}
sei();//allow interrupts
}


void loop() {
}

ISR(TIMER1_COMPA_vect){
//check volatile
//read thermocouple and pressure
//sd card csv file
}

ISR(TIMER1_COMPA_vect){
testLength++;
  if(testLength == 4)
    cli;
//other code to end test
    
}



