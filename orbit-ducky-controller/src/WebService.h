#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include "EventHandlers.h"

class WebServer
{
public:
    static void setupServer();

private:
    void setupEndPoints();
};
