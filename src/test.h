/*
   Physics Library Performance Test Functions, Declarations
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

// 
// 

#ifndef TEST_H
#define TEST_H

#ifdef __GNUG__
#pragma interface
#endif

namespace amethyst {

  // All tests return 1 for success 0 for failure
  void full_test(void);
  bool full_test(bool quiet, bool debug);

  bool test_cartsphere_conv(bool quiet, bool debug);
  bool test_engine(bool quiet, bool debug);

  void info_variable_size(void);

}

#endif /* TEST_H */
