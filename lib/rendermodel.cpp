/***********************************************************************
 Amethyst-lib
  - Rendering Model Base Class, Definitions

 Authors (c):
 2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)

 ***********************************************************************/


#include "rendermodel.h"

namespace amethyst {
namespace lib {


void RenderModel::setName(const std::string& name)
{
    _name = name;
}

const std::string& RenderModel::getName()
{
    return _name;
}


} // namespace lib
} // namespace amethyst