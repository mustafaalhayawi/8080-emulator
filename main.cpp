#include "disassembler.h"
#include "cpu.h"
#include "file.h"
#include "driver.h"
#include "graphics.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <string_view>
#include <unordered_map>

int main(int argc, char* argv[]) {
    std::string window_name = "8080 Emulator";
    std::string file_name = "8080EXM.COM";

    const Driver* driver = get_driver("cpm_terminal");
    for (int i=0; i<argc; i++) {
        if (std::string_view(argv[i]) == "--driver" && i + 1 < argc) {
            std::string driver_name{argv[++i]};
            driver = get_driver(argv[i]);
            if (driver == nullptr) {
                std::cerr << "Error: Driver '" << driver_name << "' not found\n";
                return 1;
            }
        }
    }

    State state;
    state.memory = new uint8_t[65536]();
    load_rom("8080EXM.COM", state.memory + 0x0100);
    state.pc = 0x0100;

    Renderer renderer;
    std::vector<uint32_t> host_buffer;

    if (!driver->is_headless) {
        if (!renderer.init(driver, window_name)) {
            SDL_Log("Unable to initalise program!\n");
            delete[] state.memory;
            return 1;
        }

        host_buffer.resize(driver->screen_width * driver->screen_height, 0);
    }


    auto frame_start = std::chrono::high_resolution_clock::now();
    bool running = true;

    while (running) {
        if (state.pc == 0x0000) {
            running = false;
        } else if (state.pc == 0x005) {
            if (state.c == 0x09) {
                uint16_t ptr = state.de;
                while (ptr < 0x10000 && state.memory[ptr] != '$') {
                    std::cout << static_cast<char>(state.memory[ptr]) << std::flush;
                    ptr++;
                }
            } else if (state.c == 0x02) {
                std::cout << (char)state.e << std::flush;
            }
            state.pc = (state.memory[state.sp+1] << 8) | state.memory[state.sp];
            state.sp += 2;
        } else {
            step_cpu(state);
        }

        if (state.total_states >= 33333) {
            if (!driver->is_headless && driver->translate_video != nullptr) {
                SDL_Event e;
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_EVENT_QUIT) {
                        running = false;
                    }
                }

                driver->translate_video(state.memory, host_buffer.data());

                renderer.update_texture(host_buffer.data());
                renderer.render();
            }

            if (driver->is_throttled) {
                auto frame_end = std::chrono::high_resolution_clock::now();
                auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(frame_end - frame_start).count();

                if (elapsed_time < 16667) {
                    std::this_thread::sleep_for(std::chrono::microseconds(16667 - elapsed_time));
                }
            }

            state.total_states -= 33333;
            frame_start = std::chrono::high_resolution_clock::now();
        }
    }

    delete[] state.memory;

    return 0;
}

