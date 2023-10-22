#include <Arduino.h>

#include <ESP.h>
#include <SPIFFS.h>

#include <list>

#include "Helpers.h"
#include "ODSInterpreter.h"

uint32_t last_req = millis();
std::list<byte *> decimalStream;

String spiffsInfo()
{
    uint32_t program_size = ESP.getSketchSize();

    uint32_t file_system_size = SPIFFS.totalBytes();

    uint32_t file_system_used = SPIFFS.usedBytes();

    uint32_t free_size = ESP.getFlashChipSize() - program_size - file_system_size + file_system_used;

    u_int8_t promicro_online = (millis() - last_req) >= 500 ? 0 : 1;

    return string_format("{ \"file_system_size\":%u,\"file_system_used\":%u,\"atmega32U4\":%i }", file_system_size, file_system_used, promicro_online).c_str();
}

String scripts()
{
    File root = SPIFFS.open("/");

    File file = root.openNextFile();

    String scripts_json;

    while (file)
    {
        Serial.println(file.path());

        if (!String(file.path()).startsWith("/website/"))
        {
            scripts_json += "{ \"name\":\"" + String(file.path()) + "\",\"bytes\":" + String(file.size()) + " },";
        }
        file = root.openNextFile();
    }

    if (scripts_json.length() != 0)
    {
        scripts_json = scripts_json.substring(0, scripts_json.length() - 1);
    }

    return "{ \"scripts\":[" + scripts_json + "] }";
}

String newScript(String file_name)
{
    String checkStat = file_nameChecker(file_name);
    if (!checkStat.equals("200"))
    {
        return checkStat;
    }

    if (SPIFFS.exists(file_name))
    {
        return "File with path '" + file_name + "' already exists.";
    }

    File file = SPIFFS.open(file_name, FILE_WRITE);
    file.print("COM This is a new Orbit Ducky Script file");
    file.close();

    return "200";
}

String deleteScript(String file_name)
{
    String checkStat = file_nameChecker(file_name);
    if (!checkStat.equals("200"))
    {
        return checkStat;
    }

    File file = SPIFFS.open(file_name);

    if (!SPIFFS.exists(file_name))
    {
        return "File with path '" + file_name + "' doesn't exists.";
    }

    if (SPIFFS.remove(file_name))
    {
        return "200";
    }

    return "Error deleting file '" + file_name + "'.";
}

String getScript(String file_name)
{
    String checkStat = file_nameChecker(file_name);
    if (!checkStat.equals("200"))
    {
        return "{ \"status\":404, \"content\":\"" + checkStat + "\" }";
    }

    if (!SPIFFS.exists(file_name))
    {
        return "{ \"status\":404, \"content\":\"File with path '" + file_name + "' doesn't exists.\" }";
    }

    File file = SPIFFS.open(file_name, FILE_READ);
    String content = file.readString();
    file.close();

    return "{ \"status\":200, \"content\":\"" + stringToHex(content) + "\" }";
}

String saveScript(String file_name, String new_file_name, String script)
{
    String checkStat = file_nameChecker(new_file_name);
    if (!checkStat.equals("200"))
    {
        return checkStat;
    }

    if (SPIFFS.exists(new_file_name) && !file_name.equals(new_file_name))
    {
        return "File with path '" + new_file_name + "' already exists.";
    }

    SPIFFS.rename(file_name, new_file_name);

    File file = SPIFFS.open(new_file_name, FILE_WRITE);
    file.print(script);
    file.close();

    return "200";
}

String runScript(String file_name)
{
    String checkStat = file_nameChecker(file_name);
    if (!checkStat.equals("200"))
    {
        return checkStat;
    }

    if (!SPIFFS.exists(file_name))
    {
        return "File with path '" + file_name + "' doesn't exists.";
    }

    /*
    Run Script
      - Append to 'decimalStream'
    */

    File file = SPIFFS.open(file_name, FILE_READ);
    String instruction = file.readString();

    std::vector<String> lines; // Use a vector to dynamically store the lines

    int startPos = 0; // Initialize the start position

    while (startPos < instruction.length())
    {
        int endPos = instruction.indexOf('\n', startPos); // Find the next newline character
        if (endPos == -1)
        {
            endPos = instruction.length(); // If no more newline characters, use the end of the string
        }
        lines.push_back(instruction.substring(startPos, endPos)); // Store the line
        startPos = endPos + 1;                                    // Update the start position for the next iteration
    }

    // Now, you have the lines stored in the "lines" vector.

    // Print the lines for demonstration
    for (size_t i = 0; i < lines.size(); i++)
    {
        Serial.println(lines[i]);
    }

    return "200";
}
