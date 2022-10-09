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
        const Vec2 relative_pos = m_p->pos() - m_q->pos();
        const double relative_distance = relative_pos.magnitude();
        const Vec2 normal = relative_pos / relative_distance;
        const double offset = relative_distance - m_preferred_distance;

        if(std::abs(offset) > 0) {
            Vec2 p = -offset / (m_p->mass() + m_q->mass()) * normal;
            if(!m_p->m_fixed)
                m_p->m_current_pos += (p / m_p->mass());
            if(!m_q->m_fixed)
                m_q->m_current_pos -= (p / m_q->mass());
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
