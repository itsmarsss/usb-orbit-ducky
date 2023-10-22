#include <Arduino.h>
#include <ESP.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

#include "WifiCredentials.h"

void mountSPIFFS()
{
    Serial.println("SPIFFS:");
    if (!SPIFFS.begin(true))
    {
        Serial.println("\tError mounting SPIFFS");
        ESP.restart();
    }
    Serial.println("\tMounted SPIFFS");
    Serial.println();
}

IPAddress IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
void setupAP()
{
    WiFi.softAP(ssid, password, channel, ssid_hidden, max_connection, ftm_responder);
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
}
