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
#include <iomanip>

#include "console.h"
#include "console_io.h"
#include "test.h"             // for library test functions
#include "vulkan.hpp"


using namespace amethyst;

auto show_help(ConsoleIO& io, Console_Menu& menu) -> void;
auto test_vk(ConsoleIO& io) -> void;

int main(int argc, char** argv)
{
    std::string  prompt("Amethyst> ");
    Console_Menu mainmenu;

    std::vector<std::string> history;

    std::cout << std::setprecision(1) << std::fixed;
    ConsoleIO io(std::cin, std::cout, std::cerr, true);
    ConsoleCLI cli(io, prompt, mainmenu, history);

    mainmenu.add("quit", [&cli](ConsoleIO& io, const std::string& cmd)-> void { cli.stop(); });
    mainmenu.add("clear", [](ConsoleIO& io, const std::string&)-> void { io.clear_screen(); });
    mainmenu.add("test", [](ConsoleIO& io, const std::string&)-> void { lib::full_test(io, false, false); });
    mainmenu.add("testrk4", [](ConsoleIO& io, const std::string&)-> void { lib::test_rk4(io); });
    mainmenu.add("testvk", [](ConsoleIO& io, const std::string&)-> void { test_vk(io); });

    mainmenu.add("help", [&mainmenu](ConsoleIO& io, const std::string&) { show_help(io, mainmenu); });

    cli.start();
    return 0;
}

void show_help(ConsoleIO& io, Console_Menu& menu){
    io.out << std::endl;
    for (auto& [cmd, cmdv]: menu.getCommands()) {
        io.out << cmd << std::endl;
    }
}

void
test_vk(ConsoleIO& io) {

    lib::VulkanCompute vc(io, true);

}
