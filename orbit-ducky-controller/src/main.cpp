#include <Arduino.h>

#include <Wire.h>

#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>

#include "wifi_credentials.h"

#include <list>

void requestEvent();

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}
IPAddress IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

std::list<byte> decimalStream;

void setup()
{
  Serial.begin(115200);

  if (!SPIFFS.begin(true))
  {
    Serial.println("Error mounting SPIFFS");
    return;
  }

  WiFi.softAP(ssid, password);
  delay(500);
  WiFi.softAPConfig(IP, gateway, subnet);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false); });

  server.begin();

  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop()
{
  delay(100);
}

void requestEvent()
{
  decimalStream.push_back(65);

  byte dataToSend[2] = {static_cast<byte>('w'), decimalStream.front()};
  decimalStream.pop_front();

  Wire.write(dataToSend, 2);
}