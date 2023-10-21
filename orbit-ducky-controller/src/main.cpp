#include <Arduino.h>

#include <Wire.h>

#include <ESPAsyncWebServer.h>
#include <ESP.h>
#include <SPIFFS.h>

#include "wifi_credentials.h"

#include <list>

void requestEvent();
void spiffsInfo();

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
  Serial.println("~~ USB Orbit Ducky ~~");

  Serial.begin(115200);

  Serial.println("SPIFFS:");
  if (!SPIFFS.begin(true))
  {
    Serial.println("\tError mounting SPIFFS");
    ESP.restart();
  }
  Serial.println("\tMounted SPIFFS");
  Serial.println();

  WiFi.softAP(ssid, password);
  delay(500);
  WiFi.softAPConfig(IP, gateway, subnet);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("WiFi:");
  Serial.print("\tSSID: ");
  Serial.println(ssid);

  Serial.print("\tPWD: ");
  Serial.println(password);

  Serial.print("\tAP IP address: ");
  Serial.println(IP);
  Serial.println();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false); });

  server.begin();

  Wire.begin(8);
  Wire.onRequest(requestEvent);

  spiffsInfo();
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

void spiffsInfo()
{
  uint32_t program_size = ESP.getSketchSize();

  uint32_t file_system_size = SPIFFS.totalBytes();

  uint32_t file_system_used = SPIFFS.usedBytes();

  uint32_t free_size = ESP.getFlashChipSize() - program_size - file_system_size + file_system_used;

  Serial.println("Memory:");
  Serial.print("\tProgram size: ");
  Serial.print(program_size);
  Serial.println(" bytes");

  Serial.print("File system size: ");
  Serial.print(file_system_size);
  Serial.println(" bytes");

  Serial.print("\tFile system used: ");
  Serial.print(file_system_used);
  Serial.println(" bytes");

  Serial.print("\tFree space: ");
  Serial.print(free_size);
  Serial.println(" bytes");
  Serial.println();
}