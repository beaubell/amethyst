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

    /// Constants
    static const std::string SectionName("Amethyst Library (Universe)");

    Universe::Universe(void)
    : _using_cl(false)
      //_cl_buf_mass(NULL),
      //_cl_buf_location(NULL),
      //_cl_buf_velocity(NULL)
    {
        do_gravity_calc = true;

    }


    void Universe::object_add(Object *new_object)
    {
        if (_using_cl)
          throw (std::runtime_error("Adding objects after OpenCL runtime is initialized is not supported!"));
        
        if (new_object)
            _object_list.push_back(new_object);

    }


    void Universe::object_del(const Object *del_object)
    {
        std::list<Object *>::iterator obj1 = _object_list.begin();

        do
        {
             if( (*obj1) == del_object )
             {
                 _object_list.erase(obj1);
                 break;
             }
             obj1++;
        }  while (obj1 != _object_list.end());
    }


    Object* Universe::object_find(const std::string &name)
    {
        if(!_object_list.empty())
        {
            std::list<Object *>::iterator obj1 = _object_list.begin();
            do
            {
                if(name == (*obj1)->name)
                    return *obj1;

                obj1++;
            }  while (obj1 != _object_list.end());
        }
        return NULL;
    }

    std::size_t Universe::object_count()
    {
        return _object_list.size();
    }
    
    void Universe::iterate(const double &time)
    {
        if(!_object_list.empty())
        {
            std::list<Object *>::iterator obj1;
            std::list<Object *>::iterator obj2;

            // Zero Forces out
            obj1 = _object_list.begin();
            do
            {
                (*obj1)->force_clear();

                obj1++;
            } while (obj1 != _object_list.end());

            // Do Gravity Calulation
            if (do_gravity_calc && (_object_list.size() > 1) ) {
                obj1 = _object_list.begin();
                obj2 = obj1;

                //std::cout << "Start" << std::endl;

                do
                {
                    obj2 = obj1;

                    do
                    {
                        obj2++;

                        if (obj2 == _object_list.end()) break;

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
                    }  while (obj2 != _object_list.end());

                    obj1++;
                }  while (obj1 != _object_list.end());

            }

            // Calculate Movement and Attitude
            obj1 = _object_list.begin();

            do
            {
                (*obj1)->iterate(time);

                obj1++;
            }  while (obj1 != _object_list.end());

        } // If

    } // Iterate()

    std::list<Object*>& Universe::list(void)
    {
        return _object_list;

    }

    void Universe::cl_setup()
    {
        size_t objects = _object_list.size();

        if(objects == 0)
          throw std::runtime_error("Wtf! Initializing OpenCL with no objects?");

        cl_int err = CL_SUCCESS;

        std::size_t size_vec_mass = sizeof(float_type)*objects;
        std::size_t size_vec_loc = sizeof(Cartesian_Vector)*objects;
        std::size_t size_vec_vel = sizeof(Cartesian_Vector)*objects;

        _object_mass.reserve(objects);
        _object_position.reserve(objects);
        _object_velocity.reserve(objects);

        std::cout << size_vec_mass;
        
        //if (_cl_buf_mass) delete _cl_buf_mass;
        _cl_buf_mass     = cl::Buffer(amethyst_cl_context, CL_MEM_READ_ONLY,  size_vec_mass, NULL, &err);
        //if (_cl_buf_location) delete _cl_buf_location;
        _cl_buf_location = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_loc,  NULL, &err);
        //if (_cl_buf_velocity) delete _cl_buf_velocity;
        _cl_buf_velocity = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_vel,  NULL, &err);

        //load kernels for dynamics
        
        _using_cl = true;
    }

    void Universe::cl_copytogpu()
    {
        if (!_using_cl)
          throw (std::runtime_error("Cannot copy data to GPU.  Cl is not initialized"));

        // FIXME FIll out buffer before this step.
        size_t objects = _object_list.size();
        std::list<Object *>::iterator obj = _object_list.begin();

        for (std::size_t i = 0; i < objects; i++)
        {
            if(obj == _object_list.end())
              throw std::runtime_error("Object lists are inconsistent!");

            _object_mass[i]     = (*obj)->mass;
            _object_position[i] = (*obj)->location;
            _object_velocity[i] = (*obj)->velocity;

            obj++;
        }


        // Send buffer to GPU
        cl_int err = CL_SUCCESS;
        std::size_t size_vec_mass = sizeof(float_type)*objects;
        std::size_t size_vec_loc = sizeof(Cartesian_Vector)*objects;
        std::size_t size_vec_vel = sizeof(Cartesian_Vector)*objects;

        int gpu_id = 0;
        cl::Event in_mass_event, in_location_event, in_velocity_event;
        cl::CommandQueue queue(amethyst_cl_context, cl_devices[gpu_id], CL_QUEUE_PROFILING_ENABLE, &err);

        //push our CPU arrays to the GPU
        err = queue.enqueueWriteBuffer(_cl_buf_mass,     CL_TRUE, 0, size_vec_mass,  &_object_mass[0],     NULL, &in_mass_event);
        err = queue.enqueueWriteBuffer(_cl_buf_location, CL_TRUE, 0, size_vec_loc,   &_object_position[0], NULL, &in_location_event);
        err = queue.enqueueWriteBuffer(_cl_buf_velocity, CL_TRUE, 0, size_vec_vel,   &_object_velocity[0], NULL, &in_velocity_event);

        queue.finish();
    }


    void Universe::cl_copyfrgpu()
    {
      if (!_using_cl)
          throw (std::runtime_error("Cannot copy data to GPU.  Cl is not initialized"));

        cl_int err = CL_SUCCESS;
        size_t objects = _object_list.size();

        //std::size_t size_vec_mass = sizeof(float_type)*objects;
        std::size_t size_vec_loc = sizeof(Cartesian_Vector)*objects;
        std::size_t size_vec_vel = sizeof(Cartesian_Vector)*objects;

        int gpu_id = 0;
        cl::Event in_mass_event, in_location_event, in_velocity_event;
        cl::CommandQueue queue(amethyst_cl_context, cl_devices[gpu_id], CL_QUEUE_PROFILING_ENABLE, &err);

        //get our CPU arrays from the GPU
        //err = queue.enqueueWriteBuffer(*_cl_buf_mass,     CL_TRUE, 0, size_vec_mass,  &_object_mass[0],     NULL, &in_mass_event);
        err = queue.enqueueReadBuffer(_cl_buf_location, CL_TRUE, 0, size_vec_loc,   &_object_position[0], NULL, &in_location_event);
        err = queue.enqueueReadBuffer(_cl_buf_velocity, CL_TRUE, 0, size_vec_vel,   &_object_velocity[0], NULL, &in_velocity_event);

        queue.finish();

        std::list<Object *>::iterator obj = _object_list.begin();

        for (std::size_t i = 0; i < objects; i++)
        {
            if(obj == _object_list.end())
              throw std::runtime_error("Object lists are inconsistent!");

            (*obj)->mass     = _object_mass[i];
            (*obj)->location = _object_position[i];
            (*obj)->velocity = _object_velocity[i];

            obj++;
        }


        // Send buffer to GPU
        
    }

} // namespace lib
} // namespace amethyst
