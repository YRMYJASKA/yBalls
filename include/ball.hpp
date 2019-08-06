#pragma once

#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>

#include <misc.hpp>

namespace yballs {
class Ball {
public:
    Ball()
    {
        this->c.x = 0;
        this->c.y = 0;
        this->r = 0;
        this->mass = 0;
        this->speed = Vec(0, 0);
    }
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
	
	// Operator overloading
	// Assignment
    Ball& operator=(Ball rhs) noexcept
    {
        std::swap(c, rhs.c);
        std::swap(r, rhs.r);
        std::swap(mass, rhs.mass);
        std::swap(speed, rhs.speed);
        return *this;
    }
private:
};

// Draw the specified ball onto the screen
void draw_ball(SDL_Renderer* renderer, Ball b, bool filled = false)
{
#define PI 3.14159265358979323846264338327950288419716939937510

    unsigned int x = b.r;
    unsigned int y = 0;
    double theta = 0;

    while (theta < PI * 2) {
        // Draw each point for the correct octave
        // Determine new x and y
        SDL_RenderDrawPoint(renderer, b.c.x + x, b.c.y + y);

        // Colour in the ball if the 'filled' flag is set
        if (filled) {
            SDL_RenderDrawLine(renderer, b.c.x + x, b.c.y, b.c.x + x, b.c.y + y);
        }

        // Advance to the next tilt of the drawing tip
        theta += PI / 440;
        y = b.r * sin(theta);
        x = b.r * cos(theta);
    }
}
}
