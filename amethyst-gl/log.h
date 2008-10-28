#ifndef AMETHYST_CLIENT_LOG_H
#define AMETHYST_CLIENT_LOG_H

/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? declarations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <string>
#include <vector>

namespace amethyst {
namespace client {

class Log
{
   public:
    Log();

    void add(std::string);

    const std::vector<std::string>& log();

   private:
    std::vector<std::string> log_buffer_;

};

} // namespace client
} // namespace amethyst


#endif // AMETHYST_CLIENT_LOG_H
