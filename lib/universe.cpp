/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/

#include "universe.h"
#include "object.h"
#include "physics.h"
#include "utility.h"

#include <list>
#include <iostream>
#include <stdexcept>

#include <boost/timer/timer.hpp>

namespace amethyst {
namespace lib {

/// Constants
static const std::string SectionName("Amethyst Library (Universe)");

Universe::Universe(void)
: _using_cl(false),
  _timesteps(3125)
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


void Universe::iterate(const double &dtime)
{
  if (!_using_cl)
    iterate_cpu(dtime);
  else
  {
    std::vector<cl::Event> wait_queue, new_events;
    iterate_gpu(dtime, wait_queue, new_events);

    queue_rk4.finish();
    // Copy data back to linked list (XXX - Temporary - FIXME)
    cl_copyfrgpu();
  }
}


void Universe::iterate_gpu(const double &dtime,
                           std::vector<cl::Event> wait_queue,
                           std::vector<cl::Event> &new_events)
{
  cl::Event finpos_event, finvel_event;
  new_events.clear();

  unsigned int num_objects = object_count();

  std::cout << ".";
  std::cout.flush();

#if 0
  /// DEBUG
  {
    queue_rk4.finish();
    cl::Event debug_event;
    size_t objects = _object_list.size();
    std::size_t size_vec_mass = sizeof(float_type)*objects;
    std::size_t size_vec_loc = sizeof(Cartesian_Vector)*objects;
    std::size_t size_vec_vel = sizeof(Cartesian_Vector)*objects;
    std::size_t size_exp_acc = sizeof(Cartesian_Vector)*objects*(objects-1);
    std::vector<Cartesian_Vector> dump_vec;
    dump_vec.resize(objects);
    //queue.enqueueWriteBuffer(_cl_buf_velocity, CL_TRUE, 0, size_vec_vel, &dump_vec[0], NULL, &debug_event);
    queue_rk4.enqueueReadBuffer(_cl_buf_k2_dlocation, CL_TRUE, 0, size_vec_vel, &dump_vec[0], NULL, &debug_event);
    queue_rk4.finish();
    dumpVectorHDF5(std::string("dump_loc.hd5"), dump_vec);
    queue_rk4.enqueueReadBuffer(_cl_buf_k2_dvelocity, CL_TRUE, 0, size_vec_vel, &dump_vec[0], NULL, &debug_event);
    queue_rk4.finish();
    dumpVectorHDF5(std::string("dump_vel.hd5"), dump_vec);
  }
  /// /// /// ///
#endif

  // Make K1
  iterate_gpu_rk4_gravk(dtime, num_objects, _cl_buf_mass, _cl_buf_location, _cl_buf_velocity, _cl_buf_k1_dlocation, _cl_buf_k1_dvelocity, wait_queue, new_events);
  wait_queue = new_events;

  if(0) // 1st Order
  {
    iterate_gpu_rk4_scalesum(1.0, num_objects, _cl_buf_location, _cl_buf_velocity, _cl_buf_k1_dlocation,
                             _cl_buf_k1_dvelocity, _cl_buf_location, _cl_buf_velocity, wait_queue, new_events);
    
  }
  else // Full RK4 Hotness
  {
    /// Setup for K2
    iterate_gpu_rk4_scalesum(0.5, num_objects, _cl_buf_location, _cl_buf_velocity, _cl_buf_k1_dlocation,
                             _cl_buf_k1_dvelocity, _cl_buf_tmp_location, _cl_buf_tmp_velocity, wait_queue, new_events);
    wait_queue = new_events;

    /// Make K2 Now
    iterate_gpu_rk4_gravk(dtime, num_objects, _cl_buf_mass, _cl_buf_tmp_location, _cl_buf_tmp_velocity, _cl_buf_k2_dlocation, _cl_buf_k2_dvelocity, wait_queue, new_events);
    wait_queue = new_events;

    /// Setup for K3
    iterate_gpu_rk4_scalesum(0.5, num_objects, _cl_buf_location, _cl_buf_velocity, _cl_buf_k2_dlocation,
                             _cl_buf_k2_dvelocity, _cl_buf_tmp_location, _cl_buf_tmp_velocity, wait_queue, new_events);
    wait_queue = new_events;

    /// Make K3 Now
    iterate_gpu_rk4_gravk(dtime, num_objects, _cl_buf_mass, _cl_buf_tmp_location, _cl_buf_tmp_velocity, _cl_buf_k3_dlocation, _cl_buf_k3_dvelocity, wait_queue, new_events);
    wait_queue = new_events;

    /// Setup for K4
    iterate_gpu_rk4_scalesum(1.0, num_objects, _cl_buf_location, _cl_buf_velocity, _cl_buf_k3_dlocation,
                             _cl_buf_k3_dvelocity, _cl_buf_tmp_location, _cl_buf_tmp_velocity, wait_queue, new_events);
    wait_queue = new_events;

    /// Make K4 Now
    iterate_gpu_rk4_gravk(dtime, num_objects, _cl_buf_mass, _cl_buf_tmp_location, _cl_buf_tmp_velocity, _cl_buf_k4_dlocation, _cl_buf_k4_dvelocity, wait_queue, new_events);
    wait_queue = new_events;
    
    /// Sum it all together!
    // 
    kern_rk4_finalsum.setArg(0, _cl_buf_velocity);
    kern_rk4_finalsum.setArg(1, _cl_buf_k1_dvelocity);
    kern_rk4_finalsum.setArg(2, _cl_buf_k2_dvelocity);
    kern_rk4_finalsum.setArg(3, _cl_buf_k3_dvelocity);
    kern_rk4_finalsum.setArg(4, _cl_buf_k4_dvelocity);
    kern_rk4_finalsum.setArg(5, _cl_buf_velocity);
    queue_rk4.enqueueNDRangeKernel(kern_rk4_finalsum, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &finvel_event);

    kern_rk4_finalsum.setArg(0, _cl_buf_location);
    kern_rk4_finalsum.setArg(1, _cl_buf_k1_dlocation);
    kern_rk4_finalsum.setArg(2, _cl_buf_k2_dlocation);
    kern_rk4_finalsum.setArg(3, _cl_buf_k3_dlocation);
    kern_rk4_finalsum.setArg(4, _cl_buf_k4_dlocation);
    kern_rk4_finalsum.setArg(5, _cl_buf_location);
    queue_rk4.enqueueNDRangeKernel(kern_rk4_finalsum, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &finpos_event);

  }

  new_events.clear();
  new_events.push_back(finvel_event);
  new_events.push_back(finpos_event);
}

void Universe::iterate_gpu_rk4_gravk(const double &dtime,
                                     uint num_objects,
                                     cl::Buffer &masses,
                                     cl::Buffer &old_locations,
                                     cl::Buffer &old_velocities,
                                     cl::Buffer &new_dlocations,
                                     cl::Buffer &new_dvelocities,
                                     std::vector<cl::Event> wait_queue,
                                     std::vector<cl::Event> &events)
{
  cl::Event grav_event, kvel_event, kpos_event;
  events.clear();

  // [A] Run Gravity Calculation To Find Accelerations
  kern_rk4_grav.setArg(0, masses);
  kern_rk4_grav.setArg(1, old_locations);
  kern_rk4_grav.setArg(2, _cl_buf_expanded_acceleration);   // FIXME, Uses global buffer
  queue_rk4.enqueueNDRangeKernel(kern_rk4_grav, cl::NullRange, cl::NDRange(num_objects, num_objects-1), cl::NullRange, &wait_queue, &grav_event);

  // [C] Scale velocity by time to get change in position
  kern_rk4_scale.setArg(0, old_velocities);
  kern_rk4_scale.setArg(1, dtime);
  kern_rk4_scale.setArg(2, new_dlocations);
  queue_rk4.enqueueNDRangeKernel(kern_rk4_scale, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &kpos_event);
  
  // [B] Reduce(sum) expanded accelerations for each object and scale by time to determine change in velocity
  kern_rk4_reductionscale.setArg(0, _cl_buf_expanded_acceleration); // FIXME, Uses global buffer
  kern_rk4_reductionscale.setArg(1, dtime);
  kern_rk4_reductionscale.setArg(2, new_dvelocities);
  wait_queue.clear();
  wait_queue.push_back(grav_event);
  queue_rk4.enqueueNDRangeKernel(kern_rk4_reductionscale, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &kvel_event);

  events.push_back(kpos_event);
  events.push_back(kvel_event);
}


void Universe::iterate_gpu_rk4_scalesum(const double &scale,
                                        const uint num_objects,
                                        cl::Buffer &orig_location,
                                        cl::Buffer &orig_velocity,
                                        cl::Buffer &k_dlocation,
                                        cl::Buffer &k_dvelocity,
                                        cl::Buffer &new_location,
                                        cl::Buffer &new_velocity,
                                        std::vector<cl::Event> wait_queue,
                                        std::vector<cl::Event> &events)
{
  cl::Event tmpvel_event, tmppos_event;
  events.clear();

  if(scale != 1.0)
  {
    // [D] Sum velocity with scaled change in velocity
    kern_rk4_scalesum.setArg(0, orig_velocity);
    kern_rk4_scalesum.setArg(1, k_dvelocity);
    kern_rk4_scalesum.setArg(2, (double)scale);
    kern_rk4_scalesum.setArg(3, new_velocity);
    queue_rk4.enqueueNDRangeKernel(kern_rk4_scalesum, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &tmpvel_event);

    // [E] Sum position with scaled change in position
    kern_rk4_scalesum.setArg(0, orig_location);
    kern_rk4_scalesum.setArg(1, k_dlocation);
    kern_rk4_scalesum.setArg(2, (double)scale);
    kern_rk4_scalesum.setArg(3, new_location);
    queue_rk4.enqueueNDRangeKernel(kern_rk4_scalesum, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &tmppos_event);
  }
  else
  {
    // [D] Sum velocity with change in velocity
    kern_rk4_sum.setArg(0, orig_velocity);
    kern_rk4_sum.setArg(1, k_dvelocity);
    kern_rk4_sum.setArg(2, new_velocity);
    queue_rk4.enqueueNDRangeKernel(kern_rk4_sum, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &tmpvel_event);

    // [E] Sum position with change in position
    kern_rk4_sum.setArg(0, orig_location);
    kern_rk4_sum.setArg(1, k_dlocation);
    kern_rk4_sum.setArg(2, new_location);
    queue_rk4.enqueueNDRangeKernel(kern_rk4_sum, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &tmppos_event);
  }

  events.push_back(tmppos_event);
  events.push_back(tmpvel_event);
}



void Universe::iterate_cpu(const double &dtime)
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
      (*obj1)->iterate(dtime);

      obj1++;
    }  while (obj1 != _object_list.end());

  } // If
}

std::list<Object*>& Universe::list(void)
{
  return _object_list;

}


void Universe::cl_setup()
{
  size_t objects = _object_list.size();

  if(objects == 0)
    throw std::runtime_error("Wtf! Initializing OpenCL with no objects?");

  // Reserve space for transition vector objects since objects are currently stored as linked lists.
  _object_mass.reserve(objects);
  _object_position.reserve(objects);
  _object_velocity.reserve(objects);

  /// Primary CL space for objects
  std::size_t size_vec_mass = sizeof(float_type)*objects;
  std::size_t size_vec_loc = sizeof(Cartesian_Vector)*objects;
  std::size_t size_vec_vel = sizeof(Cartesian_Vector)*objects;
  std::size_t size_exp_acc = sizeof(Cartesian_Vector)*objects*(objects-1);
  std::size_t size_vec_histloc = sizeof(Cartesian_Vector)*objects*_timesteps;
  std::size_t size_vec_histvel = sizeof(Cartesian_Vector)*objects*_timesteps;

  //if (_cl_buf_mass) delete _cl_buf_mass;
  _cl_buf_mass     = cl::Buffer(amethyst_cl_context, CL_MEM_READ_ONLY,  size_vec_mass, NULL, NULL);
  //if (_cl_buf_location) delete _cl_buf_location;
  _cl_buf_location = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_loc,  NULL, NULL);
  //if (_cl_buf_velocity) delete _cl_buf_velocity;
  _cl_buf_velocity = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_vel,  NULL, NULL);


  /// Object History Vectors
  _cl_buf_hist_location = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_histloc,  NULL, NULL);
  _cl_buf_hist_velocity = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_histvel,  NULL, NULL);

  /// CL space for integration
  _cl_buf_expanded_acceleration = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_exp_acc,  NULL, NULL);

  _cl_buf_k1_dlocation = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_loc,  NULL, NULL);
  _cl_buf_k1_dvelocity = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_vel,  NULL, NULL);
  _cl_buf_k2_dlocation = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_loc,  NULL, NULL);
  _cl_buf_k2_dvelocity = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_vel,  NULL, NULL);
  _cl_buf_k3_dlocation = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_loc,  NULL, NULL);
  _cl_buf_k3_dvelocity = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_vel,  NULL, NULL);
  _cl_buf_k4_dlocation = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_loc,  NULL, NULL);
  _cl_buf_k4_dvelocity = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_vel,  NULL, NULL);
  _cl_buf_tmp_location = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_loc,  NULL, NULL);
  _cl_buf_tmp_velocity = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_vel,  NULL, NULL);
  _cl_buf_fin_location = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_loc,  NULL, NULL);
  _cl_buf_fin_velocity = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_vel,  NULL, NULL);

  /// Load CL kernels
  kern_rk4_sum      = cl_loadkernel(std::string("rk4_sum.cl"),      std::string("rk4_sum"));
  kern_rk4_grav     = cl_loadkernel(std::string("rk4_grav.cl"),     std::string("rk4_grav"));
  kern_rk4_scale    = cl_loadkernel(std::string("rk4_scale.cl"),    std::string("rk4_scale"));
  kern_rk4_copy3d   = cl_loadkernel(std::string("rk4_copy3d.cl"),   std::string("rk4_copy3d"));
  kern_rk4_scalesum = cl_loadkernel(std::string("rk4_scalesum.cl"), std::string("rk4_scalesum"));
  kern_rk4_finalsum = cl_loadkernel(std::string("rk4_finalsum.cl"), std::string("rk4_finalsum"));
  kern_rk4_reductionscale = cl_loadkernel(std::string("rk4_reductionscale.cl"), std::string("rk4_reductionscale"));

  /// Setup Command Queue
  unsigned int gpu_id = 0;  // FIXME - Make GPU_ID dynamic?
  //queue_rk4 = cl::CommandQueue(lib::amethyst_cl_context, lib::cl_devices[gpu_id], CL_QUEUE_PROFILING_ENABLE, NULL);
  queue_rk4 = cl::CommandQueue(lib::amethyst_cl_context, lib::cl_devices[gpu_id], NULL, NULL);
  
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

  cl::Event in_mass_event, in_location_event, in_velocity_event;

  //push our CPU arrays to the GPU
  err = queue_rk4.enqueueWriteBuffer(_cl_buf_mass,     CL_TRUE, 0, size_vec_mass,  &_object_mass[0],     NULL, &in_mass_event);
  err = queue_rk4.enqueueWriteBuffer(_cl_buf_location, CL_TRUE, 0, size_vec_loc,   &_object_position[0], NULL, &in_location_event);
  err = queue_rk4.enqueueWriteBuffer(_cl_buf_velocity, CL_TRUE, 0, size_vec_vel,   &_object_velocity[0], NULL, &in_velocity_event);

  queue_rk4.finish();
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

  cl::Event in_mass_event, in_location_event, in_velocity_event;

  //get our CPU arrays from the GPU
  err = queue_rk4.enqueueReadBuffer(_cl_buf_location, CL_TRUE, 0, size_vec_loc,   &_object_position[0], NULL, &in_location_event);
  err = queue_rk4.enqueueReadBuffer(_cl_buf_velocity, CL_TRUE, 0, size_vec_vel,   &_object_velocity[0], NULL, &in_velocity_event);

  queue_rk4.finish();

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

}

void Universe::cl_integrate()
{
  size_t num_objects = _object_list.size();
  cl::Event cpvel1_event, cppos1_event;
  std::vector<cl::Event> wait_queue, new_events;

  const double dtime = 86400.0; //seconds (1 day)

  // Start timer.  Reports elapsed time when destroyed.
  boost::timer::auto_cpu_timer t;

  // Copy buffer to 1st row in history
  kern_rk4_copy3d.setArg(0, _cl_buf_location);
  kern_rk4_copy3d.setArg(1, (unsigned int)0);
  kern_rk4_copy3d.setArg(2, _cl_buf_hist_location);
  kern_rk4_copy3d.setArg(3, (unsigned int)0);
  queue_rk4.enqueueNDRangeKernel(kern_rk4_copy3d, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &cppos1_event);

  kern_rk4_copy3d.setArg(0, _cl_buf_velocity);
  kern_rk4_copy3d.setArg(1, (unsigned int)0);
  kern_rk4_copy3d.setArg(2, _cl_buf_hist_velocity);
  kern_rk4_copy3d.setArg(3, (unsigned int)0);
  queue_rk4.enqueueNDRangeKernel(kern_rk4_copy3d, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &cpvel1_event);

  wait_queue.clear();
  wait_queue.push_back(cppos1_event);
  wait_queue.push_back(cpvel1_event);
  
  // Fill in rest of history
  for (unsigned int i = 1; i < _timesteps; i++)
  {
    cl::Event cpvel_event, cppos_event;
    
    // Iterate Engine
    iterate_gpu(dtime, wait_queue, new_events);
    wait_queue = new_events;

    // Copy buffer to ith row in history
    kern_rk4_copy3d.setArg(0, _cl_buf_location);
    kern_rk4_copy3d.setArg(1, (unsigned int)0);
    kern_rk4_copy3d.setArg(2, _cl_buf_hist_location);
    kern_rk4_copy3d.setArg(3, i);
    queue_rk4.enqueueNDRangeKernel(kern_rk4_copy3d, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &cppos_event);

    kern_rk4_copy3d.setArg(0, _cl_buf_velocity);
    kern_rk4_copy3d.setArg(1, (unsigned int)0);
    kern_rk4_copy3d.setArg(2, _cl_buf_hist_velocity);
    kern_rk4_copy3d.setArg(3, i);
    queue_rk4.enqueueNDRangeKernel(kern_rk4_copy3d, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &cpvel_event);

    wait_queue.clear();
    wait_queue.push_back(cppos_event);
    wait_queue.push_back(cpvel_event);
  }

  queue_rk4.finish();

  // Dump History to File for debugging
  {
    cl::Event debug_event;
    size_t objects = _object_list.size();
    std::size_t size_vec_loc = sizeof(Cartesian_Vector)*objects*_timesteps;
    std::size_t size_vec_vel = sizeof(Cartesian_Vector)*objects*_timesteps;
    std::vector<Cartesian_Vector> dump_vec;
    dump_vec.resize(objects*_timesteps);
    //queue.enqueueWriteBuffer(_cl_buf_velocity, CL_TRUE, 0, size_vec_vel, &dump_vec[0], NULL, &debug_event);
    queue_rk4.enqueueReadBuffer(_cl_buf_hist_location, CL_TRUE, 0, size_vec_vel, &dump_vec[0], NULL, &debug_event);
    queue_rk4.finish();
    dumpVectorHDF5(std::string("dump_loc.hd5"), dump_vec);
    queue_rk4.enqueueReadBuffer(_cl_buf_hist_velocity, CL_TRUE, 0, size_vec_vel, &dump_vec[0], NULL, &debug_event);
    queue_rk4.finish();
    dumpVectorHDF5(std::string("dump_vel.hd5"), dump_vec);
  }

}

} // namespace lib
} // namespace amethyst
