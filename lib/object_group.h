#ifndef AMETHYST_OBJECT_GROUP_H
#define AMETHYST_OBJECT_GROUP_H

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

namespace amethyst {
namespace lib {

class Object_Group
{
  public:
    Object_Group();

    void set_size(uint sig_size, uint insig_size);
    void clear();
    uint get_size();
    uint get_sigsize();

    cl::Buffer mass;
    cl::Buffer location;
    cl::Buffer velocity;

    double time;

  private:
    uint _object_count;
    uint _object_count_sig;
};

} // namespace lib
} // namespace amethyst

#endif // AMETHYST_OBJECT_BUFFGRP_H