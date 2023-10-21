#include <Arduino.h>

#include <Wire.h>

#include <ESPAsyncWebServer.h>
#include <ESP.h>
#include <SPIFFS.h>

#include "wifi_credentials.h"
#include "string_format.h"

#include <list>

void requestEvent();
String hexToString(String);

String spiffsInfo();
String scripts();
String newScript(String);
String deleteScript(String);
String getScript(String);
String saveScript(String, String);
String runScript(String);

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
            { request->send(200, "text/html", spiffsInfo()); });

  server.on("/api/scripts", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", scripts()); });

  server.on("/api/new_script", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if (request->hasParam("file_name")) {
                String file_name = request->getParam("file_name")->value();

                file_name.trim();
                file_name.replace("/", "");
                file_name.replace("\\", "");

                request->send(200, "text/html", newScript(file_name));
              }
              request->send(200, "text/html", "No 'file_name' provided."); });

  server.on("/api/delete_script", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if (request->hasParam("file_name")) {
                String file_name = request->getParam("file_name")->value();

                request->send(200, "text/html", deleteScript(file_name));
              }
              request->send(200, "text/html", "No 'file_name' provided."); });

  server.on("/api/get_script", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if (request->hasParam("file_name")) {
                String file_name = request->getParam("file_name")->value();

                request->send(200, "text/html", getScript(file_name));
              }
              request->send(200, "text/html", "No 'file_name' provided."); });

  server.on("/api/save_script", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (!(request->hasParam("file_name")))
              {
                request->send(200, "text/html", "No 'file_name' provided.");
                return;
              }
              if (!(request->hasParam("script")))
              {
                request->send(200, "text/html", "No 'script' provided.");
                return;
              }

              String file_name = request->getParam("file_name")->value();

              file_name.trim();
              file_name.replace("/", "");
              file_name.replace("\\", "");

              String script = request->getParam("script")->value();
              request->send(200, "text/html", saveScript(file_name, hexToString(script))); });

  server.on("/api/run_script", HTTP_GET, [](AsyncWebServerRequest *request)
            {
            if (request->hasParam("file_name"))
            {
              String file_name = request->getParam("file_name")->value();

              request->send(200, "text/html", runScript(file_name));
            }
            request->send(200, "text/html", "No 'file_name' provided."); });

  server.on("/api/download_script", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
            if (!(request->hasParam("file_name"))) {
              request->send(200, "text/html", "No 'file_name' provided.");
              return;
            }

            String file_name = request->getParam("file_name")->value();

            if (!SPIFFS.exists("/" + file_name))
            {
              request->send(200, "text/html", "File with name '" + file_name + "' doesn't exists.");
              return;
            }

            File download = SPIFFS.open("/" + file_name, FILE_READ);

            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", download.readString());
            
            response->addHeader("Content-Type", "text/text");
            response->addHeader("Content-Disposition", "attachment; filename=" + file_name + ".ods");
            download.close();

            request->send(response); });

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

String hexToString(String hex)
{
  String str = "";
  for (int i = 0; i < hex.length(); i += 2)
  {
    String hexByte = hex.substring(i, i + 2);

    byte value = (byte)strtol(hexByte.c_str(), NULL, 16);

    str += char(value);
  }
  return str;
}

String spiffsInfo()
{
  uint32_t program_size = ESP.getSketchSize();

  uint32_t file_system_size = SPIFFS.totalBytes();

  uint32_t file_system_used = SPIFFS.usedBytes();

  uint32_t free_size = ESP.getFlashChipSize() - program_size - file_system_size + file_system_used;

  u_int8_t promicro_online = (millis() - last_req) >= 500 ? 0 : 1;

  return string_format("{ \"file_system_size\":%u,\"file_system_used\":%u,\"atmega32U4\":%i }", file_system_size, file_system_used, promicro_online).c_str();
}

String scripts()
{
  File root = SPIFFS.open("/");

  File file = root.openNextFile();

  std::string scripts_json;

  while (file)
  {
    Serial.println(file.name());

    if (!String(file.name()).equals("index.html"))
    {
      scripts_json += string_format("{ \"name\":\"%s\",\"bytes\":%s },", String(file.name()), String(file.size()));
    }
    file = root.openNextFile();
  }

  if (scripts_json.length() != 0)
  {
    scripts_json.resize(scripts_json.size() - 1);
  }

  return string_format("{ \"scripts\":[%s] } ", scripts_json.c_str()).c_str();
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
  return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '/' || c == '\\');
}

String file_nameChecker(String file_name)
{
  if (isBlank(file_name))
  {
    return "'file_name' cannot be blank.";
  }

  if (file_name.length() > 30)
  {
    return "'file_name' length cannot exceed 30 characters.";
  }

  return "200";
}

String newScript(String file_name)
{
  String checkStat = file_nameChecker(file_name);
  if (!checkStat.equals("200"))
  {
    return checkStat;
  }

  if (SPIFFS.exists("/" + file_name))
  {
    return "File with name '" + file_name + "' already exists.";
  }

  File file = SPIFFS.open("/" + file_name, FILE_WRITE);
  file.print("COM This is a new Orbit Ducky Script file");
  file.close();

  return "200";
}

String deleteScript(String file_name)
{
  String checkStat = file_nameChecker(file_name);
  if (!checkStat.equals("200"))
  {
    return checkStat;
  }

  File file = SPIFFS.open("/" + file_name);

  if (!SPIFFS.exists("/" + file_name))
  {
    return "File with name '" + file_name + "' doesn't exists.";
  }

  if (SPIFFS.remove("/" + file_name))
  {
    return "200";
  }

  return "Error deleting file '" + file_name + "'.";
}

String getScript(String file_name)
{
  String checkStat = file_nameChecker(file_name);
  if (!checkStat.equals("200"))
  {
    return checkStat;
  }

  if (!SPIFFS.exists("/" + file_name))
  {
    return "File with name '" + file_name + "' doesn't exists.";
  }

  File file = SPIFFS.open("/" + file_name, FILE_READ);
  String content = file.readString();
  file.close();

  return content;
}

String saveScript(String file_name, String script)
{
  String checkStat = file_nameChecker(file_name);
  if (!checkStat.equals("200"))
  {
    return checkStat;
  }

  File file = SPIFFS.open("/" + file_name, FILE_WRITE);
  file.print(script);
  file.close();

  return "200";
}

String runScript(String file_name)
{
  String checkStat = file_nameChecker(file_name);
  if (!checkStat.equals("200"))
  {
    return checkStat;
  }

  if (!SPIFFS.exists("/" + file_name))
  {
    return "File with name '" + file_name + "' doesn't exists.";
  }

  /*
  Run Script
    - Append to 'decimalStream'
  */

  return "200";
}