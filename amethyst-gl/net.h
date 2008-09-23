/***********************************************************************
 Amethyst-GL
  - Network setup and processing function prototypes

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#ifndef NET_H
#define NET_H

#include <sys/types.h>
#include <time.h>

#include "lib/object.h"
#include "lib/net.h"


int network_setup(void);

void net_send_telemetry(void);
void net_recv_telemetry(void);
int  net_start_thread();

#ifdef WIN32
#define GCC_PACK
#else
#define GCC_PACK __attribute__((__packed__))
#endif

#ifdef WIN32
#pragma pack(1)
#define u_int16_t unsigned short
#endif

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
} GCC_PACK;


struct object_transfer
{
  char                  name[12];
  amethyst::vectord_3d  location;
  amethyst::vectord_3d  velocity;
  amethyst::vectord_3d  acceleration;

  amethyst::vectord_4d  attitude;

  char        pad[12];
} GCC_PACK;

#ifdef WIN32
#pragma pack()
#endif

#endif  /* NET_H */
