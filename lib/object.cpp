/*
   Physics Object, Definitions
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

// A superior man is modest in his speech, but exceeds in his actions.
//                                                          - Confucious

#include <stdlib.h>
#include <string>

#include "physics.h"
#include "object.h"


namespace amethyst {

    Object::Object()
    {
        mass = 1;

        next = NULL;
        meta = NULL;

        //printf("NEW OBJECT!");

    }

    Object::Object(const Object &right)
    {
        mass = right.mass;

        next = NULL;       // FIXME
        meta = right.meta; // FIXME, Do i want to copy?

        location     = right.location;
        velocity     = right.velocity;
        acceleration = right.acceleration;
        force        = right.force;

        // Rotation
        attitude             = right.attitude;
        angular_velocity     = right.angular_velocity;
        angular_acceleration = right.angular_acceleration;

        name = right.name;

    }

#if 0
    Object::~Object()
    {

        //printf("DEAD OBJECT");
    }
#endif

    void Object::force_add(Cartesian_Vector new_force)
    {
        force += new_force;
    }


    void Object::force_add(Spherical_Vector sphr_force)
    {
        Cartesian_Vector new_force = phys_alias_transform (sphr_force);

        force += new_force;
    }


    void Object::force_clear()
    {
        force.clear();
        acceleration.clear();
    }


    void Object::force_apply(void)
    {
        //velocity = phys_alibi_transform (velocity, acceleration, time);
        acceleration = force / mass;
    }


    void Object::accel_apply(double time)
    {
        Cartesian_Vector temp(acceleration * time);
        velocity += temp;
    }


    void Object::velocity_apply(double time)
    {
        Cartesian_Vector temp(velocity * time);
        location += temp;
    }


    void Object::iterate(double time)
    {

#if 0
        // Apply rotation tranformations;
        Quaternion acc_temp = angular_acceleration * time;
        angular_velocity *= acc_temp;

        Quaternion vel_temp = angular_velocity * time;
        attitude         *= vel_temp;
#endif
        // Apply location transformations
        force_apply();
        accel_apply(time);
        velocity_apply(time);
    }

}
