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
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

void Renderer::render() {
    bool quit{ false };

    SDL_Event e;
    SDL_zero( e );

    while (quit == false) {
        while (SDL_PollEvent(&e) == true) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        SDL_FillSurfaceRect(gScreenSurface, nullptr, SDL_MapSurfaceRGB(gScreenSurface, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(gWindow);
    }
}

void Renderer::close() {
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gScreenSurface = nullptr;

    SDL_Quit();
}
