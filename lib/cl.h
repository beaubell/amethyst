#ifndef AMETHYST_CL_H
#define AMETHYST_CL_H

#define __CL_ENABLE_EXCEPTIONS yup
#include <CL/cl.hpp>

namespace amethyst {
namespace lib {
  
extern bool cl_initialized;

extern cl::Context amethyst_cl_context;
extern std::vector<cl::Device> cl_devices;

void cl_init(void);
const char* oclErrorString(cl_int error);

}
}
#endif //AMETHYST_CL_H
