#include "driver.h"

std::unordered_map<std::string, Driver> driver_registry = {
    {
        "cpm_terminal",
        { 0, 0, true, true, nullptr }
    }
};

const Driver* get_driver(std::string driver_name) {
    auto it = driver_registry.find(driver_name);

    if (it != driver_registry.end()) return &(it->second);

    return nullptr;
}

