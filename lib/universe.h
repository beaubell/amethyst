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
#include "object_group.h"

//#define BOOST_DISABLE_ASSERTS TRUE
//#include "boost/multi_array.hpp"

namespace amethyst {
namespace lib {

class Universe
{
  public:
    Universe(void);
    //~Universe(void);

    bool do_gravity_calc;
    bool gpu_rk4_4thorder;

    void object_add(      Object::ptr);
    void object_del(const Object::ptr);
    void object_del_all(void);

    Object::ptr object_find(const std::string &name);

    std::size_t object_count();

    void toggle_gravity();
    void toggle_4thorder();

    void iterate(const double &time, uint stride);
    void iterate(const double &dtime,
                 const uint stride,
                 std::vector<cl::Event> wait_queue,
                 std::vector<cl::Event> &new_events);
    
    void iterate_gpu(const double &time,
                     std::vector<cl::Event> wait_queue,
                     std::vector<cl::Event> &new_events);  // Events_out
    
    void iterate_gpu_rk4_gravk(const double &dtime,
                               uint num_objects,
                               cl::Buffer &masses,
                               Object_Group &old,
                               Object_Group &new_d,
                               std::vector<cl::Event> wait_queue,
                               std::vector<cl::Event> &events);  // Events_out

    void iterate_gpu_rk4_scalesum(const double &scale,
                                  const uint num_objects,
                                  Object_Group &orig,
                                  Object_Group &k,
                                  Object_Group &new_objs,
                                  std::vector<cl::Event> wait_queue,
                                  std::vector<cl::Event> &events);

    void iterate_gpu_rk4_finalsum(const uint num_objects,
                                  Object_Group &orig,
                                  Object_Group &k1,
                                  Object_Group &k2,
                                  Object_Group &k3,
                                  Object_Group &k4,
                                  Object_Group &new_objs,
                                  std::vector<cl::Event> wait_queue,
                                  std::vector<cl::Event> &events);

    void iterate_gpu_tohistory(const uint num_objects,
                               Object_Group &current,
                               const uint index,
                               std::vector<cl::Event> wait_queue,
                               std::vector<cl::Event> &events);

    void iterate_gpu_frhistory(const uint num_objects,
                               Object_Group &current,
                               const uint index,
                               std::vector<cl::Event> wait_queue,
                               std::vector<cl::Event> &events);
    
    void iterate_cpu(const double &time);

    void cl_setup();
    void cl_copytogpu();
    void cl_copyfrgpu();
    void cl_integrate();
    uint count_sig_objects();
    void sort_objects();

    std::list<Object::ptr>& list(void);

  //private: //FIXME Temporary fix for science
    std::list<Object::ptr>  _object_list;

    std::vector<float_type> _object_mass;
    std::vector<Cartesian_Vector> _object_position;
    std::vector<Cartesian_Vector> _object_velocity;

    bool _using_cl;
    cl::CommandQueue queue_rk4;
    cl::Buffer _cl_buf_mass;
    Object_Group _current;


    // For Runge Kutta Integration
    cl::Buffer _cl_buf_expanded_acceleration;
    Object_Group _k1;
    Object_Group _k2;
    Object_Group _k3;
    Object_Group _k4;
    Object_Group _tmp;
    Object_Group _final;

    cl::Kernel kern_rk4_sum;
    cl::Kernel kern_rk4_grav;
    cl::Kernel kern_rk4_scale;
    cl::Kernel kern_rk4_copy3d;
    cl::Kernel kern_rk4_scalesum;
    cl::Kernel kern_rk4_finalsum;
    cl::Kernel kern_rk4_reductionscale;

    // History Buffer
    unsigned int _timesteps;
    cl::Buffer _cl_buf_hist_location;
    cl::Buffer _cl_buf_hist_velocity;
    std::vector<double> _hist_time;

    
    // The amount of mass that is the threshold between gravitationally significant and insignificant.
    float_type mass_cutoff;
};

} // namespace lib
} // namespace amethyst

#endif // UNIVERSE_H
