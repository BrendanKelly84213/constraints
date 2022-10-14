#pragma once

#include "Vec2.h"

class PhysicsObject {
private:
    Vec2 m_current_pos;
    Vec2 m_old_pos;
    Vec2 m_velocity;
    Vec2 m_accel;
    Vec2 m_forces;
    float m_mass;
    int m_w;
    int m_h;
    bool m_fixed;
    size_t m_id;
    
public:
    PhysicsObject() = default;

    PhysicsObject(Vec2 pos, float mass) 
        : m_current_pos(pos), m_old_pos(pos), m_accel({0, 0}), m_forces({0, 0}), m_mass(mass), m_w(0), m_h(0), m_fixed(false), m_id(0)
    {
    }

    PhysicsObject(Vec2 pos, float mass, bool fixed) 
        : m_current_pos(pos), m_old_pos(pos), m_accel({0, 0}), m_forces({0, 0}), m_mass(mass), m_w(0), m_h(0), m_fixed(fixed), m_id(0)
    {
    }

    PhysicsObject(Vec2 pos, Vec2 accel, float mass, int w, int h) 
        : m_current_pos(pos), m_old_pos(pos), m_accel(accel), m_mass(mass), m_w(w), m_h(h), m_fixed(false), m_id(0)
    {
    }

    void apply_force(Vec2 force)
    {
        m_forces += force;
    }

    void update_position(float dt)
    {
        if(m_fixed) 
            return;

        m_old_pos = m_current_pos;
        m_velocity += (m_forces / m_mass) * dt; 
        m_current_pos += m_velocity * dt;
    }

    void update_acceleration(float dt)
    {
        m_accel = m_forces / (m_mass * dt);
    }

    void update_velocity(float dt)
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

    void move(Vec2 velocity)
    {
        if(m_fixed)
            return;
        m_current_pos += velocity;
    }

    void move_to(Vec2 position)
    {
        m_current_pos = position;
    }

    Vec2 pos() const { return m_current_pos; }
    Vec2 vel() const { return m_velocity; }
    Vec2 accel() const { return m_accel; }
    bool fixed() const { return m_fixed; }
    void set_fixed(bool fixed) { m_fixed = fixed; }
    float mass() const { return m_mass; }
    float inverse_mass() const 
    {
        if(m_mass == 0)
            return 0;
        return 1 / m_mass;
    }
    size_t id() const { return m_id; }
    void set_id(size_t id) { m_id = id; }
};
