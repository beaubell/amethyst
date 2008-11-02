/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/

#include "universe.h"
#include "object.h"
#include "physics.h"

#include <list>
#include <iostream>

namespace amethyst {
namespace lib {

    Universe::Universe(void)
    {
        do_gravity_calc = true;

    }


    void Universe::object_add(Object *new_object)
    {
        if (new_object)
            object_list.push_back(new_object);

    }


    void Universe::object_del(const Object *del_object)
    {
        std::list<Object *>::iterator obj1 = object_list.begin();

        do
        {
             if( (*obj1) == del_object )
             {
                 object_list.erase(obj1);
                 break;
             }
             obj1++;
        }  while (obj1 != object_list.end());
    }


    Object* Universe::object_find(const std::string &name)
    {
        if(!object_list.empty())
        {
            std::list<Object *>::iterator obj1 = object_list.begin();
            do
            {
                if(name == (*obj1)->name)
                    return *obj1;

                obj1++;
            }  while (obj1 != object_list.end());
        }
        return NULL;
    }


    void Universe::iterate(const double &time)
    {
        if(!object_list.empty())
        {
            std::list<Object *>::iterator obj1;
            std::list<Object *>::iterator obj2;

            // Zero Forces out
            obj1 = object_list.begin();
            do
            {
                (*obj1)->force_clear();

                obj1++;
            } while (obj1 != object_list.end());

            // Do Gravity Calulation
            if (do_gravity_calc && (object_list.size() > 1) ) {
                obj1 = object_list.begin();
                obj2 = obj1;

                //std::cout << "Start" << std::endl;

                do
                {
                    obj2 = obj1;

                    do
                    {
                        obj2++;

                        if (obj2 == object_list.end()) break;

                        //std::cout << (*obj1)->name <<  (*obj2)->name << std::endl;

                        // Determine Vector
                        Cartesian_Vector vector = phys_vector ((*obj1)->location, (*obj2)->location);

                        // Convert to Spherical Vector
                        Spherical_Vector sphr = phys_alias_transform (vector);;

                        // Use radius value from spherical vector as distance
                        double distance = sphr.r;

                        // Replace radius with aggregate gravity force
                        sphr.r = phys_gravity((*obj1)->mass, (*obj2)->mass, distance);

                        // Add gravity as a force to first object
                        (*obj1)->force_add(sphr);

                        // Invert Spherical Vector for object 2
                        sphr = phys_vector_invert(sphr);

                        // Add inverted gravity vector as a force to second object
                        (*obj2)->force_add(sphr);
                    }  while (obj2 != object_list.end());

                    obj1++;
                }  while (obj1 != object_list.end());

            }

            // Calculate Movement and Attitude
            obj1 = object_list.begin();

            do
            {
                (*obj1)->iterate(time);

                obj1++;
            }  while (obj1 != object_list.end());

        } // If

    } // Iterate()

} // namespace lib
} // namespace amethyst
