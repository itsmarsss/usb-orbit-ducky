#include <Arduino.h>
#include <Wire.h>

class WireService
{
public:
    static uint32_t lastReq;
    static void setupWireService();
};
