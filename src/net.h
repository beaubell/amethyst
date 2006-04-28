#ifndef AMETHYST_NET_H
#define AMETHYST_NET_H

/*
   Granularized Linear Physics Engine, Declarations
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

// 
// 

#include "object.h"

// Untested macros
#define ntohll(x) (((_int64)(ntohl((int)((x << 32) >> 32))) << 32) | (unsigned int)ntohl(((int)(x >> 32))))
#define htonll(x) ntohll(x)

//#ifdef __GNUG__
//#pragma interface
//#endif

namespace amethyst {

//  unsigned short ntohs(const unsigned short &in);
// #define         htons(x) ntohs(x)

  unsigned int   ntohi(const unsigned int   &in);
 #define         htoni(x) ntohi(x)

  float          ntohf(const float  &in);
 #define         htonf(x) ntohf(x)

  double         ntohd(const double &in);
 #define         htond(x) ntohd(x)

  long double    ntohld(const long double &in);
 #define         htonld(x) ntohld(x)

  struct vectorf_3d
  {
    public:
      float x;
      float y;
      float z;
  } __attribute__((__packed__));

  struct vectord_3d
  {
    public:
      double x;
      double y;
      double z;
  } __attribute__((__packed__));

  struct vectorf_4d
  {
    public:
      float w;
      float x;
      float y;
      float z;
  } __attribute__((__packed__));


  struct vectord_4d
  {
    public:
      double w;
      double x;
      double y;
      double z;
  } __attribute__((__packed__));

  // Pack network struct accounting for endianness ,float version
  inline void net_pack(vectorf_3d &left, const Cartesian_Vector &right)
  {
    left.x = htonf(right.x);
    left.y = htonf(right.y);
    left.z = htonf(right.z);
  }

  // Pack network struct accounting for endianness ,double version
  inline void net_pack(vectord_3d &left, const Cartesian_Vector &right)
  {
    left.x = htond(right.x);
    left.y = htond(right.y);
    left.z = htond(right.z);
  }

  inline void net_pack(vectorf_4d &left, const Quaternion &right)
  {
    left.w = htonf(right.w);
    left.x = htonf(right.x);
    left.y = htonf(right.y);
    left.z = htonf(right.z);
  }

  inline void net_pack(vectord_4d &left, const Quaternion &right)
  {
    left.w = htond(right.w);
    left.x = htond(right.x);
    left.y = htond(right.y);
    left.z = htond(right.z);
  }

  inline void net_unpack(Cartesian_Vector &left, const vectorf_3d &right)
  {
     left.x = ntohf(right.x);
     left.y = ntohf(right.y);
     left.z = ntohf(right.z);
  }

  inline void net_unpack(Cartesian_Vector &left, const vectord_3d &right)
  {
     left.x = ntohd(right.x);
     left.y = ntohd(right.y);
     left.z = ntohd(right.z);
  }

  inline void net_unpack(Quaternion       &left, const vectorf_4d &right)
  {
     left.w = ntohf(right.w);
     left.x = ntohf(right.x);
     left.y = ntohf(right.y);
     left.z = ntohf(right.z);
  }

  inline void net_unpack(Quaternion       &left, const vectord_4d &right)
  {
     left.w = ntohd(right.w);
     left.x = ntohd(right.x);
     left.y = ntohd(right.y);
     left.z = ntohd(right.z);
  }

}

#endif /* AMETHYST_NET_H */
