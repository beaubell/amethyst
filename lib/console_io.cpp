/***********************************************************************
 Amethyst Physics Library
  - Library Console IO Utility Function Implementations

 Authors (c):
 2003-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "console_io.h"
#include "console_defs.h"

using namespace std;

namespace amethyst {

void
ConsoleIO::clear_screen() {
    out << static_cast<char>(27) << "[H" << static_cast<char>(27) << "[2J";
}


int
ConsoleIO::getch(char& c) {
    //if ( cin.eof() ) return 0;
    char temp;

    c = 0;
    temp = in.get();

    switch (temp) {
        case 127:
            return KEY_BACKSPACE;
            break;

        case 27: {
            temp = in.get();

            switch (temp) {

                case 91 : {
                    temp = in.get();

                    switch (temp) {
                        case 72:
                            return KEY_HOME;
                            break;

                        case 70:
                            return KEY_END;
                            break;

                        case 68:
                            return KEY_LEFT;
                            break;

                        case 67:
                            return KEY_RIGHT;
                            break;

                        case 66:
                            return KEY_DOWN;
                            break;

                        case 65:
                            return KEY_UP;
                            break;

                        case 54: {
                            temp = in.get();

                            switch (temp) {
                                case 126:
                                    return KEY_NPAGE;
                                    break;
                            }
                            break;
                        }

                        case 53: {
                            temp = in.get();

                            switch (temp) {
                                case 126:
                                    return KEY_PPAGE;
                                    break;
                            }
                            break;
                        }

                        case 51: {
                            temp = in.get();

                            switch (temp) {
                                case 126:
                                    return KEY_DC;
                                    break;
                            }
                            break;
                        }

                        case 50: {
                            temp = in.get();

                            switch (temp) {
                                case 126:
                                    return KEY_IC;
                                    break;
                            }
                            break;
                        }
                    }  // switch (temp) for case 91
                    break;
                }  // case 91
            }  // switch (temp) for case 27
            break;
        }  // case 27 (ESC)

        case 9  :
            return KEY_TAB;
            break;

    }  // switch (temp)

    c = temp;
    return 0;
}

void
ConsoleIO::cursor_left(int count) {
    for (int i = 0; i < count; i++) {
        out << static_cast<char>(27) << static_cast<char>(91) << static_cast<char>(68);
    }
}

void
ConsoleIO::cursor_right(int count) {
    for (int i = 0; i < count; i++) {
        out << static_cast<char>(27) << static_cast<char>(91) << static_cast<char>(67);
    }
}

void
ConsoleIO::cursor_up(int count) {
    for (int i = 0; i < count; i++) {
        out << static_cast<char>(27) << static_cast<char>(91) << static_cast<char>(65);
    }
}

void
ConsoleIO::cursor_down(int count) {
    for (int i = 0; i < count; i++) {
        out << static_cast<char>(27) << static_cast<char>(91) << static_cast<char>(66);
    }
}

} // namespace amethyst
