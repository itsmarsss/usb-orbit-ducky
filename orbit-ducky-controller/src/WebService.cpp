#include "WebService.h"

void setupEndPoints();

AsyncWebServer server(80);
void WebServer::setupServer()
{
  setupEndPoints();
  server.begin();
}

void setupEndPoints()
{
  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "text/plain", "Not found"); });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/website/html/index.html", String(), false); });

  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/website/html/settings.html", String(), false); });

  server.on("/css/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/website/css/style.css", String(), false); });

  server.on("/css/index.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/website/css/index.css", String(), false); });

  server.on("/css/settings.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/website/css/settings.css", String(), false); });

  server.on("/js/tools.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/website/js/tools.js", String(), false); });

  server.on("/js/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/website/js/index.js", String(), false); });

  server.on("/js/settings.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/website/js/settings.js", String(), false); });

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", EventHandlers::spiffsInfo()); });

  server.on("/api/scripts", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", EventHandlers::getScripts()); });

  server.on("/api/new_script", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (request -> hasParam("file_name")) {
      String file_name = Helpers::hexToString(request -> getParam("file_name") -> value());

      file_name.trim();

      request -> send(200, "text/html", EventHandlers::newScript(file_name));
    }
    request -> send(200, "text/html", "No 'file_name' provided."); });

  server.on("/api/delete_script", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (request -> hasParam("file_name")) {
      String file_name = Helpers::hexToString(request -> getParam("file_name") -> value());

      request -> send(200, "text/html", EventHandlers::deleteScript(file_name));
    }
    request -> send(200, "text/html", "No 'file_name' provided."); });

  server.on("/api/get_script", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (request -> hasParam("file_name")) {
      String file_name = Helpers::hexToString(request -> getParam("file_name") -> value());

      request -> send(200, "text/html", EventHandlers::getScript(file_name));
    }
    request -> send(200, "text/html", "No 'file_name' provided."); });

  server.on("/api/save_script", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!(request -> hasParam("file_name"))) {
      request -> send(200, "text/html", "No 'file_name' provided.");
      return;
    }
    if (!(request -> hasParam("script"))) {
      request -> send(200, "text/html", "No 'script' provided.");
      return;
    }

    String file_name = Helpers::hexToString(request -> getParam("file_name") -> value());

    file_name.trim();

    String new_file_name = file_name;

    if (request -> hasParam("new_file_name")) {
      new_file_name = Helpers::hexToString(request -> getParam("new_file_name") -> value());
    }

    String script = Helpers::hexToString(request -> getParam("script") -> value());

    request -> send(200, "text/html", EventHandlers::saveScript(file_name, new_file_name, script)); });

  server.on("/api/run_script", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (request -> hasParam("file_name")) {
      String file_name = Helpers::hexToString(request -> getParam("file_name") -> value());

      request -> send(200, "text/html", EventHandlers::runScript(file_name));
    }
    request -> send(200, "text/html", "No 'file_name' provided."); });

  server.on("/api/download_script", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!(request -> hasParam("file_name"))) {
      request -> send(200, "text/html", "No 'file_name' provided.");
      return;
    }

    String file_name = Helpers::hexToString(request -> getParam("file_name") -> value());

    if (!SPIFFS.exists("/" + file_name)) {
      request -> send(200, "text/html", "File with name '" + file_name + "' doesn't exists.");
      return;
    }

    File download = SPIFFS.open("/" + file_name, FILE_READ);

    AsyncWebServerResponse * response = request -> beginResponse(200, "text/html", download.readString());

    response -> addHeader("Content-Type", "text/text");
    response -> addHeader("Content-Disposition", "attachment; filename=" + file_name + ".ods");
    download.close();

    request -> send(response); });

  server.on("/api/update_settings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!(request -> hasParam("ssid"))) {
      request -> send(200, "text/html", "SSID may not be empty.");
      return;
    }
    if (!(request -> hasParam("pwd"))) {
      request -> send(200, "text/html", "PWD may not be empty.");
      return;
    }
    if (!(request -> hasParam("channel"))) {
      request -> send(200, "text/html", "Channel may not be empty.");
      return;
    }
    if (!(request -> hasParam("hidessid"))) {
      request -> send(200, "text/html", "Hidden SSID may not be empty.");
      return;
    }
    if (!(request -> hasParam("maxconnect"))) {
      request -> send(200, "text/html", "Max Connections may not be empty.");
      return;
    }
    if (!(request -> hasParam("ftmrespond"))) {
      request -> send(200, "text/html", "FTM Responder may not be empty.");
      return;
    }
    if (!(request -> hasParam("ip1"))) {
      request -> send(200, "text/html", "IP may not be incomplete.");
      return;
    }
    if (!(request -> hasParam("ip2"))) {
      request -> send(200, "text/html", "IP may not be incomplete.");
      return;
    }
    if (!(request -> hasParam("ip3"))) {
      request -> send(200, "text/html", "IP may not be incomplete.");
      return;
    }
    if (!(request -> hasParam("ip4"))) {
      request -> send(200, "text/html", "IP may not be incomplete.");
      return;
    }
    if (!(request -> hasParam("gateway1"))) {
      request -> send(200, "text/html", "Gateway may not be incomplete.");
      return;
    }
    if (!(request -> hasParam("gateway2"))) {
      request -> send(200, "text/html", "Gateway may not be incomplete.");
      return;
    }
    if (!(request -> hasParam("gateway3"))) {
      request -> send(200, "text/html", "Gateway may not be incomplete.");
      return;
    }
    if (!(request -> hasParam("gateway4"))) {
      request -> send(200, "text/html", "Gateway may not be incomplete.");
      return;
    }
    if (!(request -> hasParam("subnet1"))) {
      request -> send(200, "text/html", "Subnet may not be incomplete.");
      return;
    }
    if (!(request -> hasParam("subnet2"))) {
      request -> send(200, "text/html", "Subnet may not be incomplete.");
      return;
    }
    if (!(request -> hasParam("subnet3"))) {
      request -> send(200, "text/html", "Subnet may not be incomplete.");
      return;
    }
    if (!(request -> hasParam("subnet4"))) {
      request -> send(200, "text/html", "Subnet may not be incomplete.");
      return;
    }

    try
    {
      String ssid = Helpers::hexToString(request -> getParam("ssid") -> value());
      String pwd = Helpers::hexToString(request -> getParam("pwd") -> value());
      
      uint8_t channel = request -> getParam("channel") -> value().toInt();
      uint8_t hidessid = request -> getParam("hidessid") -> value().toInt();
      uint8_t maxconnect = request -> getParam("maxconnect") -> value().toInt();
      uint8_t ftmrespond = request -> getParam("ftmrespond") -> value().toInt();
      
      uint8_t ip1 = request -> getParam("ip1") -> value().toInt();
      uint8_t ip2 = request -> getParam("ip2") -> value().toInt();
      uint8_t ip3 = request -> getParam("ip3") -> value().toInt();
      uint8_t ip4 = request -> getParam("ip4") -> value().toInt();
      
      uint8_t gateway1 = request -> getParam("gateway1") -> value().toInt();
      uint8_t gateway2 = request -> getParam("gateway2") -> value().toInt();
      uint8_t gateway3 = request -> getParam("gateway3") -> value().toInt();
      uint8_t gateway4 = request -> getParam("gateway4") -> value().toInt();
      
      uint8_t subnet1 = request -> getParam("subnet1") -> value().toInt();
      uint8_t subnet2 = request -> getParam("subnet2") -> value().toInt();
      uint8_t subnet3 = request -> getParam("subnet3") -> value().toInt();
      uint8_t subnet4 = request -> getParam("subnet4") -> value().toInt();

      ssid.toCharArray(WifiCredentials::ssid, 30);
      pwd.toCharArray(WifiCredentials::password, 15);

      WifiCredentials::channel = channel;
      WifiCredentials::ssid_hidden = hidessid;
      WifiCredentials::max_connection = maxconnect;
      WifiCredentials::ftm_responder = ftmrespond;
      
      WifiCredentials::IP = IPAddress(ip1, ip2, ip3, ip4);
      
      WifiCredentials::gateway = IPAddress(gateway1, gateway2, gateway3, gateway4);
      
      WifiCredentials::subnet = IPAddress(subnet1, subnet2, subnet3, subnet4);

      Serial.println(WifiCredentials::ssid);
      Serial.println(WifiCredentials::password);
      
      Serial.println(WifiCredentials::channel);
      Serial.println(WifiCredentials::ssid_hidden);
      Serial.println(WifiCredentials::max_connection);
      Serial.println(WifiCredentials::ftm_responder);
      
      Serial.println(WifiCredentials::IP);
      Serial.println(WifiCredentials::gateway);
      Serial.println(WifiCredentials::subnet);

      StaticJsonDocument<500> doc;

      doc["ssid"] = ssid.substring(0, 30);
      doc["pwd"] = pwd.substring(0, 15);
      
      doc["channel"] = channel;
      doc["hidessid"] = hidessid;
      doc["maxconnect"] = maxconnect;
      doc["ftmrespond"] = ftmrespond;
      
      doc["ip1"] = ip1;
      doc["ip2"] = ip2;
      doc["ip3"] = ip3;
      doc["ip4"] = ip4;
      
      doc["gateway1"] = gateway1;
      doc["gateway2"] = gateway2;
      doc["gateway3"] = gateway3;
      doc["gateway4"] = gateway4;
      
      doc["subnet1"] = subnet1;
      doc["subnet2"] = subnet2;
      doc["subnet3"] = subnet3;
      doc["subnet4"] = subnet4;

      serializeJsonPretty(doc, Serial);
    }
    catch (const std::exception &e)
    {
        Serial.println(e.what());
        request -> send(200, "text/html", e.what());
    }

    request -> send(200, "text/html", "200"); });

  server.on("/api/restart", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    request->send(200, "text/html", "ok.");
    Serial.println("Restarting...");
    ESP.restart(); });

  server.on("/api/shutdown", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    request->send(200, "text/html", "ok.");
    Serial.println("Shutting Down...");
    ESP.deepSleep(999999999*999999999U); }); // Please no...
}