
// Biopen button test

#include "MicroView.h"

const int limit0Pin = A4;   //PC4
const int limit1Pin = A5;   //PC5
const int buttUpPin = 5;    //PD5
const int buttDnPin = 3;    //PD3
const int buttOKPin = 6;    //PD6
const int triggerPin = 2;   //PD2, PCINT18 trigger option available for ISR based start/stop on ATMEGA328p

void setup() {
  pinMode(limit0Pin, INPUT);
  pinMode(limit1Pin, INPUT);
  pinMode(buttUpPin, INPUT);
  pinMode(buttDnPin, INPUT);
  pinMode(buttOKPin, INPUT);
  pinMode(triggerPin, INPUT);
    
    //Set internal pullups
    digitalWrite(limit0Pin, HIGH);  //  OK
    digitalWrite(limit1Pin, HIGH);  //  OK
    digitalWrite(buttUpPin, HIGH);  //  OK
    digitalWrite(buttDnPin, HIGH);  //  OK
    digitalWrite(buttOKPin, HIGH);  //  OK
    digitalWrite(triggerPin, HIGH);  // OK
  
  uView.begin();
  uView.clear(ALL);
}

void loop() {
  uView.clear(PAGE);
  uView.setCursor(0,0);
  uView.println("UP");
  uView.println("DN");
  uView.println("OK");
  uView.println("L0");
  uView.println("L1");
  uView.println("TRIG");
  if (digitalRead(buttUpPin)) {
    uView.circle(15, 4, 2);
  }
  if (digitalRead(buttDnPin)) {
    uView.circle(15, 12, 2);
  }
  if (digitalRead(buttOKPin)) {
    uView.circle(15, 20, 2);
  }
  if (digitalRead(limit0Pin)) {
    uView.circle(15, 28, 2);
  }
  if (digitalRead(limit1Pin)) {
    uView.circle(15, 36, 2);
  }
  if (digitalRead(triggerPin)) {
    uView.circle(28, 42, 2);
  }
  
  
  uView.display();
}
