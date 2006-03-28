/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/

using namespace std;
 
void joystick (void);
void SetupJoystick (void);

struct Joy_State
{
	int fd;
	short axis[8];
	short button[28];
};

extern struct Joy_State joy_state;

short joystick_axis_norm(short value, unsigned short null);
