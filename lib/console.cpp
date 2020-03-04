/***********************************************************************
 Amethyst Physics Library
  - Library Console CLI class implementations

 Authors (c):
 2003-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "lib/console.h"

#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include <signal.h>

#include <stdlib.h>


#include "console_io.h"
#include "console_defs.h"

#include "console_menu.h"

#include "version.h"

using namespace std;

namespace amethyst {

ConsoleCLI::ConsoleCLI(std::string& newprompt, Console_Menu& mainmenu, std::vector<std::string>& newhistory)
    : prompt_(newprompt),
      mainmenu_(mainmenu),
      history_(newhistory) {
}

ConsoleCLI::~ConsoleCLI() {
}



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

    //Add exit and quit functions to main menu becuase these functions are defined in this file
    mainmenu_.add("exit", [&](const std::string& s) {
        this->stop(s);
    });
    mainmenu_.add("quit", std::bind(&ConsoleCLI::stop, this, _1));

    //Display version information
    cout << endl << AMETHYST_VER << endl;

    //Instantiate command buffer
    string command;

    //Endless command loop
    for (;;) {
        buffer_.clear();
        show_prompt();
        cout.flush();

        command = command_get();
        std::cout << std::endl;

        switch (command_parse(command)) {
            case 1:
                cout << command << " :Command Not Recognized..." << endl;
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
    cout << "\n";
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
        ret = getch(one);

        if (ret == 0) {
            if (one != '\n') {
                unsigned int gb = 0;
                buffer_.insert(curpos, 1, one);

                for (; gb < (buffer_.length() - curpos); gb++) {
                    cout << buffer_[curpos+gb];
                }

                cursor_right(1);

                do {
                    cursor_left(1);
                    gb--;
                } while (gb != 0);

                cout.flush();
                curpos++;
            }
        } else {
            switch (ret) {
                case KEY_LEFT     : {
                    if (curpos != 0) {
                        cursor_left(1);
                        curpos--;
                    }

                    break;
                }

                case KEY_RIGHT    : {
                    if (curpos < buffer_.length()) {
                        cursor_right(1);
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
                            cursor_right(curpos - oldcurpos);
                        else
                            cursor_left(oldcurpos - curpos);

                        // clear forward
                        while (curpos < oldbuffsize) {
                            curpos++;
                            std::cout <<  " ";
                        }

                        cursor_left(curpos);
                        std::cout << buffer_;
                        curpos = buffer_.size();
                        std::cout.flush();

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
                            cursor_right(curpos - oldcurpos);
                        else
                            cursor_left(oldcurpos - curpos);

                        // clear forward
                        while (curpos < oldbuffsize) {
                            curpos++;
                            std::cout <<  " ";
                        }

                        cursor_left(curpos);

                        std::cout << buffer_;
                        curpos = buffer_.size();
                        std::cout.flush();

                    }

                    break;
                }

                case KEY_BACKSPACE: {
                    if (curpos != 0) {
                        int gb = 0;
                        cursor_left(1);
                        cout << " ";
                        cursor_left(1);
                        buffer_.erase(curpos -1, 1);
                        curpos--;

                        for (unsigned int i = 0; i < (buffer_.length()-curpos); i++) {
                            cout << buffer_[i+curpos];
                            gb++;
                        }

                        cout << " ";
                        gb++;

                        do {
                            cursor_left(1);
                            gb--;
                        } while (gb != 0);
                    }

                    break;
                }

                case KEY_TAB: {
                    //Run tab completion logic
                    string extra;
                    int retval = mainmenu_.list(buffer_.substr(0, curpos), extra);

                    if (retval == 1) {
                        buffer_ += extra;
                        curpos += extra.size();
                        cout << extra;
                        cout.flush();
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
ConsoleCLI::command_parse(const string& command) {
    //Check for null command
    if (command == "") return 0;

    //Add to command history
    history_.push_back(command);
    history_pos_ = history_.size();

    //Run command
    return mainmenu_.run(command);
}


void
ConsoleCLI::show_prompt(void) {
    cout << std::endl << prompt_ << buffer_;
}

}  // namespace amethyst
