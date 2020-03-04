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

void show_help(Console_Menu& menu);

int main(int argc, char** argv)
{
    std::string prompt("Amethyst> ");
    Console_Menu mainmenu;
    mainmenu.add("clear", clear_screen);
    mainmenu.add("test", [](std::string)-> void { lib::full_test(); });
    mainmenu.add("testrk4", [](std::string)-> void { lib::test_rk4(); });

    mainmenu.add("help", [&mainmenu](const std::string&) { show_help(mainmenu); });

    std::vector<std::string> history;

    ConsoleCLI cli(prompt, mainmenu, history);
    cli.start();
    return 0;
}

void show_help(Console_Menu& menu){
    std::cout << std::endl;
    for (auto& items: menu.get_commands()) {
        std::cout << items.command << std::endl;
    }
}
