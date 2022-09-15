#pragma once

#include <cmath>

struct Vec2 {
    double x;
    double y;

    static double dot_product(const Vec2 & a, const Vec2 & b) 
    {
        return a.x * b.x + a.y * b.y;
    }

    static double distance(const Vec2 & a, const Vec2 & b)
    {
        return std::sqrt( (a.x - b.x) * (a.x - b.x)  + (a.y - b.y) * (a.y - b.y) );
    }

    double magnitude() const 
    {
        return distance(*this, { 0, 0 });
    }

    Vec2 normal() const 
    {
        return (1 / magnitude()) * (*this);
    }

    Vec2& operator +=(const Vec2 & a)
    {
        x += a.x;
        y += a.y;
        return *this;
    }

    Vec2& operator-= (const Vec2 & a)
    {
        x -= a.x;
        y -= a.y;
        return *this;
    }

    friend Vec2 operator * (const Vec2 & a, double b)
    {
        return { a.x * b, a.y * b };
    }

    friend Vec2 operator * (double a, const Vec2 & b)
    {
        return { b.x * a, b.y * a };
    }

    friend Vec2 operator + (const Vec2 & a, const Vec2 & b)
    {
        return { a.x + b.x, a.y + b.y };
    }

    friend Vec2 operator - (const Vec2 & a, const Vec2 & b)
    {
        return { a.x - b.x, a.y - b.y };
    }

    friend Vec2 operator / (const Vec2 & a, double b)
    {
        return a * (1 / b);
    }
};
