#include <Arduino.h>

#include <list>

/*
Keyword          Purpose

COM              Comment
DLY              Delay
INT              Interval

STR              String
KID              Key Identifier
KDC              Key Decimal
*/

std::list<byte *> decimalStream;

void interpretLine(String line)
{
    byte *byteArray = new byte[2];

    int spaceIndex = line.indexOf(' ');
    if (spaceIndex != -1)
    {
        String key = line.substring(0, spaceIndex);
        String value = line.substring(spaceIndex + 1);
        try
        {
            if (key.equals("DLY"))
            {
                delay(value.toInt());
            }
            else if (key.equals("STR"))
            {
                for (int i = 0; i < value.length(); i++)
                {
                    byteArray[0] = static_cast<byte>('w');
                    byteArray[1] = static_cast<byte>(value.charAt(i));

                    decimalStream.push_back(byteArray);
                }
            }
        }
        catch (const std::exception &e)
        {
            Serial.println(e.what());
        }
    }
}