#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <unistd.h>

#include "Renderer.h"
#include "PhysicsObject.h"
#include "DistanceConstraint.h"

template <typename T>
constexpr T clamp(T value, T constraint)
{
    if(value > constraint)
        return constraint;
    return value;
}

const double allowed_distance = 100.0f;

int main()
{
    Renderer renderer;
    if(!renderer.create("Pendelum", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 800, SDL_WINDOW_RESIZABLE))
        return 1;

    int w = renderer.w();

    PhysicsObject hinge({ renderer.w() / 2.0f, 200 }, { 0, 0 }, { 0, 0 }, 1, 20, 20);
    PhysicsObject ball1({ w / 2.0f + allowed_distance - 20, 12 }, { 0, 0 }, { 0, 0 }, 1, 20, 20);

    DistanceConstraint hinge_ball_constraint(&ball1, &hinge, allowed_distance, 0.1f);

    Vec2 force_gravity = { 0, ball1.mass() * 981 };

    SDL_Event event;
    uint32_t ticks_count = SDL_GetTicks();
    while (1) {

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) 
            break;

        double dt = (SDL_GetTicks() - ticks_count) / 1000.0f; 
        ticks_count = SDL_GetTicks();

        hinge_ball_constraint.update(dt);
        ball1.apply_force(force_gravity);
        ball1.update(dt);

        renderer.clear();
        renderer.draw_line(hinge.pos(), ball1.pos());
        renderer.present();
    }

    SDL_Quit();
    return 0;
}
