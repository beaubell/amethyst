#ifndef LIB_CONSOLE_H_
#define LIB_CONSOLE_H_
/***********************************************************************
 Amethyst Physics Library
  - Library Console CLI class declaration

 Authors (c):
 2003-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "console_menu.h"
#include "console_io.h"

#include <istream>
#include <iosfwd>
#include <string>
#include <vector>

namespace amethyst {

class ConsoleCLI {

 public:
    ConsoleCLI(ConsoleIO& io, std::string& newprompt, Console_Menu& mainmenu, std::vector<std::string>& newhistory);
    ConsoleCLI(const ConsoleCLI& cli) = delete;
    ConsoleCLI& operator=(const ConsoleCLI& cli) = delete;
    ConsoleCLI(ConsoleCLI&& cli) = delete;
    ConsoleCLI& operator=(ConsoleCLI&& cli) = delete;
    virtual ~ConsoleCLI();

    void start();
    void stop();

 private:
    void catch_signal(int);

    int         command_parse(const std::string&);
    std::string command_get();

    void        show_prompt();

    bool active_;
    std::string& prompt_;
    Console_Menu& mainmenu_;

    std::string  buffer_;
    std::string  tmpbuffer_;

    std::vector<std::string>& history_;
    unsigned int history_pos_;
    ConsoleIO& io_;
};

}  // namespace amethyst

#endif  // LIB_CONSOLE_H_
