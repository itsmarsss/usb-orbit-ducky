#include "Global.h"

std::list<byte *> decimal_stream;

uint32_t getLastReq()
{
    return last_req;
}

void setLastReq(uint32_t lastReq)
{
    last_req = lastReq;
}

std::list<byte *> getDecimalStream()
{
    return decimal_stream;
}

void setDecimalStream(std::list<byte *> decimalStream)
{
    decimal_stream = decimalStream;
}
