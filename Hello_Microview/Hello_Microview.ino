#include <MicroView.h>

void setup()
{
  uView.begin();
  uView.clear(PAGE);
  uView.print("Hello,\nworld!");
  uView.display();
}

void loop() {}

