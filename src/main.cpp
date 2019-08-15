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
#include <file_parser.hpp>
#include <misc.hpp>
#include <settings.hpp>
#include <version.h>

std::vector<yballs::Ball> ball_set;
bool pause_main = false;

unsigned char BALL_COLOUR[3] = { 0xFF, 0x00, 0x00 };
unsigned char BACKGROUND_COLOUR[3] = { 0xFF, 0xFF, 0xFF };

unsigned int scrs_num = 0;
SDL_Surface* sshot;

using namespace yballs::settings;

int main(int argc, char* args[])
{
    // seed the random number generation
    std::srand(std::time(nullptr));

    // Set default values for FLAGS and PARAMS
    init_defaults();

    // Parse the command given
    for (int i = 1; i < argc; i++) {
        if (std::string("--help").compare(args[i]) == 0 || std::string("-h").compare(args[i]) == 0) {
            // Print help
            std::cout << "yBalls " << VERSION_STRING << std::endl;
            std::cout << "Usage: yballs [ARGS] [-f <filename>]\n";
            std::cout << "if no configuration file is given, random balls are generated\n";
            std::cout << "\t-h or --help:\t\t\tPrints the help\n";
            std::cout << "\t-v:\t\t\t\tVerbose output\n";
            std::cout << "\t-n <int>:\t\t\tNumber of balls if randomly generated \n";
            std::cout << "\t-g:\t\t\t\tApply gravity to the simulation\n";
            std::cout << "\t--conf:\t\t\t\tPrint out the inital ball configuration\n";
            std::cout << "\t--width <int>: \t\t\tSets the height of the simulation\n";
            std::cout << "\t--height <int>: \t\tSets the  of the simulation\n";
            std::cout << "\t--colour <hex> <hex> <hex>: \tSet the RGB colour of the balls\n";
            std::cout << "\t--bcolour <hex> <hex> <hex>: \tSet the RGB colour of the background\n";
            std::cout << "\t+ Much more... try 'yballs --help-params' to see all the possible parameters\n";
            std::cout << "Keyboard commands:\n";
            std::cout << "\tspace:\tPause\n";
            std::cout << "\tq:\tquit\n";
            std::cout << "\ts:\tScreenshot\n";
            return 0;

        } else if (std::string("--help-params").compare(args[i]) == 0) {
            std::cout << "All possible adjustable parameters that can be set for a simulation\n";
            for (auto& e : PARAMS) {
                std::cout << e.first << std::endl;
            }
            return 0;

        } else if (std::string("--colour").compare(args[i]) == 0) {
            BALL_COLOUR[0] = std::stoul(args[i + 1], nullptr, 16);
            BALL_COLOUR[1] = std::stoul(args[i + 2], nullptr, 16);
            BALL_COLOUR[2] = std::stoul(args[i + 3], nullptr, 16);
            i += 3;
        } else if (std::string("--bcolour").compare(args[i]) == 0) {
            BACKGROUND_COLOUR[0] = std::stoul(args[i + 1], nullptr, 16);
            BACKGROUND_COLOUR[1] = std::stoul(args[i + 2], nullptr, 16);
            BACKGROUND_COLOUR[2] = std::stoul(args[i + 3], nullptr, 16);
            i += 3;
        } else if (std::string("-f").compare(args[i]) == 0) {
            ball_set = yballs::configuration(args[i + 1]);
            if (ball_set.size() < 1) {
                std::cout << "reading from file failed OR empty file!\n";
                return -1;
            }
            i++;
            FLAGS["-r"] = false;
        } else if (FLAGS.find(args[i]) != FLAGS.end()) {
            // A flag was set, therefore set it to true
            FLAGS[args[i]] = true;
        } else if (PARAMS.find(args[i]) != PARAMS.end()) {
            PARAMS[args[i]] = std::stoi(args[i + 1]);
            i++;
        } else {
            std::cout << "Invalid option '" << args[i] << "'\n";
            std::cout << "Try 'yballs -h' for help\n";
            return -1;
        }
    }

    const int SCR_W = PARAMS["--width"];
    const int SCR_H = PARAMS["--height"];

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
    SDL_SetRenderDrawColor(g_renderer, BACKGROUND_COLOUR[0], BACKGROUND_COLOUR[1], BACKGROUND_COLOUR[2], 0xFF);
    SDL_RenderClear(g_renderer);

    // Create balls randomly if needed
    if (FLAGS["-r"]) {
        yballs::Vec _v;
        yballs::Ball _b;
        for (int i = 0; i < PARAMS["-n"]; i++) {
            bool valid_pos = false;
            while (!valid_pos) {
                _v = yballs::Vec(PARAMS["--min-vel"] + std::rand() % (PARAMS["--max-vel"] - PARAMS["--min-vel"] + 1),
                    PARAMS["--min-vel"] + std::rand() % (PARAMS["--max-vel"] - PARAMS["--min-vel"] + 1));
                unsigned int rad = PARAMS["--min-rad"] + std::rand() % (PARAMS["--max-rad"] - PARAMS["--min-rad"] + 1);
                valid_pos = true;
                if (PARAMS["--mass-independent"]) {
                    _b = yballs::Ball(std::rand() % (SCR_W - rad - 1) + rad,
                        std::rand() % (SCR_H - rad - 1) + rad + 1, rad,
                        PARAMS["--min-mass"] + std::rand() % (PARAMS["--max-mass"] - PARAMS["--min-mass"] + 1), _v);
                } else {
                    _b = yballs::Ball(std::rand() % (SCR_W - rad - 1) + rad,
                        std::rand() % (SCR_H - rad - 1) + rad + 1, rad,
                        rad * PARAMS["--rad-to-mass"], _v);
                }
                for (auto const& e : ball_set) {
                    if (dist_between_points(e.c, _b.c) < _b.r + e.r) {
                        valid_pos = false;
                    }
                }
            }
            ball_set.push_back(_b);
        }
    }

    // If -v OR --conf flag is set, then print the current configuration
    if (FLAGS["--conf"] || FLAGS["-v"]) {
        std::cout << "CURRENT CONFIG:\n";
        std::cout << "WIDTH " << SCR_W << std::endl;
        std::cout << "HEIGHT " << SCR_H << std::endl;
        for (int i = 0; i < ball_set.size(); i++) {
            yballs::Ball _b = ball_set[i];
            std::cout << _b << std::endl;
        }
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
			std::string sshot_name = "screenshot";
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_SPACE:
                    pause_main = !pause_main;
                    break;
                case SDLK_q:
                    running_main = false;
                    break;
                case SDLK_s:
                    std::cout << "Taking Screenshot...\n";
                    sshot = SDL_CreateRGBSurface(0, PARAMS["--width"],
                        PARAMS["--height"], 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
                    SDL_RenderReadPixels(g_renderer, NULL,
                        SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
					sshot_name += std::to_string(scrs_num);
                    SDL_SaveBMP(sshot, sshot_name.append(".bmp").c_str());
                    SDL_FreeSurface(sshot);
                    scrs_num++;
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

        SDL_SetRenderDrawColor(g_renderer, BACKGROUND_COLOUR[0], BACKGROUND_COLOUR[1], BACKGROUND_COLOUR[2], 0xFF);
        SDL_RenderClear(g_renderer);

        // TODO: Custom colour
        SDL_SetRenderDrawColor(g_renderer, BALL_COLOUR[0], BALL_COLOUR[1], BALL_COLOUR[2], 0xFF);

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

            // Check if b1 has collided with any other balls
            for (long unsigned int j = i + 1; j < ball_set.size(); j++) {
                yballs::Ball b2 = ball_set[j];
                double dist = dist_between_points(b1.c, b2.c);
                if (pow(b1.c.x - b2.c.x, 2) + pow(b1.c.y - b2.c.y, 2) <= pow(b1.r + b2.r, 2)) {
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
            b1->speed = _v1;
            b2->speed = _v2;
        }

        // Apply each ball's velocity to it's position
        for (long unsigned int i = 0; i < ball_set.size(); i++) {
            if (FLAGS["-v"]) {
                std::cout << "Ball " << i << ": ";
                std::cout << ball_set[i].speed << ", (" << ball_set[i].c.x << ", " << ball_set[i].c.y << ") =>";
            }
            draw_ball(g_renderer, ball_set[i], true);
            if (FLAGS["-g"]) {
                // apply gravity
                ball_set[i].speed.y += .98;
            }
            ball_set[i].c.x += round(ball_set[i].speed.x);
            ball_set[i].c.y += round(ball_set[i].speed.y);

            if (FLAGS["-v"]) {
                std::cout << "(" << ball_set[i].c.x << ", " << ball_set[i].c.y << ")\n";
            }
        }

        SDL_RenderPresent(g_renderer);
        prev_time = std::clock();

        const double frame_time = double(prev_time - frame_begin) / CLOCKS_PER_SEC;
        SDL_Delay(16.6 - frame_time);
        if (FLAGS["-v"]) {
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
