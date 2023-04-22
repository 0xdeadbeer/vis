#ifndef ENGINE_HPP 
#define ENGINE_HPP 

#include <ncurses.h>
#include <vector>
#include "../calendar/calendar.hpp"

struct engine_child {
    int x; 
    int y;
};

enum calendar_view_mode {
    MONTH_VIEW = 0, 
    MONTHS_VIEW,
};

// title: drawing engine
// description: use for drawing the TUI and interact with the cells
class Engine {
    public: 
        Engine(int padding = 10, calendar_view_mode view_mode = MONTH_VIEW);
        ~Engine();

        void draw(WINDOW *win);

        Calendar *calendar; 

        calendar_view_mode view_mode; 
        int padding; // padding
        
    private:
        std::vector<engine_child> cells_table;

};

#endif
