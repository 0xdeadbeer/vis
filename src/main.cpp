#include <iostream>
#include <string.h>
#include <signal.h>
#include <ncurses.h>
#include "calendar/calendar.hpp"
#include "engine/engine.hpp"
#include "global/global.hpp"

Engine engine(10);
WINDOW *main_win; 

void sig_winch(int sig) {
    winch_flag = 1;
}

void signals() {
    signal(SIGWINCH, sig_winch);
}

void print_help() {
    std::cout << "\
VIS - Vim Scheduler\
\n\
\nUsage: vis [file]\
\n\
\nModes: \
\n  - Month view (default): view the current month\
\n  - Week view: view the current week\
\n  - Months view: view the current year\
" << std::endl;
}

bool check_args(int argc, char **argv) {
    if (argc != 2) {
        print_help();
        return false;
    }

    engine.open_calendar(argv[1]);

    return true;
}

int main(int argc, char **argv) {
    if (!check_args(argc, argv))
        return EXIT_FAILURE;

    initscr();
    refresh();

    curs_set(0); // invisible cursor 
    noecho();
    halfdelay(1);
    refresh();

    main_win = newwin(LINES, COLS, 0, 0);
    if (VIS_COLORING) {
        start_color(); 
        use_default_colors();

        init_color(COLOR_BLACK, 0, 0, 0);
        init_color(COLOR_WHITE, 1000, 1000, 1000);

        init_pair(0, COLOR_WHITE, COLOR_BLACK);
        init_pair(1, COLOR_BLACK, COLOR_WHITE); 
        init_pair(2, COLOR_BLACK, COLOR_RED); 
        init_pair(3, COLOR_BLACK, COLOR_GREEN); 
    }

    // handle required signals 
    signals();

    for (;;) {
        if (winch_flag) {
            winch_flag = 0; 
            endwin();
            wrefresh(main_win);
            wclear(main_win);

            wresize(main_win, LINES, COLS);
            mvwin(main_win, 0, 0);
        }

        engine.ui_draw(main_win);
        engine.input_handle(main_win);
    }


    endwin();

    return 0; 
}
