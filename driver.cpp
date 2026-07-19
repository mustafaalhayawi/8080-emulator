#include "driver.h"

std::unordered_map<std::string, Driver> driver_registry = {
    {
        "cpm_terminal",
        { 0, 0, true, true, nullptr }
    },
    {
        "test_graphics",
        { 320, 240, false, false, test_graphics}
    }
};

void test_graphics(const uint8_t* memory, uint32_t* host_buffer) {
    for (int i=0; i<320*240; i++) {
        host_buffer[i] = 0xFF000000 | (std::rand() % (2 << 24));
    }
}

const Driver* get_driver(std::string driver_name) {
    auto it = driver_registry.find(driver_name);

    if (it != driver_registry.end()) return &(it->second);

    return nullptr;
}

