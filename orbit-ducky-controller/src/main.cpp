#include <Arduino.h>

#include <Wire.h>

#include <ESP.h>

#include "Setup.h"
#include "EventHandlers.h"
#include "OrbitDuckyWebServer.h"

void requestEvent();

void setup()
{
  Serial.println("~~ USB Orbit Ducky ~~");

  Serial.begin(115200);

  mountSPIFFS();

  setupAP();

  setupServer();

  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop()
{
  delay(100);
}

void requestEvent()
{
  last_req = millis();

  if (decimalStream.empty())
  {
    byte dataToSend[2] = {0, 0};

    Wire.write(dataToSend, 2);
    return;
  }

  byte dataToSend[2] = {static_cast<byte>('w'), decimalStream.front()};
  decimalStream.pop_front();

  Wire.write(dataToSend, 2);
}
