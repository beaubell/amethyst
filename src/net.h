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

//#ifdef __GNUG__
//#pragma interface
//#endif

namespace amethyst {

  struct vectord_3d
  {
    public:
      double x;
      double y;
      double z;

      inline vectord_3d&  operator= (const Cartesian_Vector &right)
      {
        x = right.x;
        y = right.y;
        z = right.z;

        return *this;
      }

  } __attribute__((__packed__));

  struct vectord_4d
  {
    double w;
    double x;
    double y;
    double z;

    inline vectord_4d& operator= (const Quaternion &right)
    {
      w = right.w;
      x = right.x;
      y = right.y;
      z = right.z;

      return *this;
    }

  } __attribute__((__packed__));



  inline void pack(vectord_3d &left, const Cartesian_Vector &right)
  {
    left.x = right.x;
    left.y = right.y;
    left.z = right.z;
  }

  inline void pack(vectord_4d &left, const Quaternion &right)
  {
    left.w = right.w;
    left.x = right.x;
    left.y = right.y;
    left.z = right.z;
  }


  inline void unpack(Cartesian_Vector &left, const vectord_3d &right)
  {
     left.x = right.x;
     left.y = right.y;
     left.z = right.z;
  }

  inline void unpack(Quaternion       &left, const vectord_4d &right)
  {
     left.w = right.w;
     left.x = right.x;
     left.y = right.y;
     left.z = right.z;
  }

}

#endif /* AMETHYST_NET_H */
