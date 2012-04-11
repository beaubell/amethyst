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

#include <CL/cl.hpp>


namespace amethyst {
namespace lib {
  
extern bool cl_initialized;

extern cl::Context amethyst_cl_context;
extern std::vector<cl::Device> cl_devices;

void cl_init(void);
const char* oclErrorString(cl_int error);
cl::Kernel cl_loadkernel(const std::string &file, const std::string &main);

}
}
#endif //AMETHYST_CL_H
