#ifndef KEYMAP_H
#define KEYMAP_H

#include <unordered_map>

class KeyMap
{
public:
    static std::unordered_map<std::string, int> keyMap;
};

#endif
