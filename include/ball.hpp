#pragma once

#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>

#include <misc.hpp>

namespace yballs {
class Ball {
public:
    Ball(unsigned int x, unsigned int y, double radius, double mass, Vec speed)
    {
        this->c.x = x;
        this->c.y = y;
        this->r = radius;
        this->mass = mass;
        this->speed = speed;
    }

    // Main characteristics of a ball
    Point c;
    double r;
    double mass;
    Vec speed;

private:
};

// Draw the specified ball onto the screen
void draw_ball(SDL_Renderer* renderer, Ball b, unsigned int fidelity = 360)
{
#define PI 3.14159265358979323846264338327950288419716939937510

    unsigned int x = b.r;
    unsigned int y = 0;
    double theta = 0;

    using namespace std;

    while (theta < PI * 2) {
        // Draw each point for the correct octave
        // Determine new x and y
        SDL_RenderDrawPoint(renderer, b.c.x + x, b.c.y + y);

        theta += PI / fidelity;
        y = b.r * sin(theta);
        x = b.r * cos(theta);
    }
}
}
