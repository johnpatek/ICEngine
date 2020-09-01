#include "netpong.h"
#include "SDL2/SDL.h"

/* Moving Rectangle */
int main(int argc, char *argv[])
{
    bool loop(true);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;
    SDL_Rect r;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
            return 3;
    }

    window = SDL_CreateWindow("SDL_CreateTexture",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    1024, 768,
                    SDL_WINDOW_RESIZABLE);

    r.w = 100;
    r.h = 50;

    renderer = SDL_CreateRenderer(window, -1, 0);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 768);
    
    r.x=50;
    r.y=500;
    while (loop) 
    {
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
        {
            loop = false;
        }

        if(event.type == SDL_KEYDOWN && event.key == SDLK_LEFT && x > 0)
        {

        }
        
        if(event.type == SDL_KEYDOWN && event.key == SDLK_RIGHT && x < 500)
        {

        }
        
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_RenderDrawRect(renderer,&r);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
        SDL_RenderFillRect(renderer, &r);
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
 
    return 0;
}