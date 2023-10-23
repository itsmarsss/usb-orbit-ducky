#ifndef WIRESERVICE_H
#define WIRESERVICE_H

#include <Arduino.h>
#include <Wire.h>

#include <ODSInterpreter.h>

class WireService
{
public:
    static uint32_t lastReq;
    static void setupWireService();
};

#endif
