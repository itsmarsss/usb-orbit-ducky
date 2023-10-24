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