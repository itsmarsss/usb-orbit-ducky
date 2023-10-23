#include "ODSInterpreter.h"

/*
Keyword          Purpose

COM              Comment
DLY              Delay
INT              Interval

STR              String
KID              Key Identifier
KDC              Key Decimal
*/

void ODSInterpreter::interpretLine(String line)
{
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
                    byte *byteArray = new byte[2];

                    byteArray[0] = static_cast<byte>('w');
                    byteArray[1] = static_cast<byte>(value.charAt(i));

                    Serial.println(static_cast<byte>(value.charAt(i)));
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