#include "EventHandlers.h"

String EventHandlers::spiffsInfo()
{
    uint32_t program_size = ESP.getSketchSize();

    uint32_t file_system_size = SPIFFS.totalBytes();

    uint32_t file_system_used = SPIFFS.usedBytes();

    uint32_t free_size = ESP.getFlashChipSize() - program_size - file_system_size + file_system_used;

    u_int8_t promicro_online = (millis() - WireService::lastReq) >= 500 ? 0 : 1;

    return "{ \"file_system_size\":" + String(file_system_size) + ",\"file_system_used\":" + String(file_system_used) + ",\"atmega32U4\":" + String(promicro_online) + " }";
}

String EventHandlers::getScripts()
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

String EventHandlers::newScript(String file_name)
{
    String checkStat = Helpers::file_nameChecker(file_name);
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

String EventHandlers::deleteScript(String file_name)
{
    String checkStat = Helpers::file_nameChecker(file_name);
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

String EventHandlers::getScript(String file_name)
{
    String checkStat = Helpers::file_nameChecker(file_name);
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

    return "{ \"status\":200, \"content\":\"" + Helpers::stringToHex(content) + "\" }";
}

String EventHandlers::saveScript(String file_name, String new_file_name, String script)
{
    String checkStat = Helpers::file_nameChecker(new_file_name);
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

String EventHandlers::runScript(String file_name)
{
    String checkStat = Helpers::file_nameChecker(file_name);
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

    std::vector<String> lines;

    int startPos = 0;

    while (startPos < instruction.length())
    {
        int endPos = instruction.indexOf('\n', startPos);
        if (endPos == -1)
        {
            endPos = instruction.length();
        }
        lines.push_back(instruction.substring(startPos, endPos));
        startPos = endPos + 1;
    }

    for (size_t i = 0; i < lines.size(); i++)
    {
        if (!lines[i].startsWith("COM"))
        {
            ODSInterpreter::interpretLine(lines[i]);
        }
        Serial.println(lines[i]);
    }

    return "200";
}
String EventHandlers::updateSettings(AsyncWebServerRequest *request)
{
    try
    {
        String ssid = Helpers::hexToString(request->getParam("ssid")->value());
        String pwd = Helpers::hexToString(request->getParam("pwd")->value());

        uint8_t channel = request->getParam("channel")->value().toInt();
        uint8_t hidessid = request->getParam("hidessid")->value().toInt();
        uint8_t maxconnect = request->getParam("maxconnect")->value().toInt();
        uint8_t ftmrespond = request->getParam("ftmrespond")->value().toInt();

        uint8_t ip1 = request->getParam("ip1")->value().toInt();
        uint8_t ip2 = request->getParam("ip2")->value().toInt();
        uint8_t ip3 = request->getParam("ip3")->value().toInt();
        uint8_t ip4 = request->getParam("ip4")->value().toInt();

        uint8_t gateway1 = request->getParam("gateway1")->value().toInt();
        uint8_t gateway2 = request->getParam("gateway2")->value().toInt();
        uint8_t gateway3 = request->getParam("gateway3")->value().toInt();
        uint8_t gateway4 = request->getParam("gateway4")->value().toInt();

        uint8_t subnet1 = request->getParam("subnet1")->value().toInt();
        uint8_t subnet2 = request->getParam("subnet2")->value().toInt();
        uint8_t subnet3 = request->getParam("subnet3")->value().toInt();
        uint8_t subnet4 = request->getParam("subnet4")->value().toInt();

        ssid.toCharArray(WifiCredentials::ssid, 30);
        pwd.toCharArray(WifiCredentials::password, 15);

        WifiCredentials::channel = channel;
        WifiCredentials::ssid_hidden = hidessid;
        WifiCredentials::max_connection = maxconnect;
        WifiCredentials::ftm_responder = ftmrespond;

        WifiCredentials::IP = IPAddress(ip1, ip2, ip3, ip4);

        WifiCredentials::gateway = IPAddress(gateway1, gateway2, gateway3, gateway4);

        WifiCredentials::subnet = IPAddress(subnet1, subnet2, subnet3, subnet4);

        StaticJsonDocument<500> doc;

        doc["ssid"] = ssid.substring(0, 30);
        doc["pwd"] = pwd.substring(0, 15);

        doc["channel"] = channel;
        doc["hidessid"] = hidessid;
        doc["maxconnect"] = maxconnect;
        doc["ftmrespond"] = ftmrespond;

        doc["ip1"] = ip1;
        doc["ip2"] = ip2;
        doc["ip3"] = ip3;
        doc["ip4"] = ip4;

        doc["gateway1"] = gateway1;
        doc["gateway2"] = gateway2;
        doc["gateway3"] = gateway3;
        doc["gateway4"] = gateway4;

        doc["subnet1"] = subnet1;
        doc["subnet2"] = subnet2;
        doc["subnet3"] = subnet3;
        doc["subnet4"] = subnet4;

        ArduinoJson::serializeJsonPretty(doc, Serial);

        File file = SPIFFS.open("/website/wificredentials.json", FILE_WRITE);

        ArduinoJson::serializeJson(doc, file);

        file.close();
    }
    catch (const std::exception &e)
    {
        Serial.println(e.what());
        return e.what();
    }

    return "200";
}
