#include <Arduino.h>

#include <Wire.h>

#include <ESP.h>

#include "Setup.h"
#include "EventHandlers.h"
#include "WebServer.h"

void requestEvent();

void setup()
{
  Serial.begin(115200);
  Serial.println("~~ USB Orbit Ducky ~~");

  mountSPIFFS();

  Serial.println(runScript("Aaa"));

  Wire.begin(8);
  Wire.onRequest(requestEvent);

  setupAP();

  setupServer();
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
