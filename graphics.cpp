#include "graphics.h"

bool Renderer::init(const Driver* driver, std::string window_name) {
    bool success{ true };

    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialise! SDL ERROR: %s\n", SDL_GetError());
        success = false;
    }
    else {
        if (gWindow = SDL_CreateWindow(window_name.c_str(), driver->screen_width, driver->screen_height, 0); gWindow == nullptr) {
            SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
            success = false;
        } else {
            gSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

void Renderer::update_texture(const uint32_t* host_buffer) {
    if (SDL_LockSurface(gSurface)) {
        uint32_t* surface_pixels = (uint32_t*)gSurface->pixels;
        const SDL_PixelFormatDetails* format_details = SDL_GetPixelFormatDetails(gSurface->format);

        int total_pixels = gSurface->w * gSurface->h;

        for (int i = 0; i < total_pixels; i++) {
            uint32_t pixel = host_buffer[i];

            surface_pixels[i] = SDL_MapRGBA(format_details,
                                            NULL,
                                            (uint8_t)(pixel & 0xFF),
                                            (uint8_t)((pixel & 0xFF00) >> 8),
                                            (uint8_t)((pixel & 0xFF0000) >> 16),
                                            (uint8_t)((pixel & 0xFF000000) >> 24));
        }

        SDL_UnlockSurface(gSurface);
    }
}

void Renderer::render() {
    SDL_UpdateWindowSurface(gWindow);
}

void Renderer::close() {
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gSurface = nullptr;

    SDL_Quit();
}
