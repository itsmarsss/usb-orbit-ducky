#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include "EventHandlers.h"
#include "Helpers.h"

class WebServer
{
public:
    static void setupServer();
};

#endif
