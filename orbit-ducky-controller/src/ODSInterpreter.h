#ifndef ODSINTERPRETER_H
#define ODSINTERPRETER_H

#include <Arduino.h>

#include <list>

class ODSInterpreter
{
public:
    static std::list<byte *> decimalStream;
    static void interpretLine(String line);
};

#endif
