#include <iostream>
#include <string.h>
#include <signal.h>
#include <ncurses.h>
#include "calendar/calendar.hpp"
#include "engine/engine.hpp"

Engine engine = Engine(10, MONTH_VIEW);
WINDOW *main_win; 

void sig_winch(int sig) {
    if (isendwin()) return;
    endwin();
    wclear(main_win);
    wrefresh(main_win);

    wresize(main_win, LINES, COLS);

    // redraw the TUI after the resize signal
    engine.draw(main_win);

    flushinp();
}

void signals() {
    signal(SIGWINCH, sig_winch);
}

int main() {
    initscr();
    refresh();
    noecho();

    main_win = newwin(LINES, COLS, 0, 0);

    // handle required signals 
    signals();

    for (;;) {
        wclear(main_win);
        wrefresh(main_win);
        engine.draw(main_win);
       
        char ch = getch();
        int month;
        switch (ch) {
            case 'k': 
                month = engine.calendar->get_info().current_month;
                if (month >= 12) {
                    month = 12; 
                    break;
                }

                engine.calendar->set_month(++month);
                break;
            case 'j': 
                month = engine.calendar->get_info().current_month;
                if (month <= 1) {
                    month = 1; 
                    break;
                }

                engine.calendar->set_month(--month);
                break;
        }
    }


    endwin();

    return 0; 
}
