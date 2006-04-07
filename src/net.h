/***************************************************************************
 *   Copyright (C) 2005 by Beau V.C. Bellamy                               *
 *   beau@autolfs.org                                                      *
 ***************************************************************************/

#ifndef NET_H
#define NET_H

#include <sys/types.h>
#include <time.h>

#include <amethyst/object.h>
#include <amethyst/net.h>


void net_test(void);

void net_send_telemetry(void);
int  net_recieve_thread();

struct packet_header
{
  // Sequence Counters
  u_int16_t  out_seq;
  u_int16_t  in_seq;

  // Time
  //u_int32_t  timestamp;

  // Packet Type
  u_int16_t  type;    // 1 == object transfer,  2 == chat?  maybe?
  u_int16_t  misc;    // if type==1,  misc = number of objects
} __attribute__((__packed__));


struct object_transfer
{
  char                  name[12];
  amethyst::vectord_3d  location;
  amethyst::vectord_3d  velocity;
  amethyst::vectord_3d  acceleration;

  amethyst::vectord_4d  attitude;

  char        pad[12];
} __attribute__((__packed__));

#endif  /* NET_H */
