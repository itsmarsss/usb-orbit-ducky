#include <Arduino.h>

/*
Keyword          Purpose

COM              Comment
DLY              Delay
INT              Interval

STR              String
KID              Key Identifier
KDC              Key Decimal
*/
byte *interpretLine(String line)
{
    byte *byteArray = new byte[2];
    byteArray[0] = static_cast<byte>('w');
    byteArray[1] = static_cast<byte>('A');
    return byteArray;
}