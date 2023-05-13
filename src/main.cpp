#include <iostream>
#include <string.h>
#include <signal.h>
#include <ncurses.h>
#include "calendar/calendar.hpp"
#include "engine/engine.hpp"
#include "global/global.hpp"

Engine engine(10, MONTH_VIEW);
WINDOW *main_win; 

void sig_winch(int sig) {
    if (isendwin()) return;
    endwin();

    wclear(main_win);
    wrefresh(main_win);

    wresize(main_win, LINES, COLS);

    mvwin(main_win, 0, 0);
    
    // redraw the TUI after the resize signal
    engine.ui_draw(main_win);

    flushinp();
    return;
}

void signals() {
    signal(SIGWINCH, sig_winch);
}

void print_help() {

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
        engine.ui_draw(main_win);

        engine.input_handle(main_win);
    }


    endwin();

    return 0; 
}
