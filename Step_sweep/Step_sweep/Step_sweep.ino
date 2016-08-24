/********

Step_sweep.ino
A quick playground for bipolar stepper driver control using MicroView.
Fletcher Thompson, April 2015

Developed in Arduino 1.6.3

**********/

#include <MicroView.h>

#define STEPS 200

MicroViewWidget *widget[2];

const int stepPin0 = A0;
const int stepPin1 = A2;
const int dirPin0 = A1;
const int dirPin1 = A3;
const int stepperEnablePin = 1;
long pos0 = 0;
long pos1 = 0;
int delay0 = 1500;
int delay1 = 1500;
int redrawDelay = 10;
unsigned long t = 0;
unsigned long last0 = 0;
unsigned long last1 = 0;
unsigned long lastdraw = 0;
boolean forward0 = true;
boolean forward1 = true;

void setup() {
  pinMode(stepPin0, OUTPUT);
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin0, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepperEnablePin, OUTPUT);
  digitalWrite(dirPin0, HIGH);
  digitalWrite(dirPin1, HIGH);
  digitalWrite(stepperEnablePin, LOW);
  uView.begin();
  widget[0] = new MicroViewGauge(16,24,0,199, WIDGETSTYLE0);
  widget[1] = new MicroViewGauge(48,24,0,199, WIDGETSTYLE0);
  uView.clear(PAGE);
  widget[0]->reDraw();
  widget[1]->reDraw();
}

void loop() {
  t = millis();
  if (t - last0 >= delay0) { 
    digitalWrite(stepPin0, HIGH);
    last0 = t;
    if (forward0) {
      pos0++;
    }
    else {
      pos0--;
    }
    widget[0]->setValue(pos0 % STEPS);
    delay(1);
    digitalWrite(stepPin0, LOW);
  }
  if (t - last1 >= delay1) {
    digitalWrite(stepPin1, HIGH);
    last1 = t;
    if (forward1) {
      pos1++;
    }
    else {
      pos1--;
    }
    widget[1]->setValue(pos1 % STEPS);
    delay(1);
    digitalWrite(stepPin1, LOW);
  }
  if (pos0 >= 60*STEPS) {
    digitalWrite(dirPin0, LOW);
    forward0 = false;
  }
  if (pos0 <= 0) {
    digitalWrite(dirPin0, HIGH);
    forward0 = true;
  }
  if (pos1 >= 30*STEPS) {
    digitalWrite(dirPin1, LOW);
    forward1 = false;
  }
  if (pos1 <= 0) {
    digitalWrite(dirPin1, HIGH);
    forward1 = true;
  }
  if (t-lastdraw >= redrawDelay) {
    uView.display();
  }
}
