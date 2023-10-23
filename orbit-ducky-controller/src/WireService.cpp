#include "WireService.h"

uint32_t WireService::lastReq = millis();

void WireService::setupWireService()
{
    Wire.begin(8);
    Wire.onRequest(requestEvent);
}

void requestEvent()
{
    WireService::lastReq = millis();

    if (decimalStream.empty())
    {
        byte dataToSend[2] = {0, 0};

        Wire.write(dataToSend, 2);
        return;
    }

    byte *dataToSend = decimalStream.front();
    decimalStream.pop_front();

    Wire.write(dataToSend, 2);
}