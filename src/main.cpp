//
// main.cpp
// Written by: Jyry Hjelt
//

#include <SDL2/SDL.h>
#include <iostream>

#include <ball.hpp>
#include <misc.hpp>

int main(int argc, char* args[])
{
    // TODO: parse the args

    const int SCR_W = 640;
    const int SCR_H = 480;

    SDL_Window* window = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL failed to initialize!\n";
        std::cout << "SDL_GetError:\n";
        std::cout << SDL_GetError() << "\n";
        return -1;
    }

    window = SDL_CreateWindow("yballs", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_W, SCR_H, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cout << "'window' failed to initialize!\n";
        std::cout << "SDL_GetError:\n";
        std::cout << SDL_GetError() << "\n";
        return -1;
    }
    SDL_Renderer* g_renderer = SDL_CreateRenderer(window, -1, 0);
    if (g_renderer == NULL) {
        std::cout << "'g_renderer' failed to initialize!\n";
        std::cout << "SDL_GetError:\n";
        std::cout << SDL_GetError() << "\n";
        return -1;
    }

    // Clearing the screen
    SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(g_renderer);

    // Create a ball and draw it
    yballs::Vec v1(1, 2);
    yballs::Ball b1(200, 200, 100, 1, v1);

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
        }

        SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(g_renderer);
        SDL_SetRenderDrawColor(g_renderer, 0xFF, 0x00, 0x00, 0xFF);
        draw_ball(g_renderer, b1);
        SDL_RenderPresent(g_renderer);
		b1.c.x = b1.c.x + b1.speed.x;
		b1.c.y = b1.c.y + b1.speed.y;
		SDL_Delay(100);
    }

    // Exit the program, cleanly
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    g_renderer = NULL;
    SDL_Quit();
    return 0;
}
