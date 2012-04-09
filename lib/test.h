#ifndef AMETHYST_LIB_TEST_H
#define AMETHYST_LIB_TEST_H

/***********************************************************************
 Amethyst-lib
  - Library Test Functions, Declarations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#ifdef __GNUG__
#pragma interface
#endif

namespace amethyst {
namespace lib {

  // All tests return 1 for success 0 for failure
  void full_test(void);
  bool full_test(bool quiet, bool debug);

  bool test_cartsphere_conv(bool quiet, bool debug);
  bool test_engine(bool quiet, bool debug);
  
  void info_variable_size(void);
  
  bool test_units(bool quiet, bool debug);
  void test_rk4();

} // namespace lib
} // namespace amethyst

#endif /* AMETHYST_LIB_TEST_H */
