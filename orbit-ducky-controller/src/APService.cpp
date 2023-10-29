#include "APService.h"

void APService::setupAP()
{
    WifiCredentials::loadWifiCredentials();

    WiFi.softAP(WifiCredentials::ssid,
                WifiCredentials::password,
                WifiCredentials::channel,
                WifiCredentials::ssid_hidden,
                WifiCredentials::max_connection,
                WifiCredentials::ftm_responder);
    delay(500);
    WiFi.softAPConfig(WifiCredentials::IP,
                      WifiCredentials::gateway,
                      WifiCredentials::subnet);
    IPAddress IP = WiFi.softAPIP();
    Serial.println("WiFi:");
    Serial.print("\tSSID: ");
    Serial.println(WifiCredentials::ssid);

    Serial.print("\tPWD: ");
    Serial.println(WifiCredentials::password);

    Serial.print("\tAP IP address: ");
    Serial.println(IP);
    Serial.println();
}