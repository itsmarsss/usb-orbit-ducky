#include <Arduino.h>

#include <ESP.h>
#include <SPIFFS.h>

#include <vector>

#include "Helpers.h"
#include "ODSInterpreter.h"

String spiffsInfo();
String scripts();
String newScript(String file_name);
String deleteScript(String file_name);
String getScript(String file_name);
String saveScript(String file_name, String new_file_name, String script);
String runScript(String file_name);
