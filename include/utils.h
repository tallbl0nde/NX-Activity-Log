#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <SDL2/SDL.h>
#include <switch.h>

// Map from HidControllerKeys -> int
#define KEY_MAP_SIZE 28
extern std::map<HidControllerKeys, int> key_map;

// Struct representing a "clock", which represents time between ticks
struct Clock {
    uint32_t last_tick = 0;
    uint32_t delta = 0;

    void tick() {
        uint32_t tick = SDL_GetTicks();
        delta = tick - last_tick;
        last_tick = tick;
    }
};

#endif