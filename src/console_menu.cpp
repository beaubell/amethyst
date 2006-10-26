/*
   Library Console Menu Utility Functions, Definitions
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

//
//

#include <iostream>
#include <string.h>

#include "console_menu.h"

namespace amethyst {

  Console_Menu::~Console_Menu()
  {
       // Destroy Command Vector

  }


  void Console_Menu::add(const std::string &new_command, COMMAND_FUNCTION new_function)
  {
       Command_Vector *add = new(Command_Vector);
       add->type     = OP_COMMAND;
       add->command  = new_command;
       add->function = new_function;

       _menu.push_back(add);

  }


  void Console_Menu::add(const std::string &new_command, MENU_FUNCTION run, LIST_FUNCTION list)
  {
       Command_Vector *add = new(Command_Vector);
       add->type     = OP_MENU;
       add->command  = new_command;
       add->menurun  = run;
       add->menulist = list;

       _menu.push_back(add);

  }

  int Console_Menu::run(const std::string &command)
  {
       unsigned int a, b, c;
       std::string cmd, ext;

       //Parse command string for element bounds
       for(a = 0; a < command.length(); a++)
       {
            if(command[a] != (char)' ') break;
       }
       for(b = a; b < command.length(); b++)
       {
            if(command[b] == (char)' ') break;
       }
       for(c = b; c < command.length(); c++)
       {
            if(command[c] != (char)' ') break;
       }

       //Show output for debugging
       //std::cout << "\nA:" << a << "  B:" << b << " C:" << c;

       //Separate command string from first command element and dump the rest into ext;
       cmd = command.substr(a, b - a);
       ext = command.substr(c,command.length() - c);

       //Show output for debugging,  WOW! IT WORKS!!! YES!!!
       //std::cout << "\nCMD=\"" << cmd << "\"  EXT=\"" << ext << "\"";

       //Find matching command for first command element and run associated function
       for(unsigned int i = 0; i < _menu.size(); i++)
       {
            if (cmd == _menu[i]->command)
            {
                 //std::cout << "\nCOMMAND FOUND [" << i << "]\n";

                 switch(_menu[i]->type)
                 {
                      case OP_COMMAND:
                      {
                           _menu[i]->function();
                           return 0;
                           break;
                      }
                      case OP_MENU:
                      {
                           return _menu[i]->menurun(ext);
                           break;
                      }
                 }
            }
       }

       return 1;
  }


  int Console_Menu::list(const std::string &command, std::string &unambiguity)
  {
       std::cout << "\n" << command;

       unsigned int a, b, c;
       std::string cmd, ext;

       //Parse command string for element bounds
       for(a = 0; a < command.length(); a++)
       {
            if(command[a] != (char)' ') break;
       }
       for(b = a; b < command.length(); b++)
       {
            if(command[b] == (char)' ') break;
       }
       for(c = b; c < command.length(); c++)
       {
            if(command[c] != (char)' ') break;
       }

       //Show output for debugging
       //std::cout << "\nA:" << a << "  B:" << b << " C:" << c;

       //Separate command string from first command element and dump the rest into ext;
       cmd = command.substr(a, b - a);
       ext = command.substr(c,command.length() - c);

       //Show output for debugging,  WOW! IT WORKS!!! YES!!!
       //std::cout << "\nCMD=\"" << cmd << "\"  EXT=\"" << ext << "\"";

       //Find matching command for first command element and run associated function
       for(unsigned int i = 0; i < _menu.size(); i++)
       {
            if (cmd == _menu[i]->command)
            {
                 //std::cout << "\nCOMMAND FOUND [" << i << "]\n";

                 switch(_menu[i]->type)
                 {
                      case OP_COMMAND:
                      {
                           //Nothing to do
                           return 0;
                           break;
                      }
                      case OP_MENU:
                      {
                           return _menu[i]->menulist(ext,unambiguity);
                           break;
                      }
                 }
            }
       }

       //No match found, see if the string partially matches

       //make sure command is last in string
       if(command[command.length() - 1] != ' ' && (ext == "") )
       {
            for(b = command.length(); b > 0; b--)
            {
                 if(command[b] != (char)' ') break;
            }
            for(a = b; a > 0; a--)
            {
                 if(command[a] == (char)' ') break;
            }

            if (command[a] == (char)' ') a++;

            //Show output for debugging
            std::cout << "\nA:" << a << "  B:" << b;

            cmd = command.substr(a, b - a);

            //Show output for debugging,  WOW! IT WORKS!!! YES!!!
            std::cout << "\nCMD=\"" << cmd << "\"";

            std::string              prelim, first;
            std::vector<std::string> found_commands;
            unsigned int             count = 0;

            //find matching strings
            for(unsigned int i = 0; i < _menu.size(); i++)
            {

                if (!strncmp(cmd.c_str(), _menu[i]->command.c_str(), cmd.length()))
                {
                     count++;
                     found_commands.push_back(_menu[i]->command);
                     //if(count == 1) first = _menu[i]->command;
                     //if(count == 2) std::cout << "\n" << first << "\t" << _menu[i]->command;
                     //if(count > 2)  std::cout << "\t" << _menu[i]->command;

                }
            }

            //If only one match has been found...   else
            if(count == 1)
            {
                 unambiguity = found_commands[0].substr(cmd.length(), found_commands[0].length()) + " ";
                 return 1;
            }

            //find largest string
            unsigned int largest = 0;
            for(unsigned int i = 0; i < found_commands.size(); i++)
            {
                 if(largest < found_commands[i].length()) largest = found_commands[i].length();
            }

            std::cout << "LARGEST: " << largest;

            //Find common letters
            for(unsigned int y = cmd.length(); y < largest; y++)
            {
                 char letter = found_commands[0][y];
                 bool good = true;
                 for(unsigned int x = 1; x < found_commands.size(); x++)
                 {
                      if(letter != found_commands[x][y]) good = false;
                 }
                 std::cout << ">" << letter;
                 if(good == true) unambiguity += letter; else break;
            }

            std::cout << "\nAM:\"" << unambiguity << "\"";

            //List all posibles if no characters can be found
            if(unambiguity == "")
            {
                 std::cout << "\n";
                 for(unsigned int x = 1; x < found_commands.size(); x++)
                 {
                      std::cout << found_commands[x] << "\t";
                 }
                 //std::cout << "\n";
                 return 0;
            } else return 1;
       }

  return 0;
  }

}
