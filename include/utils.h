#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <switch.h>

// Map from HidControllerKeys -> int
#define KEY_MAP_SIZE 28
extern std::map<HidControllerKeys, int> key_map;

#endif