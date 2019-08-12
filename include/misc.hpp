#pragma once

#include <cmath>
#include <iostream>

namespace yballs {

// Generalised point on the Cartesian plane
struct Point {
    int x = 0;
    int y = 0;
};

double dist_between_points(Point p1, Point p2)
{
    return sqrt(pow((p1.x - p2.x), 2) + pow(p1.y - p2.y, 2));
}

Point midpoint(Point p1, Point p2)
{
    Point p;
    p.x = (p1.x + p2.x) / 2;
    p.y = (p1.y + p2.y) / 2;
    return p;
}

// A 2D vector class to represent the velocities of the balls
class Vec {
public:
    double x, y;
    Vec(double x = 0, double y = 0)
    {
        this->x = x;
        this->y = y;
    }

    // Return the magnitude of the vector
    double mag()
    {
        return sqrtf(pow(this->x, 2) + pow(this->y, 2));
    }

    // Operator overloading
    // std::ostream
    friend std::ostream& operator<<(std::ostream& out, const Vec& v)
    {
        return out << "[" << v.x << " " << v.y << "]";
    }
    // Assignment
    Vec& operator=(Vec rhs) noexcept
    {
        std::swap(x, rhs.x);
        std::swap(y, rhs.y);
        return *this;
    }

    // Addition & Substraction
    Vec& operator+=(const Vec& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }
    friend Vec operator+(Vec lhs, const Vec& rhs)
    {
        lhs += rhs;
        return lhs;
    }
    Vec& operator-=(const Vec& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }
    friend Vec operator-(Vec lhs, const Vec& rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    // Scalar multiplication
    template <typename T>
    Vec& operator*=(T scalar)
    {
        this->x *= scalar;
        this->y *= scalar;
        return *this;
    }
    Vec& operator*=(const Vec& rhs)
    {
        this->x *= rhs.x;
        this->y *= rhs.y;
        return *this;
    }
    template <typename T>
    friend Vec operator*(Vec lhs, T scalar)
    {
        lhs *= scalar;
        return lhs;
    }
    template <typename T>
    friend Vec operator*(T scalar, Vec rhs)
    {
        rhs *= scalar;
        return rhs;
    }
    friend Vec operator*(Vec lhs, const Vec& rhs)
    {
        lhs *= rhs;
        return lhs;
    }
    template <typename T>
    Vec& operator/=(T scalar)
    {
        this->x /= scalar;
        this->y /= scalar;
        return *this;
    }
    template <typename T>
    friend Vec operator/(Vec lhs, T scalar)
    {
        lhs /= scalar;
        return lhs;
    }
};

// Dot product of two vectors
double vec_dot(Vec v1, Vec v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}
}
