/*
   Library Console Menu Utility Functions, Declarations
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

//
//

#ifndef CONSOLE_MENU_H
#define CONSOLE_MENU_H

#include <string>
#include <vector>

namespace amethyst {

  typedef int   (*MENU_FUNCTION)    (const std::string &command);
  typedef int   (*LIST_FUNCTION)    (const std::string &command, std::string &unambiguity);
  typedef void  (*COMMAND_FUNCTION) (void);

  enum Option_Type
  {
       OP_MENU, OP_COMMAND
  };

  struct Command_Vector
  {
       Option_Type       type;
       std::string       command;
       COMMAND_FUNCTION  function;
       MENU_FUNCTION     menurun;
       LIST_FUNCTION     menulist;

  };


  class Console_Menu {

       public:
           Console_Menu(){};
          virtual ~Console_Menu();
          void  add (const std::string& command, COMMAND_FUNCTION);
          void  add (const std::string& command, MENU_FUNCTION run, LIST_FUNCTION list);
          int   run (const std::string& command);
          int   list(const std::string& command, std::string &unambiguity);

       private:
          std::vector<Command_Vector*>            _menu;    //Menu
          std::vector<Command_Vector*>::iterator  _menu_it; //Menu Iterator

       };
}

#endif /* CONSOLE_MENU_H */
