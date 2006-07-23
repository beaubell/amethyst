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
            //void ~Universe(void);

            void object_add(Object *);
            void object_del(Object *);
            void object_del_all(void);

            unsigned int object_count();

            void iterate(const double &time);

        private:
            std::list<Object*>  object_list;

    };

}
