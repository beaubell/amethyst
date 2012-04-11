/***********************************************************************
 Amethyst-lib
  - OpenCL Object Buffer Group Class Definition

 Authors (c):
 2012 Beau V.C. Bellamy (beau@stellarnetservices.com)

 $Revision: $
 $LastChangedDate: $
 $LastChangedBy: $
 ***********************************************************************/

#include "object_group.h"
#include "types.h"
#include "vector.h"

#include <cstddef>


namespace amethyst {
namespace lib {

void Object_Group::set_size(uint sig_size, uint insig_size)
{
  _object_count     = sig_size + insig_size;
  _object_count_sig = sig_size;

  std::size_t size_vec_mass = sizeof(float_type)*_object_count;
  std::size_t size_vec_loc = sizeof(Cartesian_Vector)*_object_count;
  std::size_t size_vec_vel = sizeof(Cartesian_Vector)*_object_count;

  mass     = cl::Buffer(amethyst_cl_context, CL_MEM_READ_ONLY,  size_vec_mass, NULL, NULL);
  location = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_loc,  NULL, NULL);
  velocity = cl::Buffer(amethyst_cl_context, CL_MEM_READ_WRITE, size_vec_vel,  NULL, NULL);
}


unsigned int Object_Group::get_size()
{
  return _object_count;
}


unsigned int Object_Group::get_sigsize()
{
  return _object_count_sig;
}


void Object_Group::clear()
{
  mass     = cl::Buffer();
  location = cl::Buffer();
  velocity = cl::Buffer();
}


} // namespace lib
} // namespace amethyst
