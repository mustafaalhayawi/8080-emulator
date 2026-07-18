#include "driver.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>

class Renderer {
    private:
        SDL_Window* gWindow{ nullptr };
        SDL_Surface* gScreenSurface{ nullptr };
    public:
        bool init(const Driver* driver, std::string window_name);
        void update_frame(const uint8_t* memory);
        void render();
        void close();
};

