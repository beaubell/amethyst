#ifndef AMETHYST_CLIENT_STARS_H
#define AMETHYST_CLIENT_STARS_H

/***********************************************************************
 Amethyst-GL
  - Star generation function declarations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/


#include <string>

namespace amethyst {
namespace client {

void stars_load(std::string &file);
void stars_free(void);
void stars_render(void);

} // namespace client
} // namespace amethyst

#endif /* AMETHYST_CLIENT_STARS_H */
