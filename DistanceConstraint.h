#pragma once

#include "PhysicsObject.h"

class DistanceConstraint {
private:
    PhysicsObject * m_p;
    PhysicsObject * m_q;
    double m_preferred_distance;
    double m_bias_factor;

public:
    DistanceConstraint() 
        : m_p(nullptr), m_q(nullptr), m_preferred_distance(0), m_bias_factor(0)
    {
    }

    DistanceConstraint(PhysicsObject * p, PhysicsObject * q, double preferred_distance, double bias_factor)
        : m_p(p), m_q(q), m_preferred_distance(preferred_distance), m_bias_factor(bias_factor)
    {
    }

    void update(double dt)
    {
        if(dt == 0)
            return;

        const Vec2 relative_pos = m_p->pos() - m_q->pos();
        const double relative_distance = relative_pos.magnitude();
        const Vec2 normal = relative_pos / relative_distance;
        const double offset = m_preferred_distance - relative_distance;

        if(relative_distance <= m_preferred_distance)
            return;

        if(std::abs(offset) > 0) {

            // --- Black Box ---
            // https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/8constraintsandsolvers/Physics%20-%20Constraints%20and%20Solvers.pdf
            float constraint_mass = m_p->inverse_mass() + m_q->inverse_mass();
            Vec2 relative_velocity = m_p->vel() - m_q->vel();
            float velocity_dot = Vec2::dot_product(relative_velocity, normal);
            float bias = -(m_bias_factor / dt) * offset;
            float lambda = -(velocity_dot + bias) / constraint_mass;

            Vec2 p_impulse = normal * lambda;
            Vec2 q_impulse = -1 * normal * lambda;
            // -----------------

            Vec2 p_force = p_impulse / dt;
            Vec2 q_force = q_impulse / dt;

            m_p->apply_force(p_force);
            m_q->apply_force(q_force);
        }
    }

    Vec2 p_pos() const 
    { 
        return m_p->pos(); 
    }

    Vec2 q_pos() const 
    { 
        return m_q->pos(); 
    }
};
