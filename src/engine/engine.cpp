#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>
#include "engine.hpp"

Engine::Engine(int padding, calendar_view_mode view_mode) {
    this->padding = padding;
    this->view_mode = view_mode;
    this->cells_table = std::vector<engine_child>();

    calendar_information date_info = calendar_information();
    this->calendar = new Calendar(&date_info);
    this->active_cell = --date_info.current_day;
}

Engine::~Engine() {
    delete this->calendar;
}

void Engine::ui_draw(WINDOW *win) {
    this->cells_table.clear(); 

    box(win, 0, 0);
    wrefresh(win);

    keypad(win, TRUE);
    keypad(stdscr, TRUE);

    switch (this->view_mode) {
        case MONTH_VIEW:
            this->ui_month_draw(win);
            break;
        case WEEK_VIEW:
            this->ui_week_draw(win);
            break;
        case MONTHS_VIEW:
            this->ui_months_draw(win);
            break;
    }

    this->ui_top_draw(win);
    this->ui_bottom_draw(win);
    wrefresh(win);

}

void Engine::ui_month_draw(WINDOW *win) {
    calendar_information date_info = this->calendar->get_info(); 
    int z = COLS - this->padding;
    int q = LINES - (2*this->padding);
    for (int i = 0; i < date_info.current_month_days; i++) {
        int x_location = this->padding + ((z/7) * (i%7));
        int y_location = this->padding + (q/(date_info.current_month_days/7) * (i/7));

        if (this->active_cell == i) wattron(win, COLOR_PAIR(1));
        mvwprintw(win, y_location, x_location, "D%d", i+1);
        if (this->active_cell == i) wattroff(win, COLOR_PAIR(1));
    }
}

void Engine::ui_week_draw(WINDOW *win) {

}

void Engine::ui_months_draw(WINDOW *win) {

}

void Engine::ui_top_draw(WINDOW *win) {
    if (VIS_COLORING) wattron(win, COLOR_PAIR(1));

    mvwprintw(win, 1, 1, "Vi Scheduler");
    mvwprintw(win, 2, 1, "@0xdeadbeer");

    if (VIS_COLORING) wattroff(win, COLOR_PAIR(1));
}

void Engine::ui_bottom_draw(WINDOW *win) {
    if (VIS_COLORING) wattron(win, COLOR_PAIR(1));

    calendar_information date_info = this->calendar->get_info();
    mvwprintw(win, LINES-2, 1, "Year: %d, Month: %d, Day: %d", date_info.current_year, date_info.current_month, date_info.current_day);

    if (VIS_COLORING) wattroff(win, COLOR_PAIR(1));
}

void Engine::ui_dialog_draw(WINDOW *win) {
    int lines = LINES/2, cols = COLS/2; 
    int y_center = (LINES/2)-(lines/2), x_center = (COLS/2)-(cols/2);

    this->dialog = newwin(lines, cols, y_center, x_center);

    box(this->dialog, 0, 0);

    char str[200];

    mvwprintw(this->dialog, 1, 1, "Enter your name lol: ");
    wrefresh(this->dialog);

    mvwgetnstr(this->dialog, 2, 1, str, 200);
    
    wprintw(this->dialog, "Press q to save or r to retake");

    wrefresh(this->dialog);

}

void Engine::input_handle_month(WINDOW *win) {
    char key = getch();

    calendar_information date_info = this->calendar->get_info();

    int day, month, res;
    switch (key) {
        case 'h': 
            if (this->calendar->get_info().current_day <= 1) break;

            day = this->calendar->get_info().current_day; 
            this->calendar->set_day(--day);

            --this->active_cell;
            break;
        case 'l': 
            if (this->calendar->get_info().current_day >= this->calendar->get_info().current_month_days) break;

            day = this->calendar->get_info().current_day; 
            this->calendar->set_day(++day);

            ++this->active_cell;
            break;
        case 'j': 
            if (this->calendar->get_info().current_day+7 > this->calendar->get_info().current_month_days) break;

            day = this->calendar->get_info().current_day;
            this->calendar->set_day(day+7);

            this->active_cell += 7;
            break;
        case 'k': 
            if (this->calendar->get_info().current_day-7 < 1) break;

            day = this->calendar->get_info().current_day;
            this->calendar->set_day(day-7);

            this->active_cell -= 7;
            break;
        case 'u': 
            if (this->calendar->get_info().current_month <= 1) break;

            month = this->calendar->get_info().current_month;
            this->calendar->set_month(--month);
            this->calendar->set_day(1);

            this->active_cell = 0;
            break;
        case 'o':
            if (this->calendar->get_info().current_month >= 12) break;

            month = this->calendar->get_info().current_month; 
            this->calendar->set_month(++month);
            this->calendar->set_day(1);

            this->active_cell = 0;
            break;
        case 'i':
            endwin();
            int link[2];  
            char foo[4096];

            pipe(link);
            pid_t pid = fork();

            if (pid == 0) {
                dup2(link[1], STDOUT_FILENO);
                close(link[0]);
                close(link[1]);
                system("vipe");
                exit(EXIT_FAILURE);
            } else {
                close(link[1]);
                int nbytes = read(link[0], foo, sizeof(foo));
                printw("Output of vipe: %s\n", foo);
            }

            break;
    }
}

void Engine::input_handle_months(WINDOW *win) {
    // TODO
}
