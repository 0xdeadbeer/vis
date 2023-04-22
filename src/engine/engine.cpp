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

void Engine::ui_draw(WINDOW *win) {
    this->cells_table.clear(); 

    box(win, 0, 0);

    switch (this->view_mode) {
        case MONTH_VIEW:
            this->ui_month_draw(win);
            break;
        case MONTHS_VIEW:
            this->ui_months_draw(win);
            break;
    }

    this->ui_bottom_draw(win);

    wrefresh(win);
}

void Engine::ui_month_draw(WINDOW *win) {
    
}

void Engine::ui_months_draw(WINDOW *win) {

}

void Engine::ui_bottom_draw(WINDOW *win) {
    if (VIS_COLORING) wattron(win, COLOR_PAIR(1));

    calendar_information date_info = this->calendar->get_info();
    mvwprintw(win, LINES-2, 1, "Year: %d, Month: %d, Day: %d", date_info.current_year, date_info.current_month, date_info.current_day);

    if (VIS_COLORING) wattroff(win, COLOR_PAIR(1));
}
