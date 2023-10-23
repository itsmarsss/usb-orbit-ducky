#include <Arduino.h>

#include <list>

uint32_t getLastReq();
void setLastReq(uint32_t lastReq);

std::list<byte *> getDecimalStream();
void setDecimalStream(std::list<byte *> decimalStream);