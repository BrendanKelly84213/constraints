#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <unistd.h>

#include "Renderer.h"
#include "PhysicsObject.h"
#include "DistanceConstraint.h"
// #include "Cloth.h"

template <typename T>
constexpr T clamp(T value, T constraint)
{
    if(value > constraint)
        return constraint;
    return value;
}

const double allowed_distance = 50.0f;
const Vec2 pos {300, 100}; 
const double bias_factor = 0.5f;

const size_t num_points = 5;
const size_t num_links = num_points - 1;

PhysicsObject intersections[num_points];
DistanceConstraint constraints[num_links];

Vec2 scale(size_t i, size_t j) 
{
    return {pos.x + i * allowed_distance * 1.0f, pos.y + j * allowed_distance * 1.0f};
}

int main()
{
    Renderer renderer;
    if(!renderer.create("Cloth", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE))
        return 1;


    for(size_t i = 0; i < num_points; ++i) {
        intersections[i] = PhysicsObject(scale(i, 0), 1);
        if(i == 0 || i == num_points - 1)
            intersections[i].m_fixed = true;
    }

    for(size_t i = 0; i < num_links; ++i) {
        constraints[i] = DistanceConstraint(&intersections[i], &intersections[i + 1], allowed_distance, bias_factor);
    }

    std::cout << "-- INTERSECTIONS -- \n";
    for(auto i : intersections) {
        std::cout << "x: " << i.pos().x << " y: " << i.pos().y << '\n';
    }

    std::cout << "-- CONSTRAINTS -- \n";
    for(auto c : constraints) {
        std::cout << "P: x: " << c.p_pos().x << " y: " << c.p_pos().y << '\n';
        std::cout << "Q: x: " << c.q_pos().x << " y: " << c.q_pos().y << '\n';
    }

    SDL_Event event;
    uint32_t ticks_count = SDL_GetTicks();
    while (1) {

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) 
            break;


        double dt = (SDL_GetTicks() - ticks_count) / 1000.0f; 
        ticks_count = SDL_GetTicks();

        const size_t num_iterations = 20;
        for(size_t i = 0; i <= num_iterations; ++i ) {
            float sub_dt = dt / (num_iterations * 1.0f);

            for(size_t j = 0; j < num_points; ++j) {
                PhysicsObject& point = intersections[j];

                point.accelerate({ 0, 1000 });
            }

            for(size_t j = 0; j < num_points; ++j) {
                PhysicsObject& point = intersections[j];

                point.update_position(sub_dt);
            }

            for(auto c : constraints) {
                c.update(sub_dt);
            }

            for(size_t j = 0; j < num_points; ++j) {
                PhysicsObject& point = intersections[j];

                point.update_velocity(sub_dt);
            }
        }
        

        renderer.clear();

        for(auto c : constraints) {
            renderer.draw_line(c);
        }

        renderer.present();
    }

    SDL_Quit();
    return 0;
}
