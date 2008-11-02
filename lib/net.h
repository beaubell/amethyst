#ifndef AMETHYST_LIB_NET_H
#define AMETHYST_LIB_NET_H

/***********************************************************************
 Amethyst-lib
  - Network support functions

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "object.h"

#ifdef WIN32
#define GCC_PACK
#else
#define GCC_PACK __attribute__((__packed__))
#endif

namespace amethyst {
namespace lib {

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

#ifdef WIN32
#pragma pack(1)
#endif

  struct vectorf_3d
  {
    public:
      float x;
      float y;
      float z;
  } GCC_PACK;

  struct vectord_3d
  {
    public:
      double x;
      double y;
      double z;
  } GCC_PACK;

  struct vectorf_4d
  {
    public:
      float w;
      float x;
      float y;
      float z;
  } GCC_PACK;

  struct vectord_4d
  {
    public:
      double w;
      double x;
      double y;
      double z;
  } GCC_PACK;

#ifdef WIN32
#pragma pack()
#endif

  // Pack network struct accounting for endianness ,float version
  inline void net_pack(vectorf_3d &left, const Cartesian_Vector &right)
  {
    left.x = htonf(static_cast<float>(right.x));
    left.y = htonf(static_cast<float>(right.y));
    left.z = htonf(static_cast<float>(right.z));
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
    left.w = htonf(static_cast<float>(right.w));
    left.x = htonf(static_cast<float>(right.x));
    left.y = htonf(static_cast<float>(right.y));
    left.z = htonf(static_cast<float>(right.z));
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

} // namespace lib
} // namespace amethyst

#endif /* AMETHYST_LIB_NET_H */
