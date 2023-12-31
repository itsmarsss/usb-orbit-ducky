#include "ODSInterpreter.h"

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
u_int16_t ODSInterpreter::interval = 0;

void ODSInterpreter::interpretLine(String line)
{
    int spaceIndex = line.indexOf(' ');
    if (spaceIndex != -1)
    {
        try
        {
            String key = line.substring(0, spaceIndex);
            String value = line.substring(spaceIndex + 1);
            if (key.equals("TDLY"))
            {
                for (int i = 0; i < value.toInt() / 250; i++)
                {
                    byte *byteArray = new byte[2];

                    byteArray[0] = static_cast<byte>('d');
                    byteArray[1] = static_cast<byte>(250);

                    Serial.println(static_cast<byte>(250));
                    decimalStream.push_back(byteArray);
                }
                byte *byteArray = new byte[2];

                byteArray[0] = static_cast<byte>('d');
                byteArray[1] = static_cast<byte>(value.toInt() % 250);

                Serial.println(static_cast<byte>(value.toInt() % 250));
                decimalStream.push_back(byteArray);
            }
            else if (key.equals("WSTR"))
            {
                interval = value.toInt();
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
            else if (key.equals("RKID"))
            {
                byte *byteArray = new byte[2];

                byteArray[0] = static_cast<byte>('r');
                byteArray[1] = static_cast<byte>(KeyMap::keyMap.at(value.c_str()));

                Serial.println(static_cast<byte>(KeyMap::keyMap.at(value.c_str())));
                decimalStream.push_back(byteArray);
            }
            else if (key.equals("WKDC"))
            {
                byte *byteArray = new byte[2];

                byteArray[0] = static_cast<byte>('w');
                byteArray[1] = static_cast<byte>(value.toInt());

                Serial.println(static_cast<byte>(value.toInt()));
                decimalStream.push_back(byteArray);
            }
            else if (key.equals("PKDC"))
            {
                byte *byteArray = new byte[2];

                byteArray[0] = static_cast<byte>('p');
                byteArray[1] = static_cast<byte>(value.toInt());

                Serial.println(static_cast<byte>(value.toInt()));
                decimalStream.push_back(byteArray);
            }
            else if (key.equals("RKDC"))
            {
                byte *byteArray = new byte[2];

                byteArray[0] = static_cast<byte>('r');
                byteArray[1] = static_cast<byte>(value.toInt());

                Serial.println(static_cast<byte>(value.toInt()));
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

            sendInterval();
        }
        catch (const std::exception &e)
        {
            Serial.println(e.what());
        }
    }
}

void sendInterval()
{
    for (int i = 0; i < ODSInterpreter::interval / 250; i++)
    {
        byte *byteArray = new byte[2];

        byteArray[0] = static_cast<byte>('d');
        byteArray[1] = static_cast<byte>(250);

        Serial.println(static_cast<byte>(250));
        ODSInterpreter::decimalStream.push_back(byteArray);
    }
    byte *byteArray = new byte[2];

    byteArray[0] = static_cast<byte>('d');
    byteArray[1] = static_cast<byte>(ODSInterpreter::interval % 250);

    Serial.println(static_cast<byte>(ODSInterpreter::interval % 250));
    ODSInterpreter::decimalStream.push_back(byteArray);
}