#ifndef LIB_CONSOLE_IO_H_
#define LIB_CONSOLE_IO_H_
/***********************************************************************
 Amethyst Physics Library
  - Library Console IO Utility Function Declarations

 Authors (c):
 2003-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

namespace amethyst {

int  getch(char& c);
//int  getch(char &c, const bool wait);
void clear_screen(const std::string&);

void cursor_left(int count);
void cursor_right(int count);
void cursor_up(int count);
void cursor_down(int count);

}

#endif  // LIB_CONSOLE_IO_H_
