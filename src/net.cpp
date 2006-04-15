/***************************************************************************
 *   Copyright (C) 2006 by Beau V.C. Bellamy                               *
 *   beau@stellarnetservices.net                                            *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
 #define SIZEOF_FLOAT  4
 #define SIZEOF_DOUBLE 8
#endif

namespace amethyst {

  union shortchar {
     unsigned short value;
     unsigned char  a[SIZEOF_SHORT];
  };

  union intchar {
     unsigned int  value;
     unsigned char a[SIZEOF_INT];
  };

  union floatchar {
     float         value;
     unsigned char a[SIZEOF_FLOAT];
  };

  union doublechar {
     double        value;
     unsigned char a[SIZEOF_DOUBLE];
  };

  union longdoublechar {
     long double   value;
     unsigned char a[SIZEOF_LONG_DOUBLE];
  };

  unsigned int ntohi(const unsigned int &in) {

     #ifndef WORDS_BIGENDIAN
      intchar *in_ptr = (intchar *)&in;
      intchar  out;

      for (int i = 0; i < SIZEOF_INT; i++)
      {
         out.a[i] = in_ptr->a[(SIZEOF_INT-1)-i];
      }
      return out.value;
     #else
      return in.value;
     #endif
  }

  unsigned short ntohs(const unsigned short &in) {

     #ifndef WORDS_BIGENDIAN
      shortchar *in_ptr = (shortchar *)&in;
      shortchar  out;

      //printf("ptr: %d \n", in_ptr->value);

      out.a[0] = in_ptr->a[1];
      out.a[1] = in_ptr->a[0];
      
      return out.value;
     #else
      return in.value;
     #endif
  }

  float ntohf(const float &in) {

     #ifndef WORDS_BIGENDIAN
      floatchar *in_ptr = (floatchar *)&in;
      floatchar  out;

      for (int i = 0; i < SIZEOF_FLOAT; i++)
      {
         out.a[i] = in_ptr->a[(SIZEOF_FLOAT-1)-i];
      }
      return out.value;
     #else
      return in.value;
     #endif
  }

  double ntohd(const double &in) {

     #ifndef WORDS_BIGENDIAN
      doublechar *in_ptr = (doublechar *)&in;
      doublechar  out;

      for (int i = 0; i < SIZEOF_DOUBLE; i++)
      {
         out.a[i] = in_ptr->a[(SIZEOF_DOUBLE-1)-i];
      }
      return out.value;
     #else
      return in.value;
     #endif
  }

  long double ntohld(const long double &in) {

     #ifndef WORDS_BIGENDIAN
      longdoublechar *in_ptr = (longdoublechar *)&in;
      longdoublechar  out;

      for (int i = 0; i < SIZEOF_LONG_DOUBLE; i++)
      {
         out.a[i] = in_ptr->a[(SIZEOF_LONG_DOUBLE-1)-i];
      }
      return out.value;
     #else
      return in.value;
     #endif
  }

} // namespace
