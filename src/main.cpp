#include <iostream>
#include <string.h>
#include <signal.h>
#include <ncurses.h>
#include "calendar/calendar.hpp"
#include "engine/engine.hpp"
#include "global/global.hpp"

Engine engine = Engine(10, MONTH_VIEW);
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

int main() {
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
    }

    // handle required signals 
    signals();

    for (;;) {
        engine.ui_draw(main_win);

        if (engine.view_mode == MONTH_VIEW) 
            engine.input_handle_month(main_win);
        if (engine.view_mode == WEEK_VIEW) 
            engine.input_handle_week(main_win);
        else if (engine.view_mode == MONTHS_VIEW) 
            engine.input_handle_months(main_win);
    }

    endwin();

    return 0; 
}
