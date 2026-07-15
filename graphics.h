#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>

constexpr int screen_width{ 256 };
constexpr int screen_height{ 224 };

class Renderer {
    private:
        SDL_Window* gWindow{ nullptr };
        SDL_Surface* gScreenSurface{ nullptr };
    public:
        bool init();
        void update_frame(const uint8_t* memory);
        void render();
        void close();
};

