#ifndef AMETHYST_GL_SOUND_H
#define AMETHYST_GL_SOUND_H

/***********************************************************************
 Amethyst-lib
  - Physics Object Class Declarations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <string>
#include <boost/filesystem.hpp>

#include "vector.h"
#if 0
namespace amethyst {
namespace client {

  namespace fs = boost::filesystem;

  class Sound {

     private:
		path sound;

     public:
       Sound();
       virtual ~Object() {};

       int load(const fs::path &force);
       void remove(const int& index);

       void play (const int& index);
	   void stop (const int& index);
         
       };

} // namespace client
} // namespace amethyst
#endif

#endif /* OBJECT_H */

