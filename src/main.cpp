//
// main.cpp
// Written by: Jyry Hjelt
//

#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>
#include <vector>

#include <ball.hpp>
#include <misc.hpp>
#include <settings.hpp>

std::vector<yballs::Ball> ball_set;
bool pause_main = false;
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

    // Create balls
    {
        yballs::Vec _v(-1, -1);
        yballs::Ball _b;
        for (int i = 0; i < 29; i++) {
            _v = yballs::Vec(-1 + std::rand() % 4, -2 + std::rand() % 3);
            unsigned int rad = 10 + std::rand() % 30;
            bool valid_pos = false;
            while (!valid_pos) {
                valid_pos = true;
                _b = yballs::Ball(std::rand() % (SCR_W - rad - 1) + rad, std::rand() % (SCR_H - rad - 1) + rad + 1, rad, rad / 10, _v);
                for (auto const& e : ball_set) {
                    if (dist_between_points(e.c, _b.c) < _b.r + e.r) {
                        valid_pos = false;
                    }
                }
            }
            ball_set.push_back(_b);
        }
        /*
        ball_set.push_back(yballs::Ball(50, 100, 30, 20, yballs::Vec(1, 0)));
        ball_set.push_back(yballs::Ball(200, 100, 30, 20, yballs::Vec(-1, 0)));
        ball_set.push_back(yballs::Ball(300, 100, 30, 20, yballs::Vec(0, 0)));
		*/
    }

    std::clock_t prev_time = std::clock();
    bool running_main = true;
    while (running_main) {
        // Calculate the elapsed time between this and previous frame
        std::clock_t frame_begin = std::clock();

        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {

                running_main = false;
                continue;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_SPACE:
                    pause_main = !pause_main;
                    break;
                default:
                    break;
                }
            }
        }
        // If the simulation has been paused, keep the current state
        if (pause_main) {
            prev_time = frame_begin;
            continue;
        }

        SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(g_renderer);

        // TODO: Custom colour
        SDL_SetRenderDrawColor(g_renderer, 0xFF, 0x00, 0x00, 0xFF);

        // Check each pair of balls if they are colliding. If so, add them
        // to the collided_pairs. Also check and handle wall collisions.
        std::vector<std::pair<yballs::Ball*, yballs::Ball*>> collided_pairs;
        for (long unsigned int i = 0; i < ball_set.size(); i++) {
            yballs::Ball b1 = ball_set[i];

            // Wall collisions
            if (b1.c.y - b1.r < 0 || b1.c.y + b1.r > SCR_H) {
                // Collision with top or bottom wall
                // hence, mirror velocity horisontally
                b1.speed *= yballs::Vec(1, -1);
                if (b1.c.y - b1.r < 0) {
                    // Top wall collision
                    b1.c.y = b1.r + 1;
                } else {
                    b1.c.y = SCR_H - b1.r - 1;
                }

                ball_set.at(i) = b1;
            }
            if (b1.c.x - b1.r < 0 || b1.c.x + b1.r > SCR_W) {
                // Collision with the left or right wall
                // hence, mirror velocity vertically0
                if (b1.c.x - b1.r < 0) {
                    // Left wall collision
                    b1.c.x = b1.r + 1;
                } else {
                    b1.c.x = SCR_W - b1.r - 1;
                }
                b1.speed *= yballs::Vec(-1, 1);
                ball_set.at(i) = b1;
            }

            for (long unsigned int j = i + 1; j < ball_set.size(); j++) {
                yballs::Ball b2 = ball_set[j];
                double dist = dist_between_points(b1.c, b2.c);
                if (dist <= b1.r + b2.r) {
                    // There is a collision between the two balls
                    collided_pairs.push_back({ &ball_set[i], &ball_set[j] });

                    // There is overlap between the two balls
                    float overlap = 0.5 * (dist - b1.r - b2.r);
                    yballs::Vec diffvec(b1.c.x - b2.c.x, b1.c.x - b2.c.x);
                    // Move each ball 1/2 the dist from each other to prevent overlap
                    ball_set[i].c.x -= overlap * (b1.c.x - b2.c.x) / dist;
                    ball_set[i].c.y -= overlap * (b1.c.y - b2.c.y) / dist;
                    ball_set[j].c.x -= overlap * (b2.c.x - b1.c.x) / dist;
                    ball_set[j].c.y -= overlap * (b2.c.y - b1.c.y) / dist;
                }
            }
        }

        // Handle collisions between the balls and adjust velocities as needed
        for (auto& cols : collided_pairs) {
            yballs::Ball* b1 = cols.first;
            yballs::Ball* b2 = cols.second;

            yballs::Vec dist_vec1(b1->c.x - b2->c.x, b1->c.y - b2->c.y);
            yballs::Vec _v1 = b1->speed - (2 * b2->mass) / (b1->mass + b2->mass) * (vec_dot(b1->speed - b2->speed, dist_vec1) / pow(dist_vec1.mag(), 2)) * dist_vec1;

            yballs::Vec dist_vec2(b2->c.x - b1->c.x, b2->c.y - b1->c.y);
            yballs::Vec _v2 = b2->speed - (2 * b1->mass) / (b1->mass + b2->mass) * (vec_dot(b2->speed - b1->speed, dist_vec2) / pow(dist_vec2.mag(), 2)) * dist_vec2;
            // set the new velocities
            std::cout << b1->speed << " -> " << _v1 << std::endl;
            b1->speed = _v1;
            std::cout << b2->speed << " -> " << _v2 << std::endl;
            b2->speed = _v2;
        }

        // Apply each ball's velocity to it's position
        for (long unsigned int i = 0; i < ball_set.size(); i++) {
            if (yballs::settings::DEBUG) {
                std::cout << "Ball " << i << ": ";
                std::cout << ball_set[i].speed << ", (" << ball_set[i].c.x << ", " << ball_set[i].c.y << ") =>";
            }
            draw_ball(g_renderer, ball_set[i], true);

            ball_set[i].c.x += round(ball_set[i].speed.x);
            ball_set[i].c.y += round(ball_set[i].speed.y);

            if (yballs::settings::DEBUG) {
                std::cout << "(" << ball_set[i].c.x << ", " << ball_set[i].c.y << ")\n";
            }
        }

        SDL_RenderPresent(g_renderer);
        prev_time = std::clock();

        const double frame_time = double(prev_time - frame_begin) / CLOCKS_PER_SEC;
        SDL_Delay(16.6 - frame_time);
        if (yballs::settings::DEBUG) {
            std::cout << "Seconds for frame: " << frame_time << std::endl;
        }
    }

    // Exit the program, cleanly
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    g_renderer = NULL;
    SDL_Quit();
    return 0;
}
