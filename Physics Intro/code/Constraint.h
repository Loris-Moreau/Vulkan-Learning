#pragma once
#include "Math/Matrix.h"

class Body;

class Constraint
{
public:
    virtual void PreSolve ( const float dt_sec) {}
    virtual void Solve () {}
    virtual void PostSolve () {}
    
protected:
    MatMN GetInverseMassMatrix () const;
    VecN GetVelocities () const;
    void ApplyImpulses (const VecN & impulses);
    
public:
    Body * m_bodyA;
    Body * m_bodyB;
    Vec3 m_anchorA;   // The anchor location in bodyA’s space
    Vec3 m_axisA;    // The axis direction in bodyA’s space
    Vec3 m_anchorB; // The anchor location in bodyB’s space
    Vec3 m_axisB;  // The axis direction in bodyB’s space
};
