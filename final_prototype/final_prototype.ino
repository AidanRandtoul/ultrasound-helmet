#include <avr/io.h>
#include <avr/interrupt.h>

#define  PHASE1   0x20
#define  PHASE2   0x10
#define  MASK     (PHASE1 | PHASE2)
#define  CYCLES   5
#define  TIMEOUT   8000

const byte interruptPin = 2;
const byte speakerPin = 6;
unsigned int reload = 0x30D4;   // Trigger Pulse every 0.2s => 12500

unsigned long start;
volatile float distance = -1;
bool waiting = false;
volatile int count = 0;

ISR(TIMER1_COMPA_vect)
{
pulseTrigger();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pulseDetected, FALLING);

  cli();
  TCCR1A = 0;
  TCCR1B = 0; 
  OCR1A = reload;
  TCCR1B = (1<<WGM12) | (1<<CS12); 
  TIMSK1 = (1<<OCIE1A); 
  sei();
}

void loop() {

  if (count == 5) {
    distance = -1;
  }
  
  if (distance != -1) {
    beep();
    delay(int(distance * 1000));
  }
}


// Function to trigger Ultrasonic Pulses
void pulseTrigger() {

  start = micros();   // Time of First Pulse
  DDRD = MASK;
  
  for (int i=0; i < CYCLES; i++) {
    PORTD = PHASE1;
    __builtin_avr_delay_cycles (244);   // Delay of 15.25us
    PORTD = PHASE2;
    __builtin_avr_delay_cycles (244);   // Delay of 15.25us
  }

  waiting = true;
  count++;
}


// Function triggered when pulse response detected
void pulseDetected() {

  if (waiting = false) { return; }
  
  unsigned long end = micros();
  unsigned long time;
  
  // Check for timer wraparound
  if (end > start)
    time = end - start;
  else
    time = (end + ~start) - (start + ~start);

  if (time < 180) {
    return;
  }

  distance = (time * 0.00034) / 2; 
  
  waiting = false;
  count = 0;
}


// Function to trigger beeps
void beep() {

  analogWrite(speakerPin, 5);
  delay(20);
  analogWrite(speakerPin, 0);
}
