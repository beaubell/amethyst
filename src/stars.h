#ifndef STARS_H
#define STARS_H

/***************************************************************************
 *   Star Generation Routines                                              *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include <string>

void load_stars(std::string &file);
void free_stars(void);
void display_stars(void);

#endif /* STARS_H */
