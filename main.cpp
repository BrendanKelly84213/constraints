#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <unistd.h>
#include <vector>

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

const double allowed_distance = 30.0f;
const Vec2 pos {300, 100}; 
const double bias_factor = 0.01f;

const size_t num_rows = 10;
const size_t num_cols = 10;
const size_t num_points = num_rows * num_cols;
const size_t num_links = ((num_rows - 1) * num_cols) + ((num_cols - 1) * num_rows);

PhysicsObject intersections[num_points];
std::vector<DistanceConstraint> constraints;

Vec2 scale(size_t i, size_t j) 
{
    return {pos.x + i * allowed_distance * 1.0f, pos.y + j * allowed_distance * 1.0f};
}

unsigned point_index(size_t i, size_t j)
{
    return i * num_rows + j;
}

void add_link(PhysicsObject* a, PhysicsObject* b) 
{
    constraints.push_back(DistanceConstraint(a, b, allowed_distance, bias_factor));
}

int main()
{
    Renderer renderer;
    if(!renderer.create("Cloth", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE))
        return 1;

    for(size_t i = 0; i < num_cols; ++i) {
        for(size_t j = 0; j < num_rows; ++j) {
            unsigned pi = point_index(i, j);
            intersections[pi] = PhysicsObject(scale(i, j), 1);

            if(j == 0)
                intersections[pi].set_fixed(true);

            if(i > 0) {
                unsigned pi_left = point_index(i - 1, j);
                add_link(&intersections[pi], &intersections[pi_left]);
            }

            if(j > 0) {
                unsigned pi_up = point_index(i, j - 1);
                add_link(&intersections[pi], &intersections[pi_up]);
            }
        }
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

#if 1
        const size_t num_iterations = 20;
        for(size_t i = 0; i <= num_iterations; ++i ) {
            float sub_dt = dt / (num_iterations * 1.0f);

            for(auto& point : intersections) {
                point.apply_force({ 0, 1000 });
                // point.apply_force({ 55, 0 });
            }

            for(auto c : constraints) {
                c.update(sub_dt);
            }

            for(size_t j = 0; j < num_points; ++j) {
                PhysicsObject& point = intersections[j];

                point.update_position(sub_dt);
            }

            for(size_t j = 0; j < num_points; ++j) {
                PhysicsObject& point = intersections[j];

                point.update_velocity(sub_dt);
            }
        }
        #endif
        

        renderer.clear();

        for(auto c : constraints) {
            renderer.draw_line(c);
        }

        for(auto& point : intersections) {
            renderer.draw_point(point.pos());
        }

        renderer.present();
    }

    SDL_Quit();
    return 0;
}
