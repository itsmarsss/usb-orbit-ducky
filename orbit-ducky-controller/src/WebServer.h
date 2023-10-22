#include <ESPAsyncWebServer.h>

#include <SPIFFS.h>

AsyncWebServer server(80);

void setupEndPoints();

String spiffsInfo();
String scripts();
String newScript(String);
String deleteScript(String);
String getScript(String);
String saveScript(String, String, String);
String runScript(String);

String hexToString(String);

void setupServer()
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

  server.on("/css/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/website/css/style.css", String(), false); });

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", spiffsInfo()); });

  server.on("/api/scripts", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", scripts()); });

  server.on("/api/new_script", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (request -> hasParam("file_name")) {
      String file_name = hexToString(request -> getParam("file_name") -> value());

      file_name.trim();

      request -> send(200, "text/html", newScript(file_name));
    }
    request -> send(200, "text/html", "No 'file_name' provided."); });

  server.on("/api/delete_script", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (request -> hasParam("file_name")) {
      String file_name = hexToString(request -> getParam("file_name") -> value());

      request -> send(200, "text/html", deleteScript(file_name));
    }
    request -> send(200, "text/html", "No 'file_name' provided."); });

  server.on("/api/get_script", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (request -> hasParam("file_name")) {
      String file_name = hexToString(request -> getParam("file_name") -> value());

      request -> send(200, "text/html", getScript(file_name));
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

    String file_name = hexToString(request -> getParam("file_name") -> value());

    file_name.trim();

    String new_file_name = file_name;

    if (request -> hasParam("new_file_name")) {
      new_file_name = hexToString(request -> getParam("new_file_name") -> value());
    }

    String script = hexToString(request -> getParam("script") -> value());

    request -> send(200, "text/html", saveScript(file_name, new_file_name, script)); });

  server.on("/api/run_script", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (request -> hasParam("file_name")) {
      String file_name = hexToString(request -> getParam("file_name") -> value());

      request -> send(200, "text/html", runScript(file_name));
    }
    request -> send(200, "text/html", "No 'file_name' provided."); });

  server.on("/api/download_script", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!(request -> hasParam("file_name"))) {
      request -> send(200, "text/html", "No 'file_name' provided.");
      return;
    }

    String file_name = hexToString(request -> getParam("file_name") -> value());

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
}