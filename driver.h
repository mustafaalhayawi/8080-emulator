#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include <cstdint>
#include <SDL3/SDL.h>
#include <random> // temp

struct Driver {
    int screen_width, screen_height;
    bool is_headless;
    bool is_throttled;

    void (*translate_video)(const uint8_t* memory, uint32_t* host_buffer);
};

void test_graphics(const uint8_t* memory, uint32_t* host_buffer);

const Driver* get_driver(std::string driver_name);

