#ifndef LIB_CONSOLE_IO_H_
#define LIB_CONSOLE_IO_H_
/***********************************************************************
 Amethyst Physics Library
  - Library Console IO Utility Function Declarations

 Authors (c):
 2003-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include <istream>
#include <ostream>

namespace amethyst {

class ConsoleCTX; // Platform Specific Data

class ConsoleIO {
 public:
  ConsoleIO(std::istream& cin, std::ostream& cout, std::ostream& cerr)
  : in(cin),
    out(cout),
    err(cerr){};
  std::istream& in;
  std::ostream& out;
  std::ostream& err;

  int  getch(char& c);

  void clear_screen();

  void cursor_left(int count);
  void cursor_right(int count);
  void cursor_up(int count);
  void cursor_down(int count);

 private:
  ConsoleCTX *data;
};

}

#endif  // LIB_CONSOLE_IO_H_
