#ifndef UNIVERSE_H
#define UNIVERSE_H

/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/

#include <list>

#include "types.h"
#include "object.h"
#include "cl.h"

#define BOOST_DISABLE_ASSERTS TRUE
#include "boost/multi_array.hpp"

typedef boost::multi_array<unsigned int , 2> Muir2DArrayUI;
typedef boost::multi_array<unsigned int , 3> Muir3DArrayUI;
typedef boost::multi_array<unsigned int , 4> Muir4DArrayUI;

typedef boost::multi_array<float , 2> Muir2DArrayF;
typedef boost::multi_array<float , 3> Muir3DArrayF;
typedef boost::multi_array<float , 4> Muir4DArrayF;

typedef boost::multi_array<double , 2> Muir2DArrayD;
typedef boost::multi_array<double , 3> Muir3DArrayD;
typedef boost::multi_array<double , 4> Muir4DArrayD;

typedef std::vector<int> PhaseCodeT;


namespace amethyst {
namespace lib {

    class Universe
    {
        public:
            Universe(void);
            //void ~Universe(void);

            bool do_gravity_calc;

            void object_add(      Object *);
            void object_del(const Object *);
            void object_del_all(void);

            Object*      object_find(const std::string &name);

            std::size_t object_count();

            void iterate(const double &time);

            void cl_setup();
            void cl_copytogpu();
            void cl_copyfrgpu();

            std::list<Object*>& list(void);

        //private: //FIXME Temporary fix for science
            std::list<Object*>  _object_list;

            std::vector<float_type> _object_mass;
            std::vector<Cartesian_Vector> _object_position;
            std::vector<Cartesian_Vector> _object_velocity;

            bool _using_cl;
            cl::Buffer *_cl_buf_mass;
            cl::Buffer *_cl_buf_location;
            cl::Buffer *_cl_buf_velocity;

    };

} // namespace lib
} // namespace amethyst

#endif // UNIVERSE_H
