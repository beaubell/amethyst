/***********************************************************************
 Amethyst Physics Library
  - Library Console Menu Class Implementation

 Authors (c):
 2003-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "lib/console_menu.h"

#include <string.h>
#include <iostream>

namespace amethyst {

Command_Vector::Command_Vector(Option_Type ntype, CommandFunction ncmdfn , MenuFunction nmenufn, ListFunction nlistfn)
 : type(ntype),
   cmdfn(ncmdfn),
   menurunfn(nmenufn),
   menulistfn(nlistfn) {
}


Console_Menu::~Console_Menu() {
}

void
Console_Menu::add(const std::string &new_command, CommandFunction new_function) {

    menu_.emplace(std::pair(new_command, Command_Vector(OP_COMMAND, new_function, nullptr, nullptr)));
}

void
Console_Menu::add(const std::string &new_command, MenuFunction run, ListFunction list) {

    menu_.emplace(std::pair(new_command, Command_Vector(OP_MENU, nullptr, run, list)));
}

int Console_Menu::run(ConsoleIO& io, const std::string &command) {
    unsigned int a, b, c;
    std::string cmd, ext;

    // Parse command string for element bounds
    for (a = 0; a < command.length(); a++) {
        if (command[a] != static_cast<char>(' ')) {
            break;
        }
    }

    for (b = a; b < command.length(); b++) {
        if (command[b] == static_cast<char>(' ')) {
            break;
        }
    }

    for (c = b; c < command.length(); c++) {
        if (command[c] != static_cast<char>(' ')) {
            break;
        }
    }

    // Show output for debugging
    // std::cout << "\nA:" << a << "  B:" << b << " C:" << c;

    // Separate command string from first command element and dump the rest into
    // ext;
    cmd = command.substr(a, b - a);
    ext = command.substr(c, command.length() - c);

    // Show output for debugging,  WOW! IT WORKS!!! YES!!!
    // std::cout << "\nCMD=\"" << cmd << "\"  EXT=\"" << ext << "\"";

    // Find matching command for first command element and run associated function
    for (const auto& [vcmd, vobj]: menu_) {
        if (cmd == vcmd) {
            // std::cout << "\nCOMMAND FOUND [" << i << "]\n";

            switch (vobj.type) {
              case OP_COMMAND: {
                vobj.cmdfn(io, ext);
                return 0;
                break;
              }

              case OP_MENU: {
                return vobj.menurunfn(ext);
                break;
              }
            }
        }
    }

    return 1;
}

int
Console_Menu::list(const std::string &command, std::string &possibilities) {
    unsigned int a, b, c;
    std::string cmd, ext;

    if (command.length() == 0) {
        return 0;
    }

    // Parse command string for element bounds
    for (a = 0; a < command.length(); a++) {
        if (command[a] != static_cast<char>(' ')) {
            break;
        }
    }

    for (b = a; b < command.length(); b++) {
        if (command[b] == static_cast<char>(' ')) {
            break;
        }
    }

    for (c = b; c < command.length(); c++) {
        if (command[c] != static_cast<char>(' ')) {
            break;
        }
    }

    // Show output for debugging
    // std::cout << "\nA:" << a << "  B:" << b << " C:" << c;

    // Separate command string from first command element and dump the rest into
    // ext;
    cmd = command.substr(a, b - a);
    ext = command.substr(c, command.length() - c);

    // Show output for debugging,  WOW! IT WORKS!!! YES!!!
    // std::cout << "\nCMD=\"" << cmd << "\"  EXT=\"" << ext << "\"";

    // Find matching command for first command element and run associated function
    for (const auto& [vcmd, vobj]: menu_) {
        if (cmd == vcmd) {
            // std::cout << "\nCOMMAND FOUND [" << i << "]\n";

            switch (vobj.type) {
                case OP_COMMAND: {
                    // Nothing to do
                    return 0;
                    break;
                }

                case OP_MENU: {
                    return vobj.menulistfn(ext, possibilities);
                    break;
                }
            }
        }
    }

    // No match found, see if the string partially matches

    // make sure command is last in string
    if (command[command.length() - 1] != ' ' && (ext == "")) {
        for (b = command.length(); b > 0; b--) {
            if (command[b] != static_cast<char>(' ')) {
                break;
            }
        }

        for (a = b; a > 0; a--) {
            if (command[a] == static_cast<char>(' ')) {
                break;
            }
        }

        if (command[a] == static_cast<char>(' ')) {
            a++;
        }

        // Show output for debugging
        // std::cout << "\nA:" << a << "  B:" << b;

        cmd = command.substr(a, b - a);

        // Show output for debugging,  WOW! IT WORKS!!! YES!!!
        // std::cout << "\nCMD=\"" << cmd << "\"";

        std::string prelim, first;
        std::vector<std::string> found_commands;
        unsigned int count = 0;

        // find matching strings
        for (const auto& [vcmd, vobj]: menu_) {
            if (!strncmp(cmd.c_str(), vcmd.c_str(), cmd.length())) {
                count++;
                found_commands.push_back(vcmd);
                // if(count == 1) first = _menu[i]->command;
                // if(count == 2) std::cout << "\n" << first << "\t" <<
                // _menu[i]->command; if(count > 2)  std::cout << "\t" <<
                // _menu[i]->command;
            }
        }

        // If only one match has been found...   else
        if (count == 1) {
            possibilities =
                found_commands[0].substr(cmd.length(), found_commands[0].length());
            return 1;
        }

        // find largest string
        unsigned int largest = 0;

        for (unsigned int i = 0; i < found_commands.size(); i++) {
            if (largest < found_commands[i].length()) {
                largest = found_commands[i].length();
            }
        }

        // std::cout << "LARGEST: " << largest;

        // Find common letters
        for (unsigned int y = cmd.length(); y < largest; y++) {
            char letter = found_commands[0][y];
            bool good = true;

            for (unsigned int x = 1; x < found_commands.size(); x++) {
                if (letter != found_commands[x][y]) {
                    good = false;
                }
            }

            std::cout << ">" << letter;

            if (good == true) {
                possibilities += letter;
            } else {
                break;
            }
        }

        // std::cout << "\nAM:\"" << unambiguity << "\"";

        // List all posibles if no characters can be found
        if (possibilities.empty() && count > 1) {
            std::cout << "\n";

            for (unsigned int x = 1; x < found_commands.size(); x++) {
                std::cout << found_commands[x] << "\t";
            }

            // std::cout << "\n";
            return 0;
        } else {
            return count;
        }
    }

    return 0;
}

Console_Menu::CommandV&
Console_Menu::getCommands() { return menu_; }

}  // namespace amethyst
