#include <iostream>
#include <string.h>
#include <ncurses.h>
#include <curses.h>
#include "calendar.hpp"

#define MIN_WIDTH 100
#define MIN_HEIGHT 40

using namespace std; 

int main() {
    initscr();   

    int active_cell = 0; 

    for (;;) {

        refresh(); 
    }

    return 0; 
}
