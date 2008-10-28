/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? implementations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "log.h"

#include <string>
#include <iostream>

namespace amethyst {
namespace client {

Log::Log()
{
}

void Log::add(std::string message)
{
    std::cout << message << std::endl;
    log_buffer_.push_back(message);
}

const std::vector<std::string>& Log::log()
{
    return log_buffer_;
}

} // namespace client
} // namespace amethyst
