#include "WireService.h"

void requestEvent();

uint32_t WireService::lastReq = millis();

void WireService::setupWireService()
{
    Wire.begin(8);
    Wire.onRequest(requestEvent);
}

void requestEvent()
{
    WireService::lastReq = millis();

    if (ODSInterpreter::decimalStream.empty())
    {
        byte dataToSend[2] = {0, 0};

        Wire.write(dataToSend, 2);
        return;
    }

    byte *dataToSend = ODSInterpreter::decimalStream.front();
    ODSInterpreter::decimalStream.pop_front();

    Wire.write(dataToSend, 2);
}