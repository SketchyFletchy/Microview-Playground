
#include "MicroView.h"

#define ENABLE_STEPPER_INTERRUPT_0()  TIMSK1|=(1<<OCIE1A)      //16 bit double comparison Normal counter mode
#define DISABLE_STEPPER_INTERRUPT_0() TIMSK1&=~(1<<OCIE1A)
#define ENABLE_STEPPER_INTERRUPT_1() TIMSK1|=(1<<OCIE1B)
#define DISABLE_STEPPER_INTERRUPT_1() TIMSK1&=~(1<<OCIE1B)

volatile int Pos[] = {100,100};
volatile bool minLimit[] = {false, false};
volatile bool maxLimit[] = {false, false};
int axisForward[] = {1,1};
const int maxStep = 10000;
const int loopcount = 2000;

ISR(TIMER1_COMPA_vect) {
    minLimit[0] = (!(PINC & (1<<PC4))); //Read limit 0 PIN state.
    if (Pos[0] == maxStep) {
        maxLimit[0] = true;
    }
    if (minLimit[0] && (axisForward[0]<1)) { //check vs. direction
        DISABLE_STEPPER_INTERRUPT_0();
    }
    else if (maxLimit[0]) {
        DISABLE_STEPPER_INTERRUPT_0();
    }
    else {  //if OK, flip step bit - use direct write to PORTx for step bit */
        PORTD |= (1<<PD1);          //Set step bit
        Pos[0] += axisForward[0];   //Add step to direction
        OCR1A += 100;
        PORTD |= (1<<PD1);         //Unset step bit *** Lack of wait on pulse may cause bad signalling. Investgate PWM modes.
    }
}

ISR(TIMER1_COMPB_vect) {
    minLimit[1] = (!(PINC & (1<<PC5))); //Read limit 1 PIN state.
    if (Pos[1] == maxStep) {
        maxLimit[1] = true;
    }
    if (minLimit[1] && (axisForward[1]<1)) { //check vs. direction
        DISABLE_STEPPER_INTERRUPT_1();
    }
    else if (maxLimit[0]) {
        DISABLE_STEPPER_INTERRUPT_1();
    }
    else {  //if OK, flip step bit - use direct write to PORTx for step bit*/
        PORTC |= (1<<PC2);          //Set step bit
        Pos[1] += axisForward[1];   //Add step to direction
        OCR1B += 100;
        PORTC &= ~(1<<PC2);         //Unset step bit
    }
}


void setup() {
  // put your setup code here, to run once:
  cli();
  pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(1,OUTPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    digitalWrite(A4, HIGH);
    digitalWrite(A5, HIGH);
    
  TCCR1A = 0;         //Zero out Timer 1 registers - leave in normal mode and implement volatile counter array
  TCCR1B = 0;
  TCCR1B |= (1<<CS12);
  TCNT1 = 0;
  OCR1A = 1000;      //Set initial pulse rate value
  OCR1B = 500;
  TIMSK1 = 0;         //Clear Timer1 ISR mask
  
  uView.begin();
  uView.clear(PAGE);
  sei();
  ENABLE_STEPPER_INTERRUPT_0();
  ENABLE_STEPPER_INTERRUPT_1();
  digitalWrite(A2, LOW);
  digitalWrite(A2, LOW);
}

void loop() {
  static int counter = 0;
  
  // put your main code here, to run repeatedly:
  sei();
  uView.setCursor(0,0);
  uView.print(Pos[0]);
  uView.print(" ");
  uView.println(Pos[1]);
  if (minLimit[0]) {
    uView.println ("min 0");
    if (axisForward[0]==1) {
      ENABLE_STEPPER_INTERRUPT_0();
    }
  }
  if (minLimit[1]) {
    uView.println ("min 1");
    if (axisForward[1]==1) {
      ENABLE_STEPPER_INTERRUPT_1();
    }
  }
  if (maxLimit[0]) {
    uView.println ("max 0");
  }
  if (maxLimit[1]) {
    uView.println ("max 1");
  }
  uView.print(counter);
  uView.display();
  uView.clear(PAGE);
  counter++;
  if (counter == loopcount) {
    PORTC ^= ((1<<PC1)|(1<<PC3)); //Fast flip direction bits
    axisForward[0] = -1*axisForward[0];
    axisForward[1] = -1*axisForward[1];
    counter = 0;
  }
}
  
