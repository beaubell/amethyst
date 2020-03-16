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

 private:
  ConsoleCTX *data;
};

int  getch(std::istream& is, char& c);

void clear_screen(ConsoleIO& io, const std::string&);

void cursor_left(std::ostream& os, int count);
void cursor_right(std::ostream& os, int count);
void cursor_up(std::ostream& os, int count);
void cursor_down(std::ostream& os, int count);

}

#endif  // LIB_CONSOLE_IO_H_
