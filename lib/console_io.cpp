/***********************************************************************
 Amethyst Physics Library
  - Library Console IO Utility Function Implementations

 Authors (c):
 2003-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "console_io.h"
#include "console_defs.h"

#ifdef WIN32
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace amethyst {

class ConsoleCTX {
  public:
    #ifdef WIN32
    HANDLE hStdin;
    DWORD orig_modes;
    DWORD modes;
    #else
    struct termios orig_modes;
    struct termios modes;
    #endif
};


ConsoleIO::ConsoleIO(std::istream& cin, std::ostream& cout, std::ostream& cerr, bool stdio)
: in(cin),
  out(cout),
  err(cerr),
  stdio_(stdio),
  data(new ConsoleCTX()) {

    stateSave();
}

ConsoleIO::~ConsoleIO() {
    stateRestore();
}


void
ConsoleIO::stateSave() {
    if (!stdio_)
        return;

    #ifdef WIN32
    data->hStdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(data->hStdin, &data->modes);

    #else
    tcgetattr(0, &data->modes);
    #endif

    data->orig_modes = data->modes;
}

void
ConsoleIO::stateRestore() {
    if (!stdio_)
        return;

    #ifdef WIN32
    SetConsoleMode(data->hStdin, data->orig_modes );
    #else
    tcsetattr(0, 0, &data->orig_modes);
    #endif
}


void
ConsoleIO::setEcho(bool enable){
    if (!stdio_)
        return;

    // Set terminal to a usuable state
    #ifdef WIN32
    if( !enable ) {
        data->modes &= ~ENABLE_ECHO_INPUT;
    } else {
        data->modes |= ENABLE_ECHO_INPUT;
    }
    SetConsoleMode(data->hStdin, data->modes );
    #else
    if (enable) {
        data->modes.c_lflag |= ECHO;
    } else {
        data->modes.c_lflag &= ~ECHO;
    }
    tcsetattr(0, 0, &data->modes);
    #endif
}

void
ConsoleIO::setCannonical( bool canon){
    if (!stdio_)
        return;

    #ifdef WIN32
    if( !canon ) {
        data->modes &= ~ENABLE_LINE_INPUT;
    } else {
        data->modes |= ENABLE_LINE_INPUT;
    }
    SetConsoleMode(data->hStdin, data->modes );
    #else
    if (canon) {
        data->modes.c_lflag |= ICANON;
    } else {
        data->modes.c_lflag &= ~ICANON;
    }
    tcsetattr(0, 0, &data->modes);
    #endif
}



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
