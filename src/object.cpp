/*
   Physics Object, Definitions
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

// A superior man is modest in his speech, but exceeds in his actions. 
//                                                          - Confucious

#include <stdlib.h>

#include "physics.h"
#include "object.h"

#ifdef __GNUG__
#pragma implementation
#endif

namespace amethyst {

Object::Object(){

    mass = 0; 

    next = NULL;
    meta = NULL;
    name = NULL;

    //printf("NEW OBJECT!");

    }


//Object::~Object(){

    //printf("DEAD OBJECT");
    //}


void Object::force_add(Cartesian_Vector new_force) {

    force += new_force;
    }


void Object::force_add(Spherical_Vector sphr_force) {

    Cartesian_Vector new_force = phys_alias_transform (sphr_force);

    force += new_force;
    }


void Object::force_clear() {

    force.Zeroize();
    acceleration.Zeroize();
    }


void Object::force_apply(void) {

    //velocity = phys_alibi_transform (velocity, acceleration, time);
    acceleration = force / mass;
    }

void Object::accel_apply(double time) {

    Cartesian_Vector temp(acceleration * time);
    velocity += temp;
    }

void Object::velocity_apply(double time) {

    Cartesian_Vector temp(velocity * time);
    location += temp;
    }

}
