/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? implementations

 Authors (c):
 2008-2026 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "log.h"

#include <string>
#include <iostream>

namespace amethyst::client {

Log::Log() = default;


void Log::add(std::string message)
{
    std::cout << message << std::endl;
    log_buffer_.push_back(message);
}

const std::vector<std::string>& Log::log()
{
    return log_buffer_;
}

} // namespace amethyst::client
