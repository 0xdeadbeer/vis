#ifndef ENGINE_HPP 
#define ENGINE_HPP 

#include <iostream>
#include <sstream>
#include <fstream>
#include <ncurses.h>
#include <vector>
#include <map>
#include "../base64/base64.h"
#include "../calendar/calendar.hpp"
#include "../global/global.hpp"

std::string weekday_prefixes[] = {
    "Sun",
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat"
};

// title: drawing engine
// description: use for drawing the TUI and interact with the cells
class Engine {
    public: 
        Engine(int padding);
        ~Engine();

        void ui_draw(WINDOW *win);
        void ui_warning_draw(WINDOW *win, const char *message);
        void ui_month_draw(WINDOW *win); 
        void ui_bottom_draw(WINDOW *win);
        void ui_top_draw(WINDOW *win);

        bool input_block;
        void input_handle(WINDOW* win);
        bool input_handle_universal(WINDOW *win, char key);
        void input_handle_month(WINDOW *win, char key);

        calendar_information parse_date(std::string date);
        void parse_line(std::string line);
        void open_calendar(char *filename);
        bool write_calendar();

        Calendar *calendar; 

        std::map<calendar_information, std::string> events_map;
        int active_cell;

        int padding; // GUI padding
    private:
        std::fstream calendar_file; 
        char *calendar_file_location;
        calendar_information clipboard_day;
};

#endif

