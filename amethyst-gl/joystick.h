#ifndef JOYSTICK_H
#define JOYSTICK_H

/***********************************************************************
 Amethyst-GL
  - Joystick input setup function declarations

 Authors (c):
 2004-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

typedef struct {
   int joystick;
   int axis;
} Joy_Axis_Map;

short joystick_axis_norm(short value, unsigned short null);
void  joystick_setup    (void);

#endif /* JOYSTICK_H */
