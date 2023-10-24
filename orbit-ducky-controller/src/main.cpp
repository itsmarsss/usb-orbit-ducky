#include <Arduino.h>

#include "SPIFFSService.h"
#include "WireService.h"
#include "APService.h"
#include "WebService.h"

#include "KeyMap.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("~~ USB Orbit Ducky ~~");

  KeyMap::keyMap["KEY_LEFT_CTRL"] = 128;
  KeyMap::keyMap["KEY_LEFT_SHIFT"] = 129;
  KeyMap::keyMap["KEY_LEFT_ALT"] = 130;
  KeyMap::keyMap["KEY_LEFT_GUI"] = 131;
  KeyMap::keyMap["KEY_RIGHT_CTRL"] = 132;
  KeyMap::keyMap["KEY_RIGHT_SHIFT"] = 133;
  KeyMap::keyMap["KEY_RIGHT_ALT"] = 134;
  KeyMap::keyMap["KEY_RIGHT_GUI"] = 135;

  KeyMap::keyMap["KEY_TAB"] = 179;
  KeyMap::keyMap["KEY_CAPS_LOCK"] = 193;
  KeyMap::keyMap["KEY_BACKSPACE"] = 178;
  KeyMap::keyMap["KEY_RETURN"] = 176;
  KeyMap::keyMap["KEY_MENU"] = 237;

  KeyMap::keyMap["KEY_INSERT"] = 209;
  KeyMap::keyMap["KEY_DELETE"] = 212;
  KeyMap::keyMap["KEY_HOME"] = 210;
  KeyMap::keyMap["KEY_END"] = 213;
  KeyMap::keyMap["KEY_PAGE_UP"] = 211;
  KeyMap::keyMap["KEY_PAGE_DOWN"] = 214;
  KeyMap::keyMap["KEY_UP_ARROW"] = 218;
  KeyMap::keyMap["KEY_DOWN_ARROW"] = 217;
  KeyMap::keyMap["KEY_LEFT_ARROW"] = 216;
  KeyMap::keyMap["KEY_RIGHT_ARROW"] = 215;

  KeyMap::keyMap["KEY_ESC"] = 177;
  KeyMap::keyMap["KEY_F1"] = 194;
  KeyMap::keyMap["KEY_F2"] = 195;
  KeyMap::keyMap["KEY_F3"] = 196;
  KeyMap::keyMap["KEY_F4"] = 197;
  KeyMap::keyMap["KEY_F5"] = 198;
  KeyMap::keyMap["KEY_F6"] = 199;
  KeyMap::keyMap["KEY_F7"] = 200;
  KeyMap::keyMap["KEY_F8"] = 201;
  KeyMap::keyMap["KEY_F9"] = 202;
  KeyMap::keyMap["KEY_F10"] = 203;
  KeyMap::keyMap["KEY_F11"] = 204;
  KeyMap::keyMap["KEY_F12"] = 205;
  KeyMap::keyMap["KEY_F13"] = 240;
  KeyMap::keyMap["KEY_F14"] = 241;
  KeyMap::keyMap["KEY_F15"] = 242;
  KeyMap::keyMap["KEY_F16"] = 243;
  KeyMap::keyMap["KEY_F17"] = 244;
  KeyMap::keyMap["KEY_F18"] = 245;
  KeyMap::keyMap["KEY_F19"] = 246;
  KeyMap::keyMap["KEY_F20"] = 247;
  KeyMap::keyMap["KEY_F21"] = 248;
  KeyMap::keyMap["KEY_F22"] = 249;
  KeyMap::keyMap["KEY_F23"] = 250;
  KeyMap::keyMap["KEY_F24"] = 251;

  KeyMap::keyMap["KEY_PRINT_SCREEN"] = 206;
  KeyMap::keyMap["KEY_SCROLL_LOCK"] = 207;
  KeyMap::keyMap["KEY_PAUSE"] = 208;

  delay(3000);

  SPIFFSService::setupSPIFFS();

  WireService::setupWireService();

  Serial.println(EventHandlers::runScript("/AUTORUN"));

  APService::setupAP();

  WebServer::setupServer();
}

void loop()
{
  delay(100);
}
