#include "ODSInterpreter.h"

#include "KeyMap.h"

/*
Keyword          Purpose

COMT              Comment
TDLY              Delay
INTV              Interval

WSTR              String
W/P/R + KID       Key Identifier
W/P/R + KDC       Key Decimal
RLSA              Release All
*/

std::list<byte *> ODSInterpreter::decimalStream;

void ODSInterpreter::interpretLine(String line)
{
    int spaceIndex = line.indexOf(' ');
    if (spaceIndex != -1)
    {
        String key = line.substring(0, spaceIndex);
        String value = line.substring(spaceIndex + 1);
        try
        {
            if (key.equals("TDLY"))
            {
                delay(value.toInt());
            }
            else if (key.equals("WSTR"))
            {
                for (int i = 0; i < value.length(); i++)
                {
                    byte *byteArray = new byte[2];

                    byteArray[0] = static_cast<byte>('w');
                    byteArray[1] = static_cast<byte>(value.charAt(i));

                    Serial.println(static_cast<byte>(value.charAt(i)));
                    decimalStream.push_back(byteArray);
                }
            }
            else if (key.equals("WKID"))
            {
                byte *byteArray = new byte[2];

                byteArray[0] = static_cast<byte>('w');
                byteArray[1] = static_cast<byte>(KeyMap::keyMap.at(value.c_str()));

                Serial.println(static_cast<byte>(KeyMap::keyMap.at(value.c_str())));
                decimalStream.push_back(byteArray);
            }
            else if (key.equals("PKID"))
            {
                byte *byteArray = new byte[2];

                byteArray[0] = static_cast<byte>('p');
                byteArray[1] = static_cast<byte>(KeyMap::keyMap.at(value.c_str()));

                Serial.println(static_cast<byte>(KeyMap::keyMap.at(value.c_str())));
                decimalStream.push_back(byteArray);
            }
            else if (key.equals("RLSA"))
            {
                byte *byteArray = new byte[2];

                byteArray[0] = static_cast<byte>('l');
                byteArray[1] = static_cast<byte>('0');

                Serial.println("RELEASEALL");
                decimalStream.push_back(byteArray);
            }
        }
        catch (const std::exception &e)
        {
            Serial.println(e.what());
        }
    }
}