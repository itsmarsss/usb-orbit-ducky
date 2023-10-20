#include <Arduino.h>
#include <Keyboard.h>

// put function declarations here:
int myFunction(int, int);

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  Keyboard.begin();
}

void loop()
{
  // if (Serial.available())
  Keyboard.write('A'); // Serial.read()
  delay(10);
  Serial.write("Hi");
}