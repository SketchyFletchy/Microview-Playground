/*********

Line_Spray.ino
Draws a series of lines end to end, randomising location based on a token ring.
Fletcher Thompson

Developed in Arduino 1.0.5 with the microview toolkit

Just for fartarsing around. Do whatever you like with it.
/*********/

#include <MicroView.h>

const int NoLines = 15;
byte Xpoint [NoLines];
byte Ypoint [NoLines];
byte Token = 0;
int last = 0;

void setup() {
  uView.begin();
  uView.clear(PAGE);
  for (int i=0; i<NoLines; i++) {
    Xpoint[i] = random(1,64);
    Ypoint[i] = random(1,48);
  }
}  


void loop()
{
  uView.clear(PAGE);
  Token ++;
  Xpoint[Token] = random(1,64);
  Ypoint[Token] = random(1,48);
  if (Token >= NoLines) {
    Token = 0;
  }
  for (int i=0; i<NoLines; i++) {
    int j = i + Token;
    if (j >= NoLines) {
      j = j - NoLines;
    }
    uView.line(Xpoint[last], Ypoint[last], Xpoint[j], Ypoint[j]);
    last = j;
  }
  uView.display();
  delay (10);
}
