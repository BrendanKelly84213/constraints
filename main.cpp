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

const float allowed_distance = 10.0f;
const Vec2 pos {100, 100}; 
const float bias_factor = 0.05f;

const size_t num_rows = 30;
const size_t num_cols = 50;
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

    SDL_Event event;
    uint32_t ticks_count = SDL_GetTicks();
    while (1) {

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) 
            break;


        float dt = (SDL_GetTicks() - ticks_count) / 1000.0f; 
        ticks_count = SDL_GetTicks();

        const size_t num_iterations = 50;
        for(size_t i = 0; i <= num_iterations; ++i ) {
            float sub_dt = dt / (static_cast<float>(num_iterations));

            for(auto& point : intersections) {
                point.apply_force({ 0, 1000 });
                point.apply_force({ 100, 0 });
            }

            for(auto& c : constraints) {
                c.update(sub_dt);
            }

            for(auto& point : intersections) {
                point.update_velocity(sub_dt);
                point.update_position(sub_dt);
            }
        }

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
