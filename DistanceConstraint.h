#pragma once

#include "PhysicsObject.h"

class DistanceConstraint {
private:
    PhysicsObject * m_p;
    PhysicsObject * m_q;
    double m_preferred_distance;
    double m_bias_factor;

public:
    DistanceConstraint(PhysicsObject * p, PhysicsObject * q, double preferred_distance, double bias_factor)
        : m_p(p), m_q(q), m_preferred_distance(preferred_distance), m_bias_factor(bias_factor)
    {
    }
    
    void update(double dt)
    {
        const Vec2 relative_pos = m_q->pos() - m_p->pos();
        const Vec2 relative_vel = m_p->vel() - m_q->vel();
        const double relative_distance = relative_pos.magnitude();
        const double offset = relative_distance - m_preferred_distance;

        if(std::abs(offset) > 0.0f && dt > 0) {
            Vec2 offset_direction = relative_pos.normal();

            const double velocity_dot = Vec2::dot_product(relative_vel, offset_direction);
            const double bias = -(m_bias_factor / dt) * offset;

            const double lambda = -(velocity_dot + bias) / (m_p->inverse_mass() + m_q->inverse_mass());

            const Vec2 p_impulse = offset_direction * lambda;
            const Vec2 q_impulse = -1 * (offset_direction * lambda);

            m_p->apply_linear_impulse(p_impulse); 
            m_q->apply_linear_impulse(q_impulse);
        }
        
        m_p->update(dt);
        m_q->update(dt);
    }
};
