#ifndef AMETHYST_CL_H
#define AMETHYST_CL_H

#if defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#else
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

#define __CL_ENABLE_EXCEPTIONS yup

#include <cstddef>
#include <vector>

#define CL_TARGET_OPENCL_VERSION 110
#include <CL/cl.hpp>

#include "resource.h"

// OpenCL Resources outside of namespace
extern const char _binary_lib_histdist_cl_start,           _binary_lib_histdist_cl_end;
extern const char _binary_lib_rk4_copy3d_cl_start,         _binary_lib_rk4_copy3d_cl_end;
extern const char _binary_lib_rk4_copyrot_cl_start,        _binary_lib_rk4_copyrot_cl_end;
extern const char _binary_lib_rk4_finalsum_cl_start,       _binary_lib_rk4_finalsum_cl_end;
extern const char _binary_lib_rk4_grav_cl_start,           _binary_lib_rk4_grav_cl_end;
extern const char _binary_lib_rk4_reductionscale_cl_start, _binary_lib_rk4_reductionscale_cl_end;
extern const char _binary_lib_rk4_scale_cl_start,          _binary_lib_rk4_scale_cl_end;
extern const char _binary_lib_rk4_scalesum_cl_start,       _binary_lib_rk4_scalesum_cl_end;
extern const char _binary_lib_rk4_sum_cl_start,            _binary_lib_rk4_sum_cl_end;

namespace amethyst {
namespace lib {
  
extern bool cl_initialized;

extern cl::Context amethyst_cl_context;
extern std::vector<cl::Device> cl_devices;

void cl_init(void);
const char* oclErrorString(cl_int error);
cl::Kernel cl_loadkernel(const Resource& res, const std::string &mainfn);


}
}
#endif //AMETHYST_CL_H
