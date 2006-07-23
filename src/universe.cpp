/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/

#include "universe.h"
#include "object.h"

#include <list>

namespace amethyst
{

    void Universe::object_add(Object *new_object)
    {
        object_list.push_back(new_object);

    }

    void Universe::object_del(Object *new_object)
    {


    }

    void Universe::iterate(const double &time)
    {
        if(!object_list.empty())
        {
            // Do Gravity Calulations Here

            // Calculate Movement (and Attitude when it's ready)
            std::list<Object *>::iterator obj1 = object_list.begin();

            do
            {
                (*obj1)->iterate(time);

                obj1++;
            }  while (obj1 != object_list.end());

        }

    }


}
