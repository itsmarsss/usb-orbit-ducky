#include "Helpers.h"

String Helpers::hexToString(String hex)
{
    String str = "";
    for (int i = 0; i < hex.length(); i += 2)
    {
        String hexByte = hex.substring(i, i + 2);

        byte value = (byte)strtol(hexByte.c_str(), NULL, 16);

        str += char(value);
    }
    return str;
}

String Helpers::stringToHex(String str)
{
    String hex = "";
    for (int i = 0; i < str.length(); i++)
    {
        char c = str.charAt(i);
        byte value = (byte)c;
        char hexChars[3];
        snprintf(hexChars, 3, "%02X", value);
        hex += hexChars;
    }
    return hex;
}

bool Helpers::isBlank(const String &str)
{
    for (char c : str)
    {
        if (!isWhitespace(c))
        {
            return false;
        }
    }
    return true;
}

bool Helpers::isWhitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '/' || c == '\\');
}

String Helpers::file_nameChecker(String file_name)
{
    if (isBlank(file_name))
    {
        return "'file_name' cannot be blank.";
    }

    if (file_name.length() > 30)
    {
        return "'file_name' length cannot exceed 30 characters.";
    }

    if (file_name.equals("/"))
    {
        return "Root file cannot be edited.";
    }

    return "200";
}
