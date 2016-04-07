/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/

#include "universe.h"
#include "object.h"
#include "physics.h"
#include "utility.h"
#include "io_hdf5.h"

#include <list>
#include <iostream>
#include <stdexcept>

#include <boost/timer/timer.hpp>

namespace amethyst {
namespace lib {

/// Constants
static const std::string SectionName("Amethyst Library (Universe)");

Universe::Universe(void)
: do_gravity_calc(true),
  gpu_rk4_4thorder(true),
  _using_cl(false),
  _timesteps(3125),
  mass_cutoff(1e5)
{

  _hist_time.resize(_timesteps);
}


void Universe::object_add(Object::sptr new_object)
{
  if (_using_cl)
    throw (std::runtime_error("Adding objects after OpenCL runtime is initialized is not supported!"));

  if (new_object)
    _object_list.push_back(new_object);

}


void Universe::object_del(const Object::sptr del_object)
{
  auto obj1 = _object_list.begin();

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


Object::sptr Universe::object_find(const std::string &name)
{
  if(!_object_list.empty())
  {
      auto obj1 = _object_list.begin();

      do
      {
        if(name == (*obj1)->name)
            return *obj1;

        obj1++;
      }  while (obj1 != _object_list.end());
  }
  return Object::sptr();
}


std::size_t Universe::object_count()
{
  return _object_list.size();
}


void Universe::toggle_gravity()
{
  do_gravity_calc = !do_gravity_calc;

  std::cout << "Gravity " << std::string((do_gravity_calc)?"Activated":"Deactivated") << std::string(".") << std::endl;
}


void Universe::toggle_4thorder()
{ 
  gpu_rk4_4thorder = !gpu_rk4_4thorder;

  std::cout << "Integration switched to " << std::string((gpu_rk4_4thorder)?"4th":"1st") << std::string(" order.") << std::endl;
 
}


void Universe::iterate(const double &dtime,
                       uint stride)
{
  std::vector<cl::Event> wait_queue, new_events;

  iterate(dtime, stride, wait_queue, new_events);

  // Since we don't return events, go ahead and wait for the queue to finish before returning.
  if (_using_cl)
  {
    queue_rk4.finish();
    // Copy data back to linked list (XXX - Temporary - FIXME)
    cl_copyfrgpu();
  }
}


void Universe::iterate(const double &dtime,
                       uint stride,
                       std::vector<cl::Event> wait_queue,
                       std::vector<cl::Event> &new_events
                       )
{
  new_events.clear();

  if (!_using_cl)
  {
    iterate_cpu(dtime);
    _current.time += dtime;
  }
  else
  {
    for (uint i = 0; i < stride; i++)
    {
      iterate_gpu(dtime, wait_queue, new_events);
      wait_queue = new_events;
    }
  }
}


void Universe::iterate_gpu(const double &dtime,
                           std::vector<cl::Event> wait_queue,
                           std::vector<cl::Event> &new_events)
{
  new_events.clear();

  unsigned int num_objects = object_count();

  /// Make K1
  iterate_gpu_rk4_gravk(dtime, num_objects, _cl_buf_mass, _current, _k1, wait_queue, new_events);
  wait_queue = new_events;

  if(!gpu_rk4_4thorder) // 1st Order
  {
    iterate_gpu_rk4_scalesum(1.0, num_objects, _current, _k1, _current, wait_queue, new_events);
  }
  else // Full RK4 Hotness
  {
    /// Setup for K2
    iterate_gpu_rk4_scalesum(0.5, num_objects, _current, _k1, _tmp, wait_queue, new_events);
    wait_queue = new_events;

    /// Make K2
    iterate_gpu_rk4_gravk(dtime, num_objects, _cl_buf_mass, _tmp, _k2, wait_queue, new_events);
    wait_queue = new_events;

    /// Setup for K3
    iterate_gpu_rk4_scalesum(0.5, num_objects, _current, _k2, _tmp, wait_queue, new_events);
    wait_queue = new_events;

    /// Make K3
    iterate_gpu_rk4_gravk(dtime, num_objects, _cl_buf_mass, _tmp, _k3, wait_queue, new_events);
    wait_queue = new_events;

    /// Setup for K4
    iterate_gpu_rk4_scalesum(1.0, num_objects, _current, _k3, _tmp, wait_queue, new_events);
    wait_queue = new_events;

    /// Make K4
    iterate_gpu_rk4_gravk(dtime, num_objects, _cl_buf_mass, _tmp, _k4, wait_queue, new_events);
    wait_queue = new_events;
    
    /// Sum it all together!
    iterate_gpu_rk4_finalsum(num_objects, _current, _k1, _k2, _k3, _k4, _current, wait_queue, new_events);
  }

  /// Add time step in current buffer.
  // This is probably not asynchronously safe...  but I don't want to waste GPU cycles for it.
  _current.time += dtime;
}


// Performs an N-body gravitational acceleration pass as well as a time step.
void Universe::iterate_gpu_rk4_gravk(const double &dtime,
                                     uint num_objects,
                                     cl::Buffer &masses,
                                     Object_Group &old,
                                     Object_Group &new_d,
                                     std::vector<cl::Event> wait_queue,
                                     std::vector<cl::Event> &events)
{
  cl::Event grav_event, kvel_event, kpos_event;
  events.clear();

  size_t objects_sig = old.get_sigsize();

  if(do_gravity_calc)
  {
    // [A] Run Gravity Calculation To Find Accelerations
    kern_rk4_grav.setArg(0, masses);
    kern_rk4_grav.setArg(1, old.location);
    kern_rk4_grav.setArg(2, _cl_buf_expanded_acceleration);   // FIXME, Uses global buffer
    queue_rk4.enqueueNDRangeKernel(kern_rk4_grav, cl::NullRange, cl::NDRange(num_objects, objects_sig), cl::NullRange, &wait_queue, &grav_event);

    if(0)
    {
    queue_rk4.finish();
    cl::Event pos_event, acc_event;
    size_t num_objects = _object_list.size();

    Am2DArrayD position_data(boost::extents[num_objects][4]);
    Am3DArrayD accel_data(boost::extents[num_objects][objects_sig][4]);

    size_t pos_data_size    = position_data.num_elements()*sizeof(float_type);
    size_t acc_data_size    = accel_data.num_elements()*sizeof(float_type);

    HDF5 h5file(std::string("acceleration_dump.h5"), H5F_ACC_TRUNC);
    h5file.createGroup(std::string("/Dump"));

    queue_rk4.enqueueReadBuffer(old.location, CL_TRUE, 0, pos_data_size, position_data.data(), NULL, &pos_event);
    pos_event.wait();
    h5file.write_2D_double(std::string("/Dump/Position"), position_data);

    queue_rk4.enqueueReadBuffer(_cl_buf_expanded_acceleration, CL_TRUE, 0, acc_data_size, accel_data.data(), NULL, &acc_event);
    acc_event.wait();
    h5file.write_3D_double(std::string("/Dump/Acceleration"), accel_data);

    h5file.close();
    throw std::logic_error("Normal Termination for Debugging...");
    }
  }

    // [C] Scale velocity by time to get change in position
    kern_rk4_scale.setArg(0, old.velocity);
    kern_rk4_scale.setArg(1, dtime);
    kern_rk4_scale.setArg(2, new_d.location);
    queue_rk4.enqueueNDRangeKernel(kern_rk4_scale, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &kpos_event);

  if(do_gravity_calc)
  {
    // [B] Reduce(sum) expanded accelerations for each object and scale by time to determine change in velocity
    kern_rk4_reductionscale.setArg(0, (unsigned int)objects_sig);
    kern_rk4_reductionscale.setArg(1, _cl_buf_expanded_acceleration); // FIXME, Uses global buffer
    kern_rk4_reductionscale.setArg(2, dtime);
    kern_rk4_reductionscale.setArg(3, new_d.velocity);
    wait_queue.clear();
    wait_queue.push_back(grav_event);
    queue_rk4.enqueueNDRangeKernel(kern_rk4_reductionscale, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &kvel_event);
    events.push_back(kvel_event);
  }

  events.push_back(kpos_event);
}


// Performs a multiply-sum of the k_d group and sums it wht orig
void Universe::iterate_gpu_rk4_scalesum(const double &scale,
                                        const uint num_objects,
                                        Object_Group &orig,
                                        Object_Group &k_d,
                                        Object_Group &new_g,
                                        std::vector<cl::Event> wait_queue,
                                        std::vector<cl::Event> &events)
{
  cl::Event tmpvel_event, tmppos_event;
  events.clear();

  if(scale != 1.0)
  {
    if(do_gravity_calc)
    {
      // [D] Sum velocity with scaled change in velocity
      kern_rk4_scalesum.setArg(0, orig.velocity);
      kern_rk4_scalesum.setArg(1, k_d.velocity);
      kern_rk4_scalesum.setArg(2, (double)scale);
      kern_rk4_scalesum.setArg(3, new_g.velocity);
      queue_rk4.enqueueNDRangeKernel(kern_rk4_scalesum, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &tmpvel_event);
      events.push_back(tmpvel_event);
    }

    // [E] Sum position with scaled change in position
    kern_rk4_scalesum.setArg(0, orig.location);
    kern_rk4_scalesum.setArg(1, k_d.location);
    kern_rk4_scalesum.setArg(2, (double)scale);
    kern_rk4_scalesum.setArg(3, new_g.location);
    queue_rk4.enqueueNDRangeKernel(kern_rk4_scalesum, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &tmppos_event);
  }
  else
  {
    if(do_gravity_calc)
    {
      // [D] Sum velocity with change in velocity
      kern_rk4_sum.setArg(0, orig.velocity);
      kern_rk4_sum.setArg(1, k_d.velocity);
      kern_rk4_sum.setArg(2, new_g.velocity);
      queue_rk4.enqueueNDRangeKernel(kern_rk4_sum, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &tmpvel_event);
      events.push_back(tmpvel_event);
    }

    // [E] Sum position with change in position
    kern_rk4_sum.setArg(0, orig.location);
    kern_rk4_sum.setArg(1, k_d.location);
    kern_rk4_sum.setArg(2, new_g.location);
    queue_rk4.enqueueNDRangeKernel(kern_rk4_sum, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &tmppos_event);
  }

  events.push_back(tmppos_event);
}



// Performs the final weighted average of the RK4 algorithm
void Universe::iterate_gpu_rk4_finalsum(const uint num_objects,
                                        Object_Group &orig,
                                        Object_Group &k1,
                                        Object_Group &k2,
                                        Object_Group &k3,
                                        Object_Group &k4,
                                        Object_Group &new_objs,
                                        std::vector<cl::Event> wait_queue,
                                        std::vector<cl::Event> &new_events)
{
  cl::Event finpos_event, finvel_event;
  new_events.clear();

  kern_rk4_finalsum.setArg(0, orig.velocity);
  kern_rk4_finalsum.setArg(1, k1.velocity);
  kern_rk4_finalsum.setArg(2, k2.velocity);
  kern_rk4_finalsum.setArg(3, k3.velocity);
  kern_rk4_finalsum.setArg(4, k4.velocity);
  kern_rk4_finalsum.setArg(5, new_objs.velocity);
  queue_rk4.enqueueNDRangeKernel(kern_rk4_finalsum, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &finvel_event);

  kern_rk4_finalsum.setArg(0, orig.location);
  kern_rk4_finalsum.setArg(1, k1.location);
  kern_rk4_finalsum.setArg(2, k2.location);
  kern_rk4_finalsum.setArg(3, k3.location);
  kern_rk4_finalsum.setArg(4, k4.location);
  kern_rk4_finalsum.setArg(5, new_objs.location);
  queue_rk4.enqueueNDRangeKernel(kern_rk4_finalsum, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &finpos_event);

  new_events.push_back(finvel_event);
  new_events.push_back(finpos_event);
}



// Copies current data to history buffer by index number
void Universe::iterate_gpu_tohistory(const uint num_objects,
                                     Object_Group &current,
                                     const uint index,
                                     std::vector<cl::Event> wait_queue,
                                     std::vector<cl::Event> &new_events)
{
  new_events.clear();
  cl::Event cpvel_event, cppos_event;
  
  kern_rk4_copyrot.setArg(0, current.location);
  kern_rk4_copyrot.setArg(1, (unsigned int)0);
  kern_rk4_copyrot.setArg(2, (unsigned int)1);  // FIXME Earth as reference - Hardcoded
  kern_rk4_copyrot.setArg(3, _cl_buf_hist_location);
  kern_rk4_copyrot.setArg(4, index);
  queue_rk4.enqueueNDRangeKernel(kern_rk4_copyrot, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &cppos_event);

//   kern_rk4_copy3d.setArg(0, current.location);
//   kern_rk4_copy3d.setArg(1, (unsigned int)0);
//   kern_rk4_copy3d.setArg(2, _cl_buf_hist_location);
//   kern_rk4_copy3d.setArg(3, index);
//   queue_rk4.enqueueNDRangeKernel(kern_rk4_copy3d, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &cppos_event);

  kern_rk4_copy3d.setArg(0, current.velocity);
  kern_rk4_copy3d.setArg(1, (unsigned int)0);
  kern_rk4_copy3d.setArg(2, _cl_buf_hist_velocity);
  kern_rk4_copy3d.setArg(3, index);
  queue_rk4.enqueueNDRangeKernel(kern_rk4_copy3d, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &cpvel_event);

  new_events.push_back(cppos_event);
  new_events.push_back(cpvel_event);

  // Copy time to history buffer
  _hist_time[index] = current.time;
}



// Copies history data by index to the current data set
void Universe::iterate_gpu_frhistory(const uint num_objects,
                                     Object_Group &current,
                                     const uint index,
                                     std::vector<cl::Event> wait_queue,
                                     std::vector<cl::Event> &new_events)
{
  new_events.clear();
  cl::Event cpvel_event, cppos_event;

  kern_rk4_copy3d.setArg(0, _cl_buf_hist_location);
  kern_rk4_copy3d.setArg(1, index);
  kern_rk4_copy3d.setArg(2, current.location);
  kern_rk4_copy3d.setArg(3, (unsigned int)0);
  queue_rk4.enqueueNDRangeKernel(kern_rk4_copy3d, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &cppos_event);

  kern_rk4_copy3d.setArg(0, _cl_buf_hist_velocity);
  kern_rk4_copy3d.setArg(1, index);
  kern_rk4_copy3d.setArg(2, current.velocity);
  kern_rk4_copy3d.setArg(3, (unsigned int)0);
  queue_rk4.enqueueNDRangeKernel(kern_rk4_copy3d, cl::NullRange, cl::NDRange(num_objects), cl::NullRange, &wait_queue, &cpvel_event);

  new_events.push_back(cppos_event);
  new_events.push_back(cpvel_event);

  // Copy time from history buffer
  current.time = _hist_time[index];
}



// Old 1st Order CPU method for integrating the positions.
void Universe::iterate_cpu(const double &dtime)
{
  if(!_object_list.empty())
  {
    std::list<Object::sptr>::iterator obj1;
    std::list<Object::sptr>::iterator obj2;

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

std::list<Object::sptr>& Universe::list(void)
{
  return _object_list;

}


void Universe::cl_setup()
{
  size_t objects = _object_list.size();
  size_t objects_sig = count_sig_objects();
  size_t objects_insig = objects - objects_sig;
  
  std::cout << "Significant Objects"   << objects_sig << std::endl;
  std::cout << "Insignificant Objects" << objects_insig << std::endl;

  if(objects == 0)
    throw std::runtime_error("Wtf! Initializing OpenCL with no objects?");

  // Reserve space for transition vector objects since objects are currently stored as linked lists.
  _object_mass.reserve(objects);
  _object_position.reserve(objects);
  _object_velocity.reserve(objects);

  /// Primary CL space for objects
  std::size_t size_vec_mass = sizeof(float_type)*objects;
  std::size_t size_exp_acc = sizeof(Cartesian_Vector)*objects*(objects_sig);
  std::size_t size_vec_histloc = sizeof(Cartesian_Vector)*objects*_timesteps;
  std::size_t size_vec_histvel = sizeof(Cartesian_Vector)*objects*_timesteps;
  std::size_t size_vec_histdis = sizeof(float_type)*objects*_timesteps;

  _cl_buf_mass     = cl::Buffer(amethyst_cl_context, CL_MEM_READ_ONLY,  size_vec_mass, NULL, NULL);
  _current.set_size(objects_sig, objects_insig);


  /// Object History Vectors
  _cl_buf_hist_location = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_histloc, NULL, NULL);
  _cl_buf_hist_velocity = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_histvel, NULL, NULL);
  _cl_buf_hist_distance = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_histdis, NULL, NULL);

  /// CL space for integration
  _cl_buf_expanded_acceleration = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_exp_acc,  NULL, NULL);

  _k1.set_size(objects_sig, objects_insig);
  _k2.set_size(objects_sig, objects_insig);
  _k3.set_size(objects_sig, objects_insig);
  _k4.set_size(objects_sig, objects_insig);
  _tmp.set_size(objects_sig, objects_insig);
  _final.set_size(objects_sig, objects_insig);

  /// Load CL kernels
  kern_rk4_sum      = cl_loadkernel(std::string("rk4_sum.cl"),      std::string("rk4_sum"));
  kern_rk4_grav     = cl_loadkernel(std::string("rk4_grav.cl"),     std::string("rk4_grav"));
  kern_rk4_scale    = cl_loadkernel(std::string("rk4_scale.cl"),    std::string("rk4_scale"));
  kern_rk4_copy3d   = cl_loadkernel(std::string("rk4_copy3d.cl"),   std::string("rk4_copy3d"));
  kern_rk4_copyrot  = cl_loadkernel(std::string("rk4_copyrot.cl"),   std::string("rk4_copyrot"));
  kern_rk4_scalesum = cl_loadkernel(std::string("rk4_scalesum.cl"), std::string("rk4_scalesum"));
  kern_rk4_finalsum = cl_loadkernel(std::string("rk4_finalsum.cl"), std::string("rk4_finalsum"));
  kern_rk4_reductionscale = cl_loadkernel(std::string("rk4_reductionscale.cl"), std::string("rk4_reductionscale"));

  kern_dist = cl_loadkernel(std::string("histdist.cl"), std::string("histdist"));
  

  /// Setup Command Queue
  unsigned int gpu_id = 0;  // FIXME - Make GPU_ID dynamic?
  //queue_rk4 = cl::CommandQueue(lib::amethyst_cl_context, lib::cl_devices[gpu_id], CL_QUEUE_PROFILING_ENABLE, NULL);
  uint queue_options = CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
  queue_rk4 = cl::CommandQueue(lib::amethyst_cl_context, lib::cl_devices[gpu_id], queue_options, NULL);
  
  _using_cl = true;
}


void Universe::cl_copytogpu()
{
  sort_objects();
  
  uint sig_objs = _current.get_sigsize();
  uint idx_sig = 0;
  uint idx_insig = 0;

  std::cout << "Sigsize: " << sig_objs << "  Mass Cutoff: " << mass_cutoff << std::endl;
  
  if (!_using_cl)
    throw (std::runtime_error("Cannot copy data to GPU.  Cl is not initialized"));

  // FIXME FIll out buffer before this step.
  size_t objects = _object_list.size();
  auto obj = _object_list.begin();

  for (std::size_t i = 0; i < objects; i++)
  {
    uint idx = 0;

    if(obj == _object_list.end())
      throw std::runtime_error("Object lists are inconsistent!");

    // Sort Insignificant/Significant Mass Objects
    if ( ((*obj)->mass) > mass_cutoff)
    {
      idx = idx_sig;
      idx_sig++;
    }
    else
    {
      idx = sig_objs + idx_insig;
      idx_insig++;
    }

    _object_mass[idx]     = (*obj)->mass;
    _object_position[idx] = (*obj)->location;
    _object_velocity[idx] = (*obj)->velocity;

    std::cout << "Index: " << idx << " (" << (*obj)->name << ") Mass: " << (*obj)->mass << std::endl;

    obj++;
  }

  // Send buffer to GPU
  std::size_t size_vec_mass = sizeof(float_type)*objects;
  std::size_t size_vec_loc = sizeof(Cartesian_Vector)*objects;
  std::size_t size_vec_vel = sizeof(Cartesian_Vector)*objects;

  cl::Event in_mass_event, in_location_event, in_velocity_event;

  //push our CPU arrays to the GPU
  queue_rk4.enqueueWriteBuffer(_cl_buf_mass,      CL_TRUE, 0, size_vec_mass,  &_object_mass[0],     NULL, &in_mass_event);
  queue_rk4.enqueueWriteBuffer(_current.location, CL_TRUE, 0, size_vec_loc,   &_object_position[0], NULL, &in_location_event);
  queue_rk4.enqueueWriteBuffer(_current.velocity, CL_TRUE, 0, size_vec_vel,   &_object_velocity[0], NULL, &in_velocity_event);

  queue_rk4.finish();
}


void Universe::cl_copyfrgpu()
{
  if (!_using_cl)
    throw (std::runtime_error("Cannot copy data to GPU.  Cl is not initialized"));

  size_t objects = _object_list.size();

  //std::size_t size_vec_mass = sizeof(float_type)*objects;
  std::size_t size_vec_loc = sizeof(Cartesian_Vector)*objects;
  std::size_t size_vec_vel = sizeof(Cartesian_Vector)*objects;

  cl::Event in_mass_event, in_location_event, in_velocity_event;

  //get our CPU arrays from the GPU
  queue_rk4.enqueueReadBuffer(_current.location, CL_TRUE, 0, size_vec_loc,   &_object_position[0], NULL, &in_location_event);
  queue_rk4.enqueueReadBuffer(_current.velocity, CL_TRUE, 0, size_vec_vel,   &_object_velocity[0], NULL, &in_velocity_event);

  queue_rk4.finish();

  auto obj = _object_list.begin();

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



//Performs an integration for the purpose of recording a history for data analysis.
void Universe::cl_integrate()
{
  size_t num_objects = _object_list.size();
  std::vector<cl::Event> wait_queue, new_events;

  const double dtime = 60.0; //seconds (1 min)
  const uint   stride = 180;

  // Start timer.  Reports elapsed time when destroyed.
  boost::timer::auto_cpu_timer t;

  // Copy buffer to 1st row in history
  iterate_gpu_tohistory(num_objects, _current, 0, wait_queue, new_events);
  wait_queue = new_events;

  // Fill in rest of history
  for (unsigned int i = 1; i < _timesteps; i++)
  {
    std::cout << ".";
    std::cout.flush();

    // Iterate Engine
    iterate(dtime, stride, wait_queue, new_events);
    wait_queue = new_events;

    // Copy data to history
    iterate_gpu_tohistory(num_objects, _current, i, wait_queue, new_events);
    wait_queue.clear();
    queue_rk4.finish();
  }

}


// Loads history buffer from HDF5 file
void Universe::cl_load_history(const std::string &file)
{
  
  cl::Event pos_hist_event, vel_hist_event;

  size_t num_objects = _object_list.size();

  // Load Data
  HDF5 h5file(file, H5F_ACC_RDONLY);

  Am3DArrayD position_data(boost::extents[1][1][1]);
  Am3DArrayD velocity_data(boost::extents[1][1][1]);

  h5file.read_3D_double(std::string("/History/Position"), position_data);
  h5file.read_3D_double(std::string("/History/Velocity"), velocity_data);

  // Verify sizes
  const Am3DArrayD::size_type *pos_shape = position_data.shape();
  const Am3DArrayD::size_type *vel_shape = velocity_data.shape();

  if(pos_shape[0] != _timesteps || pos_shape[1] != num_objects ||
     vel_shape[0] != _timesteps || vel_shape[1] != num_objects)
    throw std::runtime_error("Incoming history size doesn't match currently setup history size");
  
  size_t pos_data_size    = position_data.num_elements()*sizeof(float_type);
  size_t vel_data_size    = velocity_data.num_elements()*sizeof(float_type);

  queue_rk4.enqueueWriteBuffer(_cl_buf_hist_location, CL_TRUE, 0, pos_data_size, position_data.data(), NULL, &pos_hist_event);
  pos_hist_event.wait();

  queue_rk4.enqueueWriteBuffer(_cl_buf_hist_velocity, CL_TRUE, 0, vel_data_size, velocity_data.data(), NULL, &vel_hist_event);
  vel_hist_event.wait();

  h5file.read_1D_double(std::string("/History/Time"), _hist_time);

  h5file.close();
  
}


// Saves history buffer to HDF5 file
void Universe::cl_save_history(const std::string &file)
{
  cl::Event pos_hist_event, vel_hist_event, dis_hist_event;

  size_t num_objects = _object_list.size();

  HDF5 h5file(file, H5F_ACC_TRUNC);
  h5file.createGroup(std::string("/History"));

  h5file.write_1D_double(std::string("/History/Time"), _hist_time);

  Am3DArrayD position_data(boost::extents[_timesteps][num_objects][4]);
  Am3DArrayD velocity_data(boost::extents[_timesteps][num_objects][4]);
  Am2DArrayD distance_data(boost::extents[_timesteps][num_objects]);

  size_t pos_data_size    = position_data.num_elements()*sizeof(float_type);
  size_t vel_data_size    = velocity_data.num_elements()*sizeof(float_type);
  size_t dis_data_size    = distance_data.num_elements()*sizeof(float_type);

  queue_rk4.enqueueReadBuffer(_cl_buf_hist_location, CL_TRUE, 0, pos_data_size, position_data.data(), NULL, &pos_hist_event);
  pos_hist_event.wait();
  h5file.write_3D_double(std::string("/History/Position"), position_data);

  queue_rk4.enqueueReadBuffer(_cl_buf_hist_velocity, CL_TRUE, 0, vel_data_size, velocity_data.data(), NULL, &vel_hist_event);
  vel_hist_event.wait();
  h5file.write_3D_double(std::string("/History/Velocity"), velocity_data);

  queue_rk4.enqueueReadBuffer(_cl_buf_hist_distance, CL_TRUE, 0, dis_data_size, distance_data.data(), NULL, &dis_hist_event);
  dis_hist_event.wait();
  h5file.write_2D_double(std::string("/History/Distance"), distance_data);

  h5file.close();
}

// Saves history buffer to HDF5 file
void Universe::cl_fill_distance_buff()
{
  
  cl::Event dis_event;
 
  size_t num_objects = _object_list.size();

  uint ref = 3; // Harded L1 point;

  kern_dist.setArg(0, _cl_buf_hist_location);
  kern_dist.setArg(1, (unsigned int)ref);
  kern_dist.setArg(2, _cl_buf_hist_distance);
  queue_rk4.enqueueNDRangeKernel(kern_dist, cl::NullRange, cl::NDRange(num_objects, _timesteps), cl::NullRange, NULL, &dis_event);
  dis_event.wait();
}


uint Universe::count_sig_objects()
{
  uint count = 0;
  
  for (auto obj = _object_list.begin(); obj != _object_list.end(); obj++)
  {
    if((*obj)->mass > mass_cutoff)
    count++;
  }

  return count;
}



bool PComp(const Object::sptr &a, const Object::sptr &b)
{
     return (*a).mass > (*b).mass;
}



void Universe::sort_objects()
{
  
  _object_list.sort(PComp);

}


} // namespace lib
} // namespace amethyst
