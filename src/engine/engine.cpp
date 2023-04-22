#include <stdlib.h>
#include <ncurses.h>
#include "engine.hpp"

Engine::Engine(int padding, calendar_view_mode view_mode) {
    this->padding = padding;
    this->view_mode = view_mode;
    this->cells_table = std::vector<engine_child>();

    calendar_information date_info = calendar_information();
    this->calendar = new Calendar(&date_info);
}

Engine::~Engine() {
    delete this->calendar;
}

void Engine::draw(WINDOW *win) {
    this->cells_table.clear(); 

    box(win, 0, 0);

    calendar_information date_info = this->calendar->get_info();

    
    switch (this->view_mode) {
        case MONTH_VIEW:
            for (int i = 1; i <= date_info.current_month_days; i++) 
                mvwprintw(win, i, 3, "D%d", i);
            break;
        case MONTHS_VIEW:
            for (int i = 1; i <= 12; i++) 
                wprintw(win, "M%d", i);
            break;
    }

    wprintw(win, "\n");

    mvwprintw(win, LINES-1, 0, "Year: %d, Month: %d, Day: %d", date_info.current_year, date_info.current_month, date_info.current_day);

    wrefresh(win);
}
