#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <unistd.h>

#include "Renderer.h"
#include "PhysicsObject.h"

double lagrange_multiplier(const PhysicsObject& particle, const Vec2 external_force)
{
    const double mass = particle.mass();
    const Vec2 pos = particle.pos();
    const Vec2 vel = particle.vel();
    return (-1 * dot_product(external_force, pos) - dot_product(mass * vel, vel)) / dot_product(pos, pos);
}

template <typename T>
constexpr T clamp(T value, T constraint)
{
    if(value > constraint)
        return constraint;
    return value;
}

constexpr double distance(Vec2 a, Vec2 b)
{
    return std::sqrt( (a.x - b.x) * (a.x - b.x)  + (a.y - b.y) * (a.y - b.y) );
}

constexpr double vec_mag(Vec2 vec)
{
    return distance(vec, { 0, 0 });
}

constexpr Vec2 vector_normal(Vec2 vec)
{
    double mag = vec_mag(vec); 
    return (1 / mag) * vec;
}

// Assume 1cm = 1 pixel
//

const double allowed_distance = 200.0f;

int main()
{
    Renderer renderer;
    if(!renderer.create())
        return 1;

    Texture sample = renderer.texture_from_bmp("sample.bmp");

    int w = renderer.w();
    PhysicsObject test({ w / 2.0f + allowed_distance, 12 }, { 0, 0 }, { 0, 0 }, 1, 50, 50);
    PhysicsObject hinge({ w / 2.0f, 12 }, { 0, 0 }, { 0, 0 }, 0, 10, 10);
    Vec2 force_gravity = { 0, test.mass() * 981 };

    SDL_Event event;
    uint32_t ticks_count = SDL_GetTicks();
    while (1) {

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) 
            break;

        double dt = (SDL_GetTicks() - ticks_count) / 1000.0f; 
        ticks_count = SDL_GetTicks();

        const Vec2 relative_pos = hinge.pos() - test.pos();
        const double relative_distance = vec_mag(relative_pos);
        const double offset = relative_distance - allowed_distance;

        Vec2 test_total_force = force_gravity;

        if(offset > 0.0f && dt > 0) {

            Vec2 offset_direction = vector_normal(relative_pos);

            const double velocity_dot = dot_product(test.vel(), offset_direction);
            const double bias_factor = 0.01f;
            const double bias = -(bias_factor / dt) * offset;

            const double lambda = -(velocity_dot + bias) / (test.inverse_mass());

            const Vec2 test_impulse = offset_direction * lambda;
            const Vec2 hinge_impulse = -1 * (offset_direction * lambda);

            test.apply_linear_impulse(test_impulse); 
            hinge.apply_linear_impulse(hinge_impulse);
        }

        test.apply_force(test_total_force);
        test.update(dt);
        test.draw(renderer, sample);

        if(test.pos().y + test.m_h >= renderer.h() || test.pos().y <= 0) {
            test.m_vel.y = -test.m_vel.y;
        }
        if(test.pos().x >= renderer.w() - test.m_w || test.pos().x <= 0) {
            test.m_vel.x = -test.m_vel.x;
        }
    }

    SDL_Quit();
    return 0;
}
