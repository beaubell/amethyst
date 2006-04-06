/***************************************************************************
 *   Copyright (C) 2005 by Beau V.C. Bellamy                               *
 *   beau@autolfs.org                                                      *
 ***************************************************************************/

#ifndef AMETHYST_NET_H
#define AMETHYST_NET_H

#include <amethyst/object.h>


void net_test(void);

void net_send_telematry(void);

struct packet_header
{
    char type;
    char misc;
};
struct object_transfer
{
    char                        name[10];
    amethyst::Cartesian_Vector  location;
    amethyst::Cartesian_Vector  velocity;
    amethyst::Cartesian_Vector  acceleration;

    amethyst::Quaternion        attitude;

    char                        pad[10];
};

#endif  /* AMETHYST_NET_H */
