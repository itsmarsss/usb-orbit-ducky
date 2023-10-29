#ifndef WIFICREDENTIALS_H
#define WIFICREDENTIALS_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

class WifiCredentials
{
public:
    static char ssid[];
    static char password[];
    static uint8_t channel;
    static uint8_t ssid_hidden;
    static uint8_t max_connection;
    static boolean ftm_responder;

    static IPAddress IP;
    static IPAddress gateway;
    static IPAddress subnet;

    static void loadWifiCredentials();
};

#endif
