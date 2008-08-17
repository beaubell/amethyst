#ifndef STARS_H
#define STARS_H

/***************************************************************************
 *   Star Generation Routines                                              *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include <string>

void stars_load(std::string &file);
void stars_free(void);
void stars_render(void);

#endif /* STARS_H */
