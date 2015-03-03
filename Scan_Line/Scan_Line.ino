/*********

Scan_Line.ino
Scans a simple line vertically across the screen.
Fletcher Thompson, Me3D
Created 17/2/2015

Future revision plan:
1.2 - include a speed control
1.25 - link speed control to trim pot.

Developed in Arduino 1.0.5 using the microview toolkit

Not for distribution, this is just me fiddling around.
/**********/

#include <MicroView.h>

void setup()
{
  uView.begin();
  uView.clear(PAGE);
}

int x = 0;
int y = 0;
int SCR_WIDTH = uView.getLCDWidth();
int SCR_HEIGHT = uView.getLCDHeight();
int DrawDelay = 50;

void loop()
{
  uView.clear(PAGE);
  uView.line(x,0,x,SCR_HEIGHT);
  uView.line(0,y,SCR_WIDTH,y);
  uView.circle(x,y,10);
  uView.display();
  x++;
  if (x >= SCR_WIDTH) x=0;
  y = x*SCR_HEIGHT/SCR_WIDTH;
  delay(DrawDelay);
}

