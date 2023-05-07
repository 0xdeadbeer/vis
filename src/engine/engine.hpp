#ifndef ENGINE_HPP 
#define ENGINE_HPP 

#include <iostream>
#include <ncurses.h>
#include <vector>
// #include <unordered_map>
#include <map>
#include "../calendar/calendar.hpp"
#include "../global/global.hpp"

struct engine_child {
    int x; 
    int y;
};

enum calendar_view_mode {
    MONTH_VIEW = 0, 
    WEEK_VIEW,
    MONTHS_VIEW
};

std::string calendar_view_mode_str[] = {
    "MONTH_VIEW",
    "WEEK_VIEW",
    "MONTHS_VIEW"
};

// title: drawing engine
// description: use for drawing the TUI and interact with the cells
class Engine {
    public: 
        Engine(int padding = 10, calendar_view_mode view_mode = MONTH_VIEW);
        ~Engine();

        void ui_draw(WINDOW *win);
        void ui_month_draw(WINDOW *win); 
        void ui_week_draw(WINDOW *win);
        void ui_months_draw(WINDOW *win);
        void ui_bottom_draw(WINDOW *win);
        void ui_top_draw(WINDOW *win);

        void input_handle_month(WINDOW *win);
        void input_handle_week(WINDOW *win);
        void input_handle_months(WINDOW *win);

        Calendar *calendar; 
        // std::unordered_map<calendar_information, std::string> events_map;
        std::map<calendar_information, std::string> events_map;
        int active_cell;

        calendar_view_mode view_mode; 
        int padding; // padding
        
    private:
        std::vector<engine_child> cells_table;

};

#endif

