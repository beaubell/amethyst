/*
   Physics Object, Definitions
   Author: Beau V.C. Bellamy
*/

// A superior man is modest in his speech, but exceeds in his actions. 
//                                                          - Confucious

#ifdef __GNUG__
#pragma implementation
#endif

using namespace std;

#include <stdlib.h>
#include "physics.h"
#include "object.h"

Object::Object(){

    location.x = 0;
    location.y = 0;
    location.z = 0;

    velocity.x = 0;
    velocity.y = 0;
    velocity.z = 0;

    acceleration.x = 0;
    acceleration.y = 0;
    acceleration.z = 0;

    mass = 0; 

    next = NULL;

    //printf("NEW OBJECT!");

    }


Object::~Object(){

    //printf("DEAD OBJECT");
    }


void Object::force_add(Cartesian_Vector force) {

    acceleration = phys_alibi_transform (acceleration, force);
    }


void Object::force_add(Spherical_Vector force) {

    Cartesian_Vector new_force = phys_alias_transform (force);

    acceleration = phys_alibi_transform (acceleration, new_force);
    }

    
void Object::force_clear() {

    acceleration.x = 0;
    acceleration.y = 0;
    acceleration.z = 0;
    }

    
void Object::force_apply(double time) {
    
    velocity = phys_alibi_transform (velocity, acceleration, time);
    }


void Object::velocity_apply(double time) {

    location = phys_alibi_transform (location, velocity, time);
    }
