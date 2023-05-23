#ifndef AMETHYST_LIB_OBJECT_GROUP_H
#define AMETHYST_LIB_OBJECT_GROUP_H

/***********************************************************************
 Amethyst-lib
  - OpenCL Object Buffer Group Class Declaration

 Authors (c):
 2012 Beau V.C. Bellamy (beau@stellarnetservices.com)

 $Revision: $
 $LastChangedDate: $
 $LastChangedBy: $
 ***********************************************************************/

#include "cl.h"

#include <cstddef>

namespace amethyst::lib {

class Object_Group
{
  public:
    Object_Group();

    void set_size(size_t sig_size, size_t insig_size);
    void clear();
    [[nodiscard]] auto get_size() const -> size_t;
    [[nodiscard]] auto get_sigsize() const -> size_t;

    cl::Buffer mass;
    cl::Buffer location;
    cl::Buffer velocity;

    double time;

  private:
    std::size_t _object_count;
    std::size_t _object_count_sig;
};

} // namespace amethyst::lib

#endif // AMETHYST_LIB_OBJECT_GROUP_H
