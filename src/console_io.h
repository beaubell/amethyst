/*
   Library Console IO, Declarations
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

// 
// 

#ifndef CONSOLE_IO_H
#define CONSOLE_IO_H

#ifdef __GNUG__
#pragma interface
#endif

namespace amethyst {

  int  getch(char &c);
  //int  getch(char &c, const bool wait);
  void clear_screen(void);
  
  void cursor_left (void);
  void cursor_right(void);
  void cursor_up   (void);
  void cursor_down (void);
}

#endif /* CONSOLE_IO_H */
