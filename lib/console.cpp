/***********************************************************************
 Amethyst Physics Library
  - Library Console CLI class implementations

 Authors (c):
 2003-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "lib/console.h"

#include <string>
#include <vector>
#include <functional>

#include <signal.h>

#include <stdlib.h>


#include "console_io.h"
#include "console_defs.h"

#include "console_menu.h"

#include "version.h"

namespace amethyst {

ConsoleCLI::ConsoleCLI(ConsoleIO& io,
                       std::string& newprompt,
                       Console_Menu& mainmenu,
                       std::vector<std::string>& newhistory)
: prompt_(newprompt),
  mainmenu_(mainmenu),
  history_(newhistory),
  io_(io) {
}


ConsoleCLI::~ConsoleCLI() = default;


void
ConsoleCLI::start() {
    using namespace std::placeholders;

    // Set terminal to a usuable state
    tcgetattr(0, &modes_);     /* get the current terminal modes */
    orig_modes_ = modes_;       // save the original settings
    modes_.c_lflag &= ~ICANON; /* turn off canonical mode */
    modes_.c_lflag &= ~ECHO;   /* turn off echoing */
    tcsetattr(0, 0, &modes_);  /* set the new modes */

    //Speed up IO operations
    //std::ios::sync_with_stdio(false);

    //Set signal handler to catch CTRL-C
    //signal(SIGINT, [this](int signum) -> void { catch_signal(signum); });



    //Display version information
    io_.out << '\n' << AMETHYST_VER << '\n';

    //Instantiate command buffer
    std::string command;

    //Endless command loop
    for (;;) {
        buffer_.clear();
        show_prompt();
        io_.out.flush();

        command = command_get();

        switch (command_parse(command)) {
            case 1:
                io_.out << command << " :Command Not Recognized..." << '\n';
                break;

            case 2:
                stop("");
                return;
                break;
        }
    }

}


void
ConsoleCLI::stop(const std::string&) {
    io_.out << "\n";
    tcsetattr(0, 0, &orig_modes_);
    exit(0);
}


void
ConsoleCLI::catch_signal(int /*unused*/) {
    stop("");
}


std::string
ConsoleCLI::command_get(void) {
    unsigned int curpos = 0; // Cursor Position
    int    ret;
    char   one;        // Input Charater


    tcsetattr(0, 0, &modes_);

    do {
        ret = io_.getch(one);

        if (ret == 0) {
            if (one != '\n') {
                unsigned int gb = 0;
                buffer_.insert(curpos, 1, one);

                for (; gb < (buffer_.length() - curpos); gb++) {
                    io_.out << buffer_[curpos+gb];
                }

                io_.cursor_right(1);

                do {
                    io_.cursor_left(1);
                    gb--;
                } while (gb != 0);

                io_.out.flush();
                curpos++;
            }
        } else {
            switch (ret) {
                case KEY_LEFT     : {
                    if (curpos != 0) {
                        io_.cursor_left(1);
                        curpos--;
                    }

                    break;
                }

                case KEY_RIGHT    : {
                    if (curpos < buffer_.length()) {
                        io_.cursor_right(1);
                        curpos++;
                    }

                    break;
                }

                case KEY_UP: {
                    if (history_pos_ > 0) {

                        auto oldbuffsize = buffer_.size();
                        auto oldcurpos = curpos;

                        if (history_pos_ == history_.size())
                            tmpbuffer_ = buffer_;

                        history_pos_--;
                        buffer_ = history_[history_pos_];

                        curpos = buffer_.size();

                        if (curpos > oldcurpos)
                            io_.cursor_right(curpos - oldcurpos);
                        else
                            io_.cursor_left(oldcurpos - curpos);

                        // clear forward
                        while (curpos < oldbuffsize) {
                            curpos++;
                            io_.out <<  " ";
                        }

                        io_.cursor_left(curpos);
                        io_.out << buffer_;
                        curpos = buffer_.size();
                        io_.out.flush();

                    }
                    break;
                }

                case KEY_DOWN: {
                    if (history_pos_ < history_.size()) {

                        history_pos_++;

                        auto oldbuffsize = buffer_.size();
                        auto oldcurpos = curpos;

                        if (history_pos_ ==  history_.size()) {
                            buffer_ = tmpbuffer_;
                        } else {
                            buffer_ = history_[history_pos_];
                        }

                        curpos = buffer_.size();

                        if (curpos > oldcurpos)
                            io_.cursor_right(curpos - oldcurpos);
                        else
                            io_.cursor_left(oldcurpos - curpos);

                        // clear forward
                        while (curpos < oldbuffsize) {
                            curpos++;
                            io_.out <<  " ";
                        }

                        io_.cursor_left(curpos);

                        io_.out << buffer_;
                        curpos = buffer_.size();
                        io_.out.flush();

                    }

                    break;
                }

                case KEY_BACKSPACE: {
                    if (curpos != 0) {
                        int gb = 0;
                        io_.cursor_left(1);
                        io_.out << " ";
                        io_.cursor_left(1);
                        buffer_.erase(curpos -1, 1);
                        curpos--;

                        for (unsigned int i = 0; i < (buffer_.length()-curpos); i++) {
                            io_.out << buffer_[i+curpos];
                            gb++;
                        }

                        io_.out << " ";
                        gb++;

                        do {
                            io_.cursor_left(1);
                            gb--;
                        } while (gb != 0);
                    }

                    break;
                }

                case KEY_TAB: {
                    //Run tab completion logic
                    std::string extra;
                    int retval = mainmenu_.list(buffer_.substr(0, curpos), extra);

                    if (retval == 1) {
                        buffer_ += extra;
                        curpos += extra.size();
                        io_.out << extra;
                        io_.out.flush();
                    } else if (retval > 1) {
                        show_prompt();
                    }

                    //cout << "RESULT: " << extra;




                    //cout << "[TAB]";
                    break;
                }
            }

            //cout << ret;
        }
    } while (one != '\n');

    tcsetattr(0, 0, &orig_modes_);

    return buffer_;
}


int
ConsoleCLI::command_parse(const std::string& command) {
    //Check for null command
    if (command == "") return 0;

    //Add to command history
    history_.push_back(command);
    history_pos_ = history_.size();

    //Run command
    return mainmenu_.run(io_, command);
}


void
ConsoleCLI::show_prompt(void) {
    io_.out << '\n' << prompt_ << buffer_;
}

}  // namespace amethyst
