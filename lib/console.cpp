/*
   Library Console, Definitions
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

// 
// 

#include <iostream>
#include <string>
#include <vector>

#include <signal.h>
#include <termios.h>
#include <stdlib.h>

#include "console.h"
#include "console_io.h"
#include "console_defs.h"

#include "console_menu.h"
#include "console_menu_main.h"

#include "version.h"
#include "cl.h"

#ifdef __GNUG__
#pragma implementation
#endif

using namespace std;

namespace amethyst {

  void   stop_console (void);
  void   catch_signal (int);

  int    command_parse(const string &);
  string command_get  (void);
  
  void   show_prompt(void);
  
  struct termios orig_modes;
  struct termios modes;
    
  void start_console (void)
  {
  
       // Set terminal to a usuable state
       tcgetattr(0, &modes);     /* get the current terminal modes */
       orig_modes = modes;       // save the original settings
       modes.c_lflag &= ~ICANON; /* turn off canonical mode */
       modes.c_lflag &= ~ECHO;   /* turn off echoing */
       tcsetattr(0, 0, &modes);  /* set the new modes */
       
       //Speed up IO operations
       //std::ios::sync_with_stdio(false);
  
       //Set signal handler to catch CTRL-C
       (void) signal(SIGINT, catch_signal);
       
       //Add exit and quit functions to main menu becuase these functions are defined in this file
       consolemain.add("exit", stop_console);
       consolemain.add("quit", stop_console);
       consolemain.add("clinit", lib::cl_init);
       
       //Display version information
       cout << endl << AMETHYST_VER << endl;
       
       //Instantiate command buffer
       string command;
       
       vector<string>                    history;     //Command History
       vector<string>::iterator          history_it;  //Command History Iterator
        
       //Endless command loop
       for (;;)
       {
            show_prompt();
            cout.flush();
            
            command = command_get();
                
            switch (command_parse(command))
            {
                 case 1: cout << endl << command << " :Command Not Recognized..." << endl; break;
                 case 2: stop_console(); return; break;
            }
       }     
       
  }
  
  
  void   stop_console(void)
  {
        cout << "\n";
        tcsetattr( 0, 0, &orig_modes );
        exit(0);
  }
  
  
  void   catch_signal(int /*foo unused*/)
  {
       stop_console();
  }
  
  
  string command_get(void)
  {
       unsigned int curpos = 0; // Cursor Position
       int    ret;
       char   one;        // Input Charater
       string buffer;     // Command Buffer
   
       tcsetattr(0, 0, &modes);
           
       do
       {
            ret = getch(one);
            if (ret == 0)
            {
                 if (one != '\n')
                 {
                      unsigned int gb = 0;
                      buffer.insert(curpos , 1, one);
                      for (; gb < (buffer.length() - curpos); gb++)
                      {
                           cout << buffer[curpos+gb];
                      }
                      
                      cursor_right();
                      
                      do
                      {
                           cursor_left();
                           gb--;
                      } while (gb != 0);
                      
                      cout.flush();
                      curpos++;
                 }
            } 
            else 
            {
                 switch(ret)
                 {
                      case KEY_LEFT     :{
                                              if (curpos != 0)
                                              {
                                                   cursor_left ();
                                                   curpos--;
                                              }
                                              break;
                                         }
                      case KEY_RIGHT    :{
                                              if (curpos < buffer.length())
                                              {
                                                   cursor_right();
                                                   curpos++;
                                              }
                                              break;
                                         }
                      case KEY_BACKSPACE:{
                                              if (curpos != 0)
                                              {
                                                   int gb = 0;
                                                   cursor_left (); 
                                                   cout << " ";
                                                   cursor_left();
                                                   buffer.erase(curpos -1, 1);
                                                   curpos--;
                                                   for (unsigned int i = 0; i < (buffer.length()-curpos); i++)
                                                   {
                                                        cout << buffer[i+curpos];
                                                        gb++;
                                                   }
                                                   cout << " ";
                                                   gb++;
                                                   do
                                                   {
                                                        cursor_left();
                                                        gb--;
                                                   } while (gb != 0);
                                              }
                                              break;
                                         }
                      case KEY_TAB      :{
                                              //Run tab completion logic
                                              string extra;
                                              if(consolemain.list(buffer.substr(0, curpos), extra) == 1)
                                                   cout << "RESULT: " << extra;
                                              
                                              //cout << "[TAB]";
                                              break;
                                         }
                 }
                 //cout << ret;
            }
       } while (one != '\n');
       
       tcsetattr( 0, 0, &orig_modes );
       
       return buffer;
  }
  
  
  int command_parse(const string &command)
  {
       //Check for null command
       if (command == "") return 0;
       
       //Run command
       return consolemain.run(command);
  }
  
  
  void show_prompt(void)
  {
       cout << "\namethyst> ";
  }
         
}
