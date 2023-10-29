#ifndef EVENTHANDLERS_H
#define EVENTHANDLERS_H

#include <Arduino.h>

#include <ESPAsyncWebServer.h>
#include <ESP.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#include <vector>

#include "Helpers.h"
#include "WireService.h"
#include "ODSInterpreter.h"
#include "WifiCredentials.h"

class EventHandlers
{
public:
    static String spiffsInfo();
    static String getScripts();
    static String newScript(String file_name);
    static String deleteScript(String file_name);
    static String getScript(String file_name);
    static String saveScript(String file_name, String new_file_name, String script);
    static String runScript(String file_name);
    static String updateSettings(AsyncWebServerRequest *request);
};

#endif