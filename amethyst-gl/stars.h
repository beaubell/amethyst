#ifndef STARS_H
#define STARS_H

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

void stars_load(std::string &file);
void stars_free(void);
void stars_render(void);

#endif /* STARS_H */
