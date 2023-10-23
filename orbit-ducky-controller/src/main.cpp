#include <Arduino.h>

#include "Setup.h"
#include "WireService.h"
#include "WebService.h"

void requestEvent();

void setup()
{
  Serial.begin(115200);
  Serial.println("~~ USB Orbit Ducky ~~");

  mountSPIFFS();

  Serial.println(runScript("/Aaa"));

  WireService::setupWireService();

  setupAP();

  setupServer();
}

void loop()
{
  delay(100);
}
