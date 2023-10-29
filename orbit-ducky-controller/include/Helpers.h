#ifndef HELPERS_H
#define HELPERS_H

#include <Arduino.h>

class Helpers
{
public:
    static String hexToString(String hex);
    static String stringToHex(String str);

    static bool isBlank(const String &str);
    static bool isWhitespace(char c);
    static String file_nameChecker(String file_name);
};

#endif
