#ifndef AMETHYST_LIB_CL_H
#define AMETHYST_LIB_CL_H

#if defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#else
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif


#include <cstddef>
#include <vector>

#define CL_HPP_ENABLE_EXCEPTIONS yup
#define CL_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#include <CL/opencl.hpp>

#include "resource.h"

// OpenCL Resources outside of namespace
DECLARE_RESOURCE(lib_histdist_cl);
DECLARE_RESOURCE(lib_rk4_copy3d_cl);
DECLARE_RESOURCE(lib_rk4_copyrot_cl);
DECLARE_RESOURCE(lib_rk4_finalsum_cl);
DECLARE_RESOURCE(lib_rk4_grav_cl);
DECLARE_RESOURCE(lib_rk4_reductionscale_cl);
DECLARE_RESOURCE(lib_rk4_scale_cl);
DECLARE_RESOURCE(lib_rk4_scalesum_cl);
DECLARE_RESOURCE(lib_rk4_sum_cl);

namespace amethyst::lib {

extern bool cl_initialized;

extern cl::Context amethyst_cl_context;
extern std::vector<cl::Device> cl_devices;

void cl_init();
const char* oclErrorString(cl_int error);
cl::Kernel cl_loadkernel(const Resource& res, const std::string &mainfn);


}
#endif //AMETHYST_LIB_CL_H
