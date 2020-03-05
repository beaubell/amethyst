#ifndef LIB_CONSOLE_H_
#define LIB_CONSOLE_H_
/***********************************************************************
 Amethyst Physics Library
  - Library Console CLI class declaration

 Authors (c):
 2003-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "console_menu.h"

#include <string>
#include <vector>
#include <termios.h>

namespace amethyst {

class ConsoleCLI {

 public:
    ConsoleCLI(std::string& newprompt, Console_Menu& mainmenu, std::vector<std::string>& newhistory);
    virtual ~ConsoleCLI();
    void start();
    void stop(const std::string&);

 private:
    void catch_signal(int);

    int         command_parse(const std::string&);
    std::string command_get(void);

    void        show_prompt(void);

    std::string& prompt_;
    Console_Menu& mainmenu_;

    std::string  buffer_;
    std::string  tmpbuffer_;

    std::vector<std::string>& history_;
    unsigned int history_pos_;

    struct termios orig_modes_;
    struct termios modes_;
};

}  // namespace amethyst

#endif  // LIB_CONSOLE_H_
