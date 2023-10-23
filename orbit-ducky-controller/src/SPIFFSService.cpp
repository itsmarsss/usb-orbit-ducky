#include "SPIFFSService.h"

void SPIFFSService::setupSPIFFS()
{
    Serial.println("SPIFFS:");
    if (!SPIFFS.begin(true))
    {
        Serial.println("\tError mounting SPIFFS");
        ESP.restart();
    }
    Serial.println("\tMounted SPIFFS");
    Serial.println();
}