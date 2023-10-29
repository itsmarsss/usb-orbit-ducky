#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <ESPAsyncWebServer.h>
#include <ESP.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#include "EventHandlers.h"
#include "Helpers.h"
#include "WifiCredentials.h"

class WebServer
{
public:
    static void setupServer();
};

#endif
