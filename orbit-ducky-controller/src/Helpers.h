#include <Arduino.h>
#include <memory>
#include <string>
#include <stdexcept>

template <typename... Args>
std::string string_format(const std::string &format, Args... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if (size_s <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

String hexToString(String hex)
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

String stringToHex(String str)
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

bool isBlank(const String &str)
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

bool isWhitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '/' || c == '\\');
}

String file_nameChecker(String file_name)
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
