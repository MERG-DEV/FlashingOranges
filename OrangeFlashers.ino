// This sketch is for an Arduino Nano and is used to emulate orange flashing warning lights
//
// Ian Morgan August 2019

// setting the 16-bit timer1 interrupt to run at 1kHz
#define TIMER_PRELOAD 65473 // preload timer 65536-(16MHz/256/1000Hz)

#define NUM_LIGHTS    2 // How many orange LEDs are to be attached
#define ON            1
#define OFF           0

// static variables are not supposed to be modified by another routine, so the compiler can optimise
// code by holding values in registers instead of reading from RAM all the time

// For this sketch, the values in these variables will never change while running.
// However, modifying these values, and recompiling and loading the sketch into the Arduino will
// change how it operate.
// If NUM_LIGHTS is modified, then that number of values need to be repeated inside
// the curly brackets

const int lightPin[NUM_LIGHTS] = {5, 6}; // define the pins that will power the lights
const int flashDuration[NUM_LIGHTS] = {50, 300}; // milliseconds LED is on while flashing
const int flashOffDuration[NUM_LIGHTS] = {500, 575}; // milliseconds LED is off while flashing
const int workingTime[NUM_LIGHTS] = {11, 17}; // seconds the flasher is working for
const int inhibitedTime[NUM_LIGHTS] = {12, 19}; // seconds the flasher is inhibited for

// volatile variables may be modified and used by both interrupt and loop routines,
// so the compiler should not optimise code, but should always get the value from RAM every
// time it is used

// These values change as the program operates, so do not change these values.
// If NUM_LIGHTS is modified, then that number of values need to be repeated inside
// the curly brackets

volatile int lightNumber = 0; // the number of the LED to be refreshed by the interrupt routine
volatile int lightState[NUM_LIGHTS] = {OFF,OFF}; // should each light should currently be on or off
volatile int countdown[NUM_LIGHTS] = {1, 1}; // milliseconds LED has remaining before changing
volatile int inhibitCountdown[NUM_LIGHTS] = {1, 1}; // seconds left before changing state
volatile int active[NUM_LIGHTS] = {ON, ON}; // current state of the flashers, active or inhibited

// =============================================================================
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize digital pins powering the orange LEDs as outputs.
  for (int i = 0; i< NUM_LIGHTS; i++)
  {
    pinMode (lightPin[i], OUTPUT);
  }

  // initialize timer1 which will be used to run an
  // interrupt handler every 1/1000 second
  
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = TIMER_PRELOAD; 
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt

  interrupts();             // enable all interrupts

  // light up the on-board LED
  digitalWrite(LED_BUILTIN, ON);   

} // end of setup ==========================================================================================

void loop() {

  digitalWrite(LED_BUILTIN, OFF);   // turn the on-board LED off
  delay(500); // wait for a period of 1/2 second
  digitalWrite(LED_BUILTIN, ON);   // turn the on-board LED on again
  delay(500); // wait for a period of 1/2 second

  // because of the above delays, the following code will run about once every second.
  // The code inside the 'for' loop will run for each orange LED
  for (int i = 0; i< NUM_LIGHTS; i++)
  {
    inhibitCountdown[i]--; // decrease the countdown by 1 second

    // has the countdown reached its end?
    if (inhibitCountdown[i] == 0)
    {
      if (active[i] == ON)
      {
        active[i] = OFF; // flashing was active, so inhibit it
        inhibitCountdown[i] = inhibitedTime[i]; // reset the countdown
      }
      else
      {
        active[i] = ON; // flashing was inhibited, so activate it
        inhibitCountdown[i] = workingTime[i]; // reset the countdown
      }
      
    }
  }
    
} // end of loop ============================================================================================

// timer interrupt service routine that runs every 1/1000 second
ISR(TIMER1_OVF_vect)        
{
  // Fires 1000 times per second
  noInterrupts(); // disable any interrupts
  TCNT1 = TIMER_PRELOAD;            // preload timer

  // switch off currently lit LED
  digitalWrite(lightPin[lightNumber], OFF);

  // point at the next LED
  lightNumber++;
  if (lightNumber >= NUM_LIGHTS) lightNumber = 0;

  // is flashing activated for this LED?
  if (active[lightNumber] == ON)
  {
    // Switch on the LED, if it is supposed to alight now
    if (lightState[lightNumber] == ON)
    {
      digitalWrite(lightPin[lightNumber], ON);
    }

    countdown[lightNumber]--; // decrease the countdown for this LED
    
    // Has the countdown reached its end?
    if (countdown[lightNumber] == 0)
    {

      // change the current required state of the LED
      if (lightState[lightNumber] == ON)
      {
        lightState[lightNumber] = OFF;
        // reset the countdown value
        countdown[lightNumber] = flashOffDuration[lightNumber];
      }
      else
      {
        lightState[lightNumber] = ON;
        // reset the countdown value
        countdown[lightNumber] = flashDuration[lightNumber];
      }
    }
  }
  interrupts(); // re-enable interrupts
}
