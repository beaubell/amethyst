/*
   Library Console Main Menu, Declaration
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

// 
// 

#ifndef CONSOLE_MENU_MAIN_H
#define CONSOLE_MENU_MAIN_H

#include "console_menu.h"

#ifdef __GNUG__
#pragma interface
#endif

namespace amethyst {

  class Console_Menu_Main : public Console_Menu {
  
       public:
          Console_Menu_Main();
          
       };
       
  extern Console_Menu_Main consolemain;
}

#endif /* CONSOLE_MENU_MAIN_H */
