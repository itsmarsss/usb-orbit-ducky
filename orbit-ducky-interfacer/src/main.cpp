#include <Arduino.h>
#include <Keyboard.h>

#include <Wire.h>

void write(byte decimal);
void press(byte decimal);
void release(byte decimal);
void releaseAll();

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Keyboard.begin();
}

void loop()
{
  Wire.requestFrom(8, 2);

  if (Wire.available())
  {
    /*
    Byte #1     Value     Event         Short     Decimal
                0         Write         w         119
                1         Press         p         112
                2         Release       r         114
                3         ReleaseAll    l         108
    */
    byte event = Wire.read();

    /*
    Refer to https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
      - Rest of ASCII is still the same
        - eg. 65 = 'A'
    */
    byte decimal = Wire.read();

    Keyboard.write(decimal);

    // Debug
    Serial.print(static_cast<char>(event));
    Serial.print(" - ");
    Serial.print(static_cast<char>(decimal));
    Serial.println();

    switch (event)
    {
    case 0: // w
      write(decimal);
      break;
    case 1: // p
      press(decimal);
      break;
    case 2: // r
      release(decimal);
      break;
    case 3: // l
      releaseAll();
      break;
    }
  }

  delay(10);
}

void write(byte decimal)
{
  Keyboard.write(decimal);
}

void press(byte decimal)
{
  Keyboard.press(decimal);
}

void release(byte decimal)
{
  Keyboard.release(decimal);
}

void releaseAll()
{
  Keyboard.releaseAll();
}