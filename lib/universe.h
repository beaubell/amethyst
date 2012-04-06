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

//#define BOOST_DISABLE_ASSERTS TRUE
//#include "boost/multi_array.hpp"

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
    void iterate_gpu(const double &time);
    void iterate_cpu(const double &time);

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
    cl::CommandQueue queue_rk4;
    cl::Buffer _cl_buf_mass;
    cl::Buffer _cl_buf_location;
    cl::Buffer _cl_buf_velocity;
    cl::Buffer _cl_buf_hist_location;
    cl::Buffer _cl_buf_hist_velocity;

    // For Runge Kutta Integration
    cl::Buffer _cl_buf_expanded_acceleration;
    cl::Buffer _cl_buf_k1_dlocation;
    cl::Buffer _cl_buf_k1_dvelocity;
    cl::Buffer _cl_buf_k2_dlocation;
    cl::Buffer _cl_buf_k2_dvelocity;
    cl::Buffer _cl_buf_k3_dlocation;
    cl::Buffer _cl_buf_k3_dvelocity;
    cl::Buffer _cl_buf_k4_dlocation;
    cl::Buffer _cl_buf_k4_dvelocity;
    cl::Buffer _cl_buf_tmp_location;
    cl::Buffer _cl_buf_tmp_velocity;
    cl::Buffer _cl_buf_fin_location;
    cl::Buffer _cl_buf_fin_velocity;

    cl::Kernel kern_rk4_sum;
    cl::Kernel kern_rk4_grav;
    cl::Kernel kern_rk4_scale;
    cl::Kernel kern_rk4_scalesum;
    cl::Kernel kern_rk4_finalsum;
    cl::Kernel kern_rk4_reductionscale;
    unsigned int _timesteps;
};

} // namespace lib
} // namespace amethyst

#endif // UNIVERSE_H
