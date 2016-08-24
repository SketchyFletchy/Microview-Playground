/* Step code debug

Testing 2 timer interrupt based step code
*/

#include "MicroView.h"

#define ENABLE_STEPPER_INTERRUPT_0()  TIMSK0|=(1<<OCIE0A)
#define DISABLE_STEPPER_INTERRUPT_0() TIMSK0&=~(1<<OCIE0A)
#define ENABLE_STEPPER_INTERRUPT_1() TIMSK2|=(1<<OCIE2A)
#define DISABLE_STEPPER_INTERRUPT_1() TIMSK2&=~(1<<OCIE2A)

volatile int Pos[] = {2000,2000};
int stepCount[] = {150,150};
const int M0StepPin = A2;   //PC0
const int M0DirPin = A3;    //PC1
const int M1StepPin = A0;   //PC2
const int M1DirPin = A1;    //PC3
const int limit0Pin = A4;
const int limit1Pin = A5;
const int ForwardPin = 6;
const int BackPin = 3;
const int maxStep = 10000;
int axisForward[] = {1,1};
volatile bool minLimit[] = {false,false};
volatile bool maxLimit[] = {false,false};

bool readForward = false;
bool readBack = false;


ISR(TIMER0_COMPA_vect) {    //Axis 0 step timer interrupt   
    minLimit[0] = (!(PINC & (1<<PC4))); //Read limit 0 PIN state.
    if (Pos[0] >= maxStep) {
        maxLimit[0] = true;
    }
    if (minLimit[0] && (axisForward[0]<1)) { //check vs. direction
        DISABLE_STEPPER_INTERRUPT_0();
    }
    else if (maxLimit[0]) {
        DISABLE_STEPPER_INTERRUPT_0();
    }
    else {  //if OK, flip step bit - use direct write to PORTx for step bit
        PORTC |= (1<<PC0);          //Set step bit
        Pos[0] += axisForward[0];   //Add step to direction
        PORTC &= ~(1<<PC0);         //Unset step bit *** Lack of wait on pulse may cause bad signalling. Investgate PWM modes.
    }
}

ISR(TIMER2_COMPA_vect) {    //Axis 1 step timer interrupt   
    minLimit[1] = (!(PINC & (1<<PC5))); //Read limit 1 PIN state.
    if (Pos[1] >= maxStep) {
        maxLimit[1] = true;
    }
    if (minLimit[1] && (axisForward[1]<1)) { //check vs. direction
        DISABLE_STEPPER_INTERRUPT_1();
    }
    else if (maxLimit[0]) {
        DISABLE_STEPPER_INTERRUPT_1();
    }
    else {  //if OK, flip step bit - use direct write to PORTx for step bit
        PORTC |= (1<<PC2);          //Set step bit
        Pos[1] += axisForward[1];   //Add step to direction
        PORTC &= ~(1<<PC2);         //Unset step bit
    }
}

void setup() {
  // put your setup code here, to run once:
    pinMode(M0StepPin, OUTPUT);
    pinMode(M0DirPin, OUTPUT);
    pinMode(M1StepPin, OUTPUT);
    pinMode(M1DirPin, OUTPUT);
    pinMode(ForwardPin, INPUT);
    pinMode(BackPin, INPUT);
    pinMode(limit0Pin, INPUT);
    pinMode(limit1Pin, INPUT);
    
   digitalWrite(M0DirPin, LOW);
   digitalWrite(M1DirPin, LOW);
   digitalWrite(ForwardPin, HIGH); //Set pullup
   digitalWrite(BackPin, HIGH);
   digitalWrite(limit0Pin, HIGH);
   digitalWrite(limit1Pin, HIGH);
  
   cli();
    
   TCCR0A = 0;             // Set entire TCCR0A register to 0
   TCCR0B = 0;             // Same for TCCR0B
   TCCR0A |= (1<<WGM01);   // Interrupt0 CTC mode enable
   TCCR0B |= ((1<<CS00)|(1<<CS02)); // Set timer 0 prescaler to 1024
   TCNT0  = 0;             // Initialise counter value to 0
   OCR0A  = stepCount[0];   // Initialise timer rollover at slowest setting to avoid excessive interrupts
   TIMSK0 = 0;
    
   TCCR2A = 0;             //Same for Timer2
   TCCR2B = 0;
   TCCR2A |= (1<<WGM21);
   TCCR2B |= ((1<<CS20)|(1<<CS22));
   TCNT2  = 0;
   OCR2A  = stepCount[1];
   TIMSK2 = 0;
    
   sei();
   uView.begin();
   uView.clear(PAGE);
}

void loop() {
  // put your main code here, to run repeatedly:
    readForward = digitalRead(ForwardPin);
    readBack = digitalRead(BackPin);
    minLimit[0] = !(digitalRead(limit0Pin));
    minLimit[1] = !(digitalRead(limit1Pin));

    if (readForward == readBack) {
      DISABLE_STEPPER_INTERRUPT_0();
      DISABLE_STEPPER_INTERRUPT_1();
    }
    else if (readForward == 0) {
      axisForward[0] = axisForward[1] = 1;
      digitalWrite(M0DirPin, LOW);
      digitalWrite(M1DirPin, LOW);
      ENABLE_STEPPER_INTERRUPT_0();
      ENABLE_STEPPER_INTERRUPT_1();
    }
    else if (readBack == 0) {
      axisForward[0] = axisForward[1] = -1;
      digitalWrite(M0DirPin, HIGH);
      digitalWrite(M1DirPin, HIGH);
      ENABLE_STEPPER_INTERRUPT_0();
      ENABLE_STEPPER_INTERRUPT_1();
    }
    uView.clear(PAGE);
    uView.setCursor(0,0);
    uView.println(Pos[0]);
    uView.println(Pos[1]);
    uView.println("Min0 Min1");
    uView.println("Max0 Max1");
    if (minLimit[0]) {
      uView.rectFill(0,16,21,8,WHITE, XOR);
    }
    if (minLimit[1]) {
      uView.rectFill(25,16,21,8,WHITE, XOR);
    }
    if (maxLimit[0]) {
      uView.rectFill(0,24,21,8,WHITE, XOR);
    }
    if (maxLimit[1]) {
      uView.rectFill(25,24,21,8,WHITE, XOR);
    }
    uView.display();
}
