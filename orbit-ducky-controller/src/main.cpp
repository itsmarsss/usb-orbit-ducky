#include <Arduino.h>

#include <Wire.h>

#include <ESPAsyncWebServer.h>
#include <ESP.h>
#include <SPIFFS.h>

#include "wifi_credentials.h"
#include "string_format.h"

#include <list>

void requestEvent();
std::string spiffsInfo();
std::string scripts();
bool new_script(String);
bool delete_script(String);

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}
IPAddress IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

std::list<byte> decimalStream;
uint32_t last_req = millis();

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

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", spiffsInfo().c_str()); });

  server.on("/api/scripts", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", scripts().c_str()); });

  server.on("/api/new_script", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if (request->hasParam("file_name")) {
                String file_name = request->getParam("file_name")->value();

                if (new_script(file_name)) {
                  request->send(200, "text/html", "200");
                }
              }
              request->send(200, "text/html", "No 'file_name' provided."); });

  server.on("/api/delete_script", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if (request->hasParam("file_name")) {
                String file_name = request->getParam("file_name")->value();

                if(delete_script(file_name)) {
                  request->send(200, "text/html", "200");
                }
              }
              request->send(200, "text/html", "No 'file_name' provided."); });

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

  last_req = millis();
}

std::string spiffsInfo()
{
  uint32_t program_size = ESP.getSketchSize();

  uint32_t file_system_size = SPIFFS.totalBytes();

  uint32_t file_system_used = SPIFFS.usedBytes();

  uint32_t free_size = ESP.getFlashChipSize() - program_size - file_system_size + file_system_used;

  u_int8_t promicro_online = (millis() - last_req) >= 500 ? 0 : 1;

  return string_format("{ \"file_system_size\":%u,\"file_system_used\":%u,\"atmega32U4\":%i }", file_system_size, file_system_used, promicro_online);
}

std::string scripts()
{
  File root = SPIFFS.open("/");

  File file = root.openNextFile();

  std::string scripts_json;

  while (file)
  {
    Serial.println(file.name());

    // if (strcmp(file.name(), "index.html") != 0)
    {
      scripts_json += string_format("{ \"name\":\"%s\",\"bytes\":%s },", String(file.name()), String(file.size()));
    }
    file = root.openNextFile();
  }

  if (scripts_json.length() != 0)
  {
    scripts_json.resize(scripts_json.size() - 1);
  }

  return string_format("{ \"scripts\":[%s] } ", scripts_json.c_str());
}

bool isBlank(const String &str)
{
  for (char c : str)
  {
    if (!isWhitespace(c))
    {
      return false;
    }
  }
  return true;
}

bool isWhitespace(char c)
{
  return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

bool new_script(String file_name)
{
  if (isBlank(file_name))
  {
    return false;
  }

  File root = SPIFFS.open("/");

  File file = root.openNextFile();

  while (file)
  {
    if (strcmp(file.name(), file_name.c_str()) == 0)
    {
      return false;
    }
    file = root.openNextFile();
  }

  File new_file = SPIFFS.open("/" + file_name, FILE_WRITE);
  new_file.print("COM This is a new Orbit Ducky Script file");
  new_file.close();

  return true;
}

bool delete_script(String file_name)
{
  if (isBlank(file_name))
  {
    return false;
  }

  return SPIFFS.remove("/" + file_name);
}