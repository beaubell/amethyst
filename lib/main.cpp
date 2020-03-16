/***********************************************************************
 Amethyst
  - Library Console CLI Interface

 Authors (c):
 2003-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

// No great genius has ever existed without some touch of madness. 
//                                                         - Aristotle

#include <string>
#include <iostream>

#include "console.h"
#include "console_io.h"
#include "test.h"             // for library test functions


using namespace amethyst;

void show_help(ConsoleIO& io, Console_Menu& menu);

int main(int argc, char** argv)
{
    using std::placeholders::_1;

    std::string  prompt("Amethyst> ");
    Console_Menu mainmenu;

    std::vector<std::string> history;

    ConsoleIO io(std::cin, std::cout, std::cerr);
    ConsoleCLI cli(io, prompt, mainmenu, history);

    mainmenu.add("quit", [&cli](ConsoleIO& io, const std::string& cmd)-> void { cli.stop(cmd);});
    mainmenu.add("clear", clear_screen);
    mainmenu.add("test", [](ConsoleIO& io, const std::string&)-> void { lib::full_test(); });
    mainmenu.add("testrk4", [](ConsoleIO& io, const std::string&)-> void { lib::test_rk4(); });

    mainmenu.add("help", [&mainmenu](ConsoleIO& io, const std::string&) { show_help(io, mainmenu); });

    cli.start();
    return 0;
}

void show_help(ConsoleIO& io, Console_Menu& menu){
    io.out << std::endl;
    for (auto& [cmd, cmdv]: menu.get_commands()) {
        io.out << cmd << std::endl;
    }
}
