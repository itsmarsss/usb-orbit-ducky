#include <Arduino.h>

#include "SPIFFSService.h"
#include "WireService.h"
#include "APService.h"
#include "WebService.h"

void requestEvent();

void setup()
{
  Serial.begin(115200);
  Serial.println("~~ USB Orbit Ducky ~~");

  SPIFFSService::setupSPIFFS();

  Serial.println(runScript("/Aaa"));

  WireService::setupWireService();

  APService::setupAP();

  WebServer::setupServer();
}

void loop()
{
  delay(100);
}
