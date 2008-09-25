#ifndef JOYSTICK_H
#define JOYSTICK_H

/***********************************************************************
 Amethyst-GL
  - Joystick input setup function declarations

 Authors (c):
 2004-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision: 150 $
 $LastChangedDate: 2008-09-23 13:43:06 -0800 (Tue, 23 Sep 2008) $
 $LastChangedBy: beau $
 ***********************************************************************/

typedef struct {
   int joystick;
   int axis;
} Joy_Axis_Map;

short joystick_axis_norm(short value, unsigned short null);
void  joystick_setup    (void);

#endif /* JOYSTICK_H */
