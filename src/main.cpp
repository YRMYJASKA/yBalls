//
// main.cpp
// Written by: Jyry Hjelt
//

#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unordered_map>

#include <ball.hpp>
#include <misc.hpp>
#include <settings.hpp>

std::unordered_map<int, yballs::Ball> ball_set;

int main(int argc, char* args[])
{
    // seed the random number generation
    std::srand(std::time(nullptr));

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
    {
        yballs::Vec _v;
        yballs::Ball _b;
        for (int i = 0; i < 10; i++) {
            _v.x = -10 + std::rand() % 20;
            _v.y = -10 + std::rand() % 20;
            _b = yballs::Ball((std::rand() % SCR_W + 10), (std::rand() % SCR_H + 10),
                (std::rand() % 150 + 10), 1, _v);
            ball_set[i] = _b;
        }
    }

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
        for (const auto& e : ball_set) {
            draw_ball(g_renderer, e.second, true);
            yballs::Ball tempball;
            tempball = e.second;
            tempball.c.x += tempball.speed.x;
            tempball.c.y += tempball.speed.y;
            if (yballs::settings::DEBUG) {
                std::cout << "Ball " << e.first << "\n";
                std::cout << e.second.speed << " => " << tempball.speed << std::endl;
                std::cout << "(" << e.second.c.x << ", " << e.second.c.y << ") => ("
                          << tempball.c.x << ", " << tempball.c.y << ")\n";
            }
            ball_set.at(e.first) = tempball;
        }
        SDL_RenderPresent(g_renderer);
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
