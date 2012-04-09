/*
   Library Console Main Menu, Definition
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

//
//

#include "console_menu_main.h"

#include "console_io.h"       // for clear_screen
#include "test.h"             // for library test functions

namespace amethyst {

  using namespace lib;

  //Instantiate Main Console Menu
  Console_Menu_Main consolemain;

  Console_Menu_Main::Console_Menu_Main()
  {
       add("clear", clear_screen);
       add("test",  full_test);
       add("testrk4", test_rk4);
  }

  Console_Menu_Main::~Console_Menu_Main()
  {

  }
}
