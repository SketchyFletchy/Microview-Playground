
// Biopen button test

#include "MicroView.h"

const int limit0Pin = A4;   //PC4
const int limit1Pin = A5;   //PC5
const int buttUpPin = 5;    //PD5
const int buttDnPin = 3;    //PD3
const int buttOKPin = 6;    //PD6
const int triggerPin = 2;   //PD2, PCINT18 trigger option available for ISR based start/stop on ATMEGA328p

//byte lastInput = 0;
byte newInput = 0;
byte bounce[] = {0,0,0};
byte debounceMax = 10;

bool buttUpPush = false;
bool buttDnPush = false;
bool buttOKPush = false;
//String string1 = "00000000";
String string2 = "00000000";

void buttCheck() {
//    lastInput = newInput;
    newInput = PIND;
    if (!(newInput & (1<<PD5))) {    //Button Up handler (CHECK LOGIC on all these***)
        if (bounce[0] < debounceMax) {
            bounce[0]++;
        }
        else if (bounce[0] = debounceMax) {
            buttUpPush = true;
            bounce[0]++;
        }
    }
    if (newInput & (1<<PD5)) {
        bounce[0] = 0;
        buttUpPush = false;
    }
    if (!(newInput & (1<<PD3))) {    //Button Dn handler
        if (bounce[1] < debounceMax) {
            bounce[1]++;
        }
        else if (bounce[1] = debounceMax) {
            buttDnPush = true;
            bounce[1]++;
        }
    }
    if (newInput & (1<<PD3)) {
        bounce[1] = 0;
        buttDnPush = false;
    }
    if (!(newInput & (1<<PD6))) {    //Button OK handler
        if (bounce[2] < debounceMax) {
            bounce[2]++;
        }
        else if (bounce[2] = debounceMax) {
            buttOKPush = true;
            bounce[2]++;
        }
    }
    if (newInput & (1<<PD6)) {
        bounce[2] = 0;
        buttOKPush = false;
    }
    if ((buttUpPush && buttDnPush)||((buttUpPush||buttDnPush) && buttOKPush)) { //multiple input traps to nothing, cancel push flags
        buttOKPush = false;
        buttDnPush = false;
        buttUpPush = false;
        return;
    }
}

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
  buttCheck();
//  string1 = String(lastInput, BIN);
  string2 = String(newInput, BIN);
  
  uView.clear(PAGE);
  uView.setCursor(0,0);
  uView.println("UP");
//  uView.print(string1);
  uView.print("DN");
  uView.print(string2);
  uView.println("OK");
  uView.print("L0");
  uView.print(bounce[0]);
  uView.print(bounce[1]);
  uView.println(bounce[2]);
  uView.println("L1");
  uView.println("TR");
  if (buttUpPush) {
    uView.rect(0,0,10,8,WHITE,XOR);
  }
  if (buttDnPush) {
    uView.rectFill(0,8,10,8,WHITE,XOR);
  }
  if (buttOKPush) {
    uView.rectFill(0,16,10,8,WHITE,XOR);
  }
  if (digitalRead(limit0Pin) == 0) {
    uView.rectFill(0,24,10,8,WHITE,XOR);
  }
  if (digitalRead(limit1Pin) == 0) {
    uView.rectFill(0,32,10,8,WHITE,XOR);
  }
  if ((PIND & (1<<PD2)) == 0) {  //Trigger fast read handler
    uView.rectFill(0,40,10,8,WHITE,XOR);
  }
  uView.display();
}
