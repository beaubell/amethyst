#ifndef AMETHYST_CLIENT_HUD_H
#define AMETHYST_CLIENT_HUD_H

/***********************************************************************
 Amethyst-GL
  - Heads up display function declarations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

namespace amethyst {
namespace client {

void hud_setup(void);
void hud_shutdown(void);
void hud_render(const double eyeangle);

} // namespace client
} // namespace amethyst

#endif  /* AMETHYST_CLIENT_HUD_H */
