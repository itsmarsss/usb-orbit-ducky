#ifndef ODSINTERPRETER_H
#define ODSINTERPRETER_H

#include <Arduino.h>

#include <list>

#include "KeyMap.h"
#include "Helpers.h"

class ODSInterpreter
{
public:
    static std::list<byte *> decimalStream;
    static u_int16_t interval;
    static void interpretLine(String line);
};

#endif
