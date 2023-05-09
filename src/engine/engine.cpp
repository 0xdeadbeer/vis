#include <stdlib.h>
#include <string.h>
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

        // check if day has an event
        // TOOD: Temporary solution
        calendar_information tmp_date = date_info;
        tmp_date.current_day = i+1;

        if (this->events_map.count(tmp_date) > 0)
            wattron(win, COLOR_PAIR(2));

        if (this->active_cell == i) 
            wattron(win, COLOR_PAIR(1));

        mvwprintw(win, y_location, x_location, "D%d", i+1);
        wattroff(win, COLOR_PAIR(1));
        wattroff(win, COLOR_PAIR(2));
    }
}

void Engine::ui_week_draw(WINDOW *win) {
    // TODO
}

void Engine::ui_months_draw(WINDOW *win) {
    // TODO
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
    std::string current_view_mode = calendar_view_mode_str[this->view_mode];

    mvwprintw(win, LINES-2, 1, "Year: %d, Month: %d, Day: %d, Mode: %s\n", date_info.current_year, date_info.current_month, date_info.current_day, current_view_mode.c_str());

    if (VIS_COLORING) wattroff(win, COLOR_PAIR(1));
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
        case 'i': { // TODO: Break down this monstrocity of a case
            endwin();
            int vipe_out[2], vipe_in[2];  
            char output_buffer[4096];
            memset(output_buffer, 0, sizeof(output_buffer));

            pipe(vipe_out);
            pipe(vipe_in);
            pid_t pid = fork();

            if (pid == 0) {
                dup2(vipe_in[0], STDIN_FILENO);
                dup2(vipe_out[1], STDOUT_FILENO);

                close(vipe_in[0]);
                close(vipe_in[1]);
                close(vipe_out[0]);
                close(vipe_out[1]);

                system("vipe");
                exit(EXIT_FAILURE);
            } else {
                close(vipe_in[0]);
                close(vipe_out[1]);

                std::string event = this->events_map[this->calendar->get_info()];
                write(vipe_in[1], event.c_str(), event.length()); 
                close(vipe_in[1]);

                int nbytes = read(vipe_out[0], output_buffer, sizeof(output_buffer));

                if (nbytes > 0) 
                    this->events_map[this->calendar->get_info()] = output_buffer;
                else 
                    this->events_map.erase(this->calendar->get_info());
            }
            
            wclear(win);
            wrefresh(win);
            break;
        }
        case 'x': {
            calendar_information date_info = this->calendar->get_info();
            this->events_map.erase(date_info);
            break;
        }
        case 'g': 
            this->calendar->set_day(1);
            this->active_cell = 0;
            break;
        case 'G':
            this->calendar->set_day(this->calendar->get_info().current_month_days);
            this->active_cell = this->calendar->get_info().current_month_days-1;
            break;
        case 't': 
            this->view_mode = WEEK_VIEW;
            wclear(win);
            break;
    }
}

void Engine::input_handle_week(WINDOW *win) {
    // TODO
}

void Engine::input_handle_months(WINDOW *win) {
    // TODO
}
