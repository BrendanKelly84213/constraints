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
        const double offset = m_preferred_distance - relative_distance;
        
        m_p->m_current_pos += 0.5f * offset * normal; 
        m_q->m_current_pos -= 0.5f * offset * normal; 

        m_p->update_position(dt);
        m_q->update_position(dt);
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
