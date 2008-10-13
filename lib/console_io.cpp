/*
   Library Console IO, Definitions
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

//
//

#include <iostream>
//#include <termios.h>

#include "console_io.h"
#include "console_defs.h"

#ifdef __GNUG__
#pragma implementation
#endif

using namespace std;

namespace amethyst {

  void clear_screen(void)
  {
       cout << static_cast<char>(27) << "[H" << static_cast<char>(27) << "[2J";
  }


  //int getch(char &c)
  //{
  //     getch(c, true);
  //}


  int getch(char &c)
  {
       //if ( cin.eof() ) return 0;
       char temp;

       c = 0;
       temp = cin.get();

       switch (temp)
       {
          case 127: return KEY_BACKSPACE; break;
          case 27:
          {
            temp = cin.get();

            switch(temp)
            {

                 case 91 :
                   {
                      temp = cin.get();
                      switch(temp)
                      {
                           case 72: return KEY_HOME;  break;
                           case 70: return KEY_END;   break;
                           case 68: return KEY_LEFT;  break;
                           case 67: return KEY_RIGHT; break;
                           case 66: return KEY_DOWN;  break;
                           case 65: return KEY_UP;    break;
                           case 54:
                             {
                                temp = cin.get();
                                switch(temp)
                                {
                                case 126: return KEY_NPAGE; break;
                                }
                                break;
                             }
                           case 53:
                             {
                                temp = cin.get();
                                switch(temp)
                                {
                                case 126: return KEY_PPAGE; break;
                                }
                                break;
                             }
                           case 51:
                             {
                                temp = cin.get();
                                switch(temp)
                                {
                                case 126: return KEY_DC;    break;
                                }
                                break;
                             }
                           case 50:
                             {
                                temp = cin.get();
                                switch(temp)
                                {
                                case 126: return KEY_IC;    break;
                                }
                                break;
                             }

                      }
                      break;
                   }


            }
          }
          case 9  : return KEY_TAB;       break;

       }

       c = temp;
       return 0;
  }

  void cursor_left (void)
  {
       cout << static_cast<char>(27) << static_cast<char>(91) << static_cast<char>(68);
  }

  void cursor_right(void)
  {
       cout << static_cast<char>(27) << static_cast<char>(91) << static_cast<char>(67);
  }

  void cursor_up   (void)
  {
       cout << static_cast<char>(27) << static_cast<char>(91) << static_cast<char>(65);
  }

  void cursor_down (void)
  {
       cout << static_cast<char>(27) << static_cast<char>(91) << static_cast<char>(66);
  }

}
