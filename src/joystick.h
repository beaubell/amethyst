#ifndef JOYSTICK_H
#define JOYSTICK_H

/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/

typedef struct {
   int joystick;
   int axis;
} Joy_Axis_Map;

short joystick_axis_norm(short value, unsigned short null);
void  setup_joystick    (void);

#endif /* JOYSTICK_H */
