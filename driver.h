#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include <cstdint>

struct Driver {
    int screen_width, screen_height;
    bool is_headless;
    bool is_throttled;

    void (*translate_video)(const uint8_t* memory, uint32_t* host_buffer);
};

const Driver* get_driver(std::string driver_name);

