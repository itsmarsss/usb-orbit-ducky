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
  Serial.println("Start");
}

void loop()
{
  Wire.requestFrom(8, 2);

  if (Wire.available() > 0)
  {
    /*
    Byte #1     Event         Short     Decimal
                Null          0         0
                Write         w         119
                Press         p         112
                Release       r         114
                ReleaseAll    l         108
                Delay         d         100
    */
    byte event = Wire.read();
    if (event == 0)
    {
      return;
    }

    /*
    Refer to https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
      - Rest of ASCII is still the same
        - eg. 65 = 'A'
    */
    byte decimal = Wire.read();

    // Debug
    Serial.print(static_cast<char>(event));
    Serial.print(" - ");
    Serial.print(static_cast<char>(decimal));
    Serial.println();

    switch (event)
    {
    case 119: // w
      write(decimal);
      break;
    case 112: // p
      press(decimal);
      break;
    case 114: // r
      release(decimal);
      break;
    case 108: // l
      releaseAll();
      break;
    case 100: // d
      delay(decimal);
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