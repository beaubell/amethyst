#ifndef UNIVERSE_H
#define UNIVERSE_H

/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/

#include <list>

#include "object.h"


namespace amethyst {

    class Universe
    {
        public:
            Universe(void);
            //void ~Universe(void);

            bool do_gravity_calc;

            void object_add(      Object *);
            void object_del(const Object *);
            void object_del_all(void);

            unsigned int object_count();

            void iterate(const double &time);

        private:
            std::list<Object*>  object_list;

    };

}

#endif // UNIVERSE_H
