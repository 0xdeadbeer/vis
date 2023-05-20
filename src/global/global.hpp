#ifndef VIS_GLOBAL 
#define VIS_GLOBAL 

#include <signal.h>
#include <ncurses.h>

#define VIS_COLORING has_colors() && can_change_color()
volatile sig_atomic_t winch_flag = 0; // DESCRIPTION: flags if the resolution of the terminal has changed
volatile sig_atomic_t low_res_flag = 0; // DESCRIPTION: flags if the resolution of the terminal is too small

#endif 
