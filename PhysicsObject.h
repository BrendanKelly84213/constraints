#pragma once

#include "Vec2.h"

// TODO: Rename member variables

class PhysicsObject {
private:
    Vec2 m_current_pos;
    Vec2 m_old_pos;
    Vec2 m_velocity;
    Vec2 m_accel;
    Vec2 m_forces;
    double m_mass;
    int m_w;
    int m_h;
    bool m_fixed;
    
public:
    PhysicsObject() = default;

    PhysicsObject(Vec2 pos, double mass) 
        : m_current_pos(pos), m_old_pos(pos), m_accel({0, 0}), m_forces({0, 0}), m_mass(mass), m_w(0), m_h(0), m_fixed(false)
    {
    }

    PhysicsObject(Vec2 pos, double mass, bool fixed) 
        : m_current_pos(pos), m_old_pos(pos), m_accel({0, 0}), m_forces({0, 0}), m_mass(mass), m_w(0), m_h(0), m_fixed(fixed)
    {
    }

    PhysicsObject(Vec2 pos, Vec2 accel, double mass, int w, int h) 
        : m_current_pos(pos), m_old_pos(pos), m_accel(accel), m_mass(mass), m_w(w), m_h(h), m_fixed(false)
    {
    }


    void apply_force(Vec2 force)
    {
        m_forces += force;
    }

    void apply_linear_impulse(Vec2 force)
    {
    }

    void update_position(double dt)
    {
        if(m_fixed) 
            return;

        m_old_pos = m_current_pos;
        m_velocity += (m_forces / m_mass) * dt; 
        m_current_pos += m_velocity * dt;
    }

    void update_acceleration(double dt)
    {
        m_accel = m_forces / (m_mass * dt);
    }

    void update_velocity(double dt)
    {
        if(dt == 0)
            return;
        m_velocity = (m_current_pos - m_old_pos) / dt;
        m_forces = {};
    }

    void accelerate(Vec2 acc)
    {
        m_accel += acc;
    }

    Vec2 pos() const { return m_current_pos; }
    Vec2 vel() const { return m_velocity; }
    Vec2 accel() const { return m_accel; }
    bool fixed() const { return m_fixed; }
    void set_fixed(bool fixed) { m_fixed = fixed; }
    double mass() const { return m_mass; }
    double inverse_mass() const 
    {
        if(m_mass == 0)
            return 0;
        return 1 / m_mass;
    }
};
