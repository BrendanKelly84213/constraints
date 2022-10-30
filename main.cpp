#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <unistd.h>
#include <vector>

#include "Renderer.h"
#include "PhysicsObject.h"
#include "DistanceConstraint.h"

const float allowed_distance = 8.0f;
const Vec2 pos {100, 50}; 
const float bias_factor = 0.05f;

const size_t num_rows = 40;
const size_t num_cols = 60;
const size_t num_points = num_rows * num_cols;
const size_t num_iterations = 16;

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

    for(size_t i = num_cols - 1; i > 0; --i) {
        for(size_t j = num_rows - 1; j > 0; --j) {
            unsigned pi = point_index(i, j);
            intersections[pi] = PhysicsObject(scale(i, j), 1);
            intersections[pi].set_id(point_index(i, j));

            if(j == 1)
                intersections[pi].set_fixed(true);

            if(i > 1) {
                unsigned pi_left = point_index(i - 1, j);
                add_link(&intersections[pi], &intersections[pi_left]);
            }

            if(j > 1) {
                unsigned pi_up = point_index(i, j - 1);
                add_link(&intersections[pi], &intersections[pi_up]);
            }
        }
    }

    PhysicsObject mouse_object({0, 0}, 1.0f, true);
    mouse_object.set_id(point_index(num_cols, num_rows) + 1);
    for(auto& point : intersections) {
        constraints.push_back(DistanceConstraint(&point, &mouse_object, 0.0f, 0.07f, false, false));
    }

    SDL_Event event;
    uint32_t ticks_count = SDL_GetTicks();
    
    bool dragging = false;
    bool grabbed = false;
    bool cutting = false;
    while (1) {

        int mouse_x, mouse_y;
        mouse_object.move_to({ static_cast<double>(mouse_x), static_cast<double>(mouse_y) });
        SDL_GetMouseState(&mouse_x, &mouse_y);

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) 
            break;
        else if(event.type == SDL_MOUSEBUTTONDOWN) {
            if(event.button.button == SDL_BUTTON_LEFT)
                dragging = true;
            else if(event.button.button == SDL_BUTTON_RIGHT)
                cutting = true;
        } else if(event.type == SDL_MOUSEBUTTONUP) {
            dragging = false;
            grabbed = false;
            cutting = false;
        }

        for(auto& constraint : constraints) {
            if(dragging && !grabbed && constraint.has(mouse_object) && constraint.distance() < 26.0f) {
                constraint.set_preferred_distance(26.0f);
                constraint.set_connected(true);
                grabbed = true;
                break;
            } else if(!dragging && constraint.has(mouse_object)) {
                constraint.set_connected(false);
            } else if(cutting && Vec2::distance(constraint.p_pos(), mouse_object.pos()) < allowed_distance && Vec2::distance(constraint.q_pos(), mouse_object.pos()) < allowed_distance){
                constraint.set_connected(false);
            }
        }

        float dt = (SDL_GetTicks() - ticks_count) / 1000.0f; 
        ticks_count = SDL_GetTicks();

        for(size_t i = 0; i <= num_iterations; ++i) {

            float sub_dt = dt / (static_cast<float>(num_iterations));

            for(auto& c : constraints) {
                if(!c.hard())
                    c.update(sub_dt);
            }

            for(auto& point : intersections) {
                point.apply_force({ 0, 800 });
            }

            for(auto& point : intersections) {
                point.update_position(sub_dt);
            }

            for(auto& c : constraints) {
                if(c.hard())
                    c.update(sub_dt);
            }

            for(auto& point : intersections) {
                point.update_velocity(sub_dt);
            }
        }

        renderer.clear();

        for(auto& c : constraints) {
            if(c.connected())
                renderer.draw_line(c);
        }

        renderer.present();
    }

    SDL_Quit();
    return 0;
}
