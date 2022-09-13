#pragma once

#include "Renderer.h"
#include "Texture.h"
#include "Math.h"


struct PhysicsObject {
// private:
    Vec2 m_pos;
    Vec2 m_vel;
    Vec2 m_accel;
    double m_mass;
    int m_w;
    int m_h;
    
// public:
    PhysicsObject(Vec2 pos, Vec2 vel, Vec2 accel, double mass, int w, int h) 
        : m_pos(pos), m_vel(vel), m_accel(accel), m_mass(mass), m_w(w), m_h(h) 
    {
    }

    void apply_force(Vec2 force)
    {
        if(m_mass != 0) 
            m_accel = { force.x / m_mass, force.y / m_mass };
    }

    void apply_linear_impulse(Vec2 force)
    {
        m_vel = m_vel + force * inverse_mass();
    }

    // Update velocity and position 
    void update(double dt)
    {
        m_vel = m_vel + m_accel * dt;
        m_pos = m_pos + m_vel * dt;
    }

    // Draw a texture at the objects location within its constraints
    void draw(Renderer & renderer, Texture & tex)
    {
        renderer.draw(tex, std::floor(m_pos.x), std::round(m_pos.y), m_w, m_h);
    }

    Vec2 pos() const { return m_pos; }
    Vec2 vel() const { return m_vel; }
    Vec2 accel() const { return m_accel; }
    double mass() const { return m_mass; }
    double inverse_mass() const 
    {
        if(m_mass == 0)
            return 0;
        return 1 / m_mass;
    }
};
