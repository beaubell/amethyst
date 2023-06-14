#ifndef AMETHYST_LIB_TEST_H
#define AMETHYST_LIB_TEST_H

/***********************************************************************
 Amethyst-lib
  - Library Test Functions, Declarations

 Authors (c):
 2006-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "console_io.h"

namespace amethyst::lib {

  // All tests return 1 for success 0 for failure
  bool full_test(ConsoleIO& io, bool quiet, bool debug);

  bool test_cartsphere_conv(ConsoleIO& io, bool quiet, bool debug);
  bool test_engine(ConsoleIO& io, bool quiet, bool debug);
  
  void info_variable_size(ConsoleIO& io);
  
  bool test_units(ConsoleIO& io, bool quiet, bool debug);
  void test_rk4(ConsoleIO& io);

} // namespace amethyst::lib

#endif /* AMETHYST_LIB_TEST_H */
