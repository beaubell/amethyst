#ifndef LIB_CONSOLE_MENU_H_
#define LIB_CONSOLE_MENU_H_
/***********************************************************************
 Amethyst Physics Library
  - Library Console Menu Class Declaration

 Authors (c):
 2003-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include <functional>
#include <string>
#include <map>

#include "console_io.h"

namespace amethyst {

typedef std::function<int(const std::string &command)> MenuFunction;
typedef std::function<void(ConsoleIO& io, const std::string &arguments)> CommandFunction;
typedef std::function<int(const std::string &command, std::string &possibilies)> ListFunction;

enum Option_Type { OP_MENU, OP_COMMAND };

class Command_Vector {
 public:
    Command_Vector(Option_Type type, CommandFunction , MenuFunction, ListFunction);
 
    Option_Type type;
    CommandFunction cmdfn;
    MenuFunction menurunfn;
    ListFunction menulistfn;
};

class Console_Menu {
 public:
    typedef std::map<std::string, Command_Vector> CommandV;

    Console_Menu() {}
    virtual ~Console_Menu();
    void add(const std::string &command, CommandFunction);
    void add(const std::string &command, MenuFunction run, ListFunction list);
    int run(ConsoleIO& io, const std::string &command);
    int list(const std::string &command, std::string &possibilites);
    CommandV& get_commands(void);

 private:
    CommandV menu_;
};

} // namespace amethyst

#endif // LIB_CONSOLE_MENU_H_
