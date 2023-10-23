#include "APService.h"

IPAddress IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
void APService::setupAP()
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