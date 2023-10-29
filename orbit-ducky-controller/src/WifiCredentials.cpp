#include "WifiCredentials.h"

char WifiCredentials::ssid[] = "USB Orbit Ducky";
char WifiCredentials::password[] = "12345678";
uint8_t WifiCredentials::channel = 1;
uint8_t WifiCredentials::ssid_hidden = 1;
uint8_t WifiCredentials::max_connection = 1;
boolean WifiCredentials::ftm_responder = true;

IPAddress WifiCredentials::IP(192, 168, 1, 1);
IPAddress WifiCredentials::gateway(192, 168, 1, 1);
IPAddress WifiCredentials::subnet(255, 255, 255, 0);

void WifiCredentials::loadWifiCredentials()
{
    File file = SPIFFS.open("/website/wificredentials.json", FILE_READ);

    DynamicJsonDocument doc(500);
    ArduinoJson::deserializeJson(doc, file);

    file.close();

    const char *ssid = doc["ssid"];
    const char *password = doc["pwd"];

    String(ssid).toCharArray(WifiCredentials::ssid, 30);
    String(password).toCharArray(WifiCredentials::password, 15);

    WifiCredentials::channel = doc["channel"];
    WifiCredentials::ssid_hidden = doc["hidessid"];
    WifiCredentials::max_connection = doc["maxconnect"];
    WifiCredentials::ftm_responder = doc["ftmrespond"];

    WifiCredentials::IP = IPAddress(doc["ip1"], doc["ip2"], doc["ip3"], doc["ip4"]);

    WifiCredentials::gateway = IPAddress(doc["gateway1"], doc["gateway2"], doc["gateway3"], doc["gateway4"]);

    WifiCredentials::subnet = IPAddress(doc["subnet1"], doc["subnet2"], doc["subnet3"], doc["subnet4"]);
}
