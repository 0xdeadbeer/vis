#include <stdlib.h>
#include <string>
#include <string.h>
#include <ncurses.h>
#include <signal.h>
#include "engine.hpp"

Engine::Engine(int padding) {
    this->padding = padding;

    calendar_information date_info = calendar_information();
    this->calendar = new Calendar(&date_info);
    this->active_cell = --date_info.current_day;
}

Engine::~Engine() {
    delete this->calendar;
}

void Engine::ui_draw(WINDOW *win) {
    int screen_width, screen_height;
    getmaxyx(win, screen_height, screen_width);

    if (screen_height < 25 || screen_width < 70) {
        this->ui_warning_draw(win, "Window resolution too small, please resize it."); 
        return;
    }

    low_res_flag = 0;
    box(win, 0, 0);

    keypad(win, TRUE);
    keypad(stdscr, TRUE);

    this->ui_month_draw(win);
    this->ui_top_draw(win);
    this->ui_bottom_draw(win);
    wrefresh(win);
}

void Engine::ui_warning_draw(WINDOW *win, const char *message) {
    low_res_flag = 1;

    mvwprintw(win, 0, 0, "%s", message);

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
        // TODO: Temporary solution
        calendar_information tmp_date = date_info;
        tmp_date.current_day = i+1;

        if (this->events_map.count(tmp_date) > 0)
            wattron(win, COLOR_PAIR(2));

        calendar_information current_date = this->calendar->get_current_date();
        if (datecmp(&current_date, &tmp_date)) 
            wattron(win, COLOR_PAIR(3));

        if (this->active_cell == i) 
            wattron(win, COLOR_PAIR(1));

        std::string weekday_prefix = weekday_prefixes[get_weekday(&tmp_date)];

        // mvwprintw(win, y_location, x_location, "D%d", i+1);
        mvwprintw(win, y_location, x_location, "%s[%d]", weekday_prefix.c_str(), i+1);
        wattroff(win, COLOR_PAIR(1));
        wattroff(win, COLOR_PAIR(2));
        wattroff(win, COLOR_PAIR(3));
    }
}

void Engine::ui_top_draw(WINDOW *win) {
    if (VIS_COLORING) wattron(win, COLOR_PAIR(1));

    mvwprintw(win, 1, 1, "Vi Scheduler");

    if (VIS_COLORING) wattroff(win, COLOR_PAIR(1));
}

void Engine::ui_bottom_draw(WINDOW *win) {
    if (VIS_COLORING) wattron(win, COLOR_PAIR(1));

    calendar_information date_info = this->calendar->get_info();

    mvwprintw(win, LINES-2, 1, "Year: %d, Month: %d, Day: %d\n", date_info.current_year, date_info.current_month, date_info.current_day);

    if (VIS_COLORING) wattroff(win, COLOR_PAIR(1));
}

void Engine::input_handle(WINDOW *win) {
    char key = getch(); 

    if (key == ERR) return;
    if (low_res_flag) return;

    if (this->input_handle_universal(win, key))
        return;

    this->input_handle_month(win, key);
}

bool Engine::input_handle_universal(WINDOW *win, char key) {
    int day;
    switch (key) {
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
        case 'w': 
            this->write_calendar();
            break;
        case 'q': 
            endwin(); 
            this->calendar_file.close();

            exit(EXIT_SUCCESS);
            break;
        default: 
            return false;
            break;
    } 

    return true;
}

void Engine::input_handle_month(WINDOW *win, char key) {
    calendar_information date_info = this->calendar->get_info();

    int day, month;
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
        case 'g': 
            this->calendar->set_day(1);
            this->active_cell = 0;
            break;
        case 'G':
            this->calendar->set_day(this->calendar->get_info().current_month_days);
            this->active_cell = this->calendar->get_info().current_month_days-1;
            break;
    }
}

calendar_information Engine::parse_date(std::string date) {
    char *token = strtok((char *) date.c_str(), ".");  
    std::vector<int> tokens;

    while (token != NULL) {
        try {
            tokens.push_back(std::stoi(token));
        } catch (...) {
            std::cout << "Error parsing calendar file. Suspected incorrect format." << std::endl;
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, ".");
    }

    return calendar_information(tokens[0], tokens[1], tokens[2]);
}

void Engine::parse_line(std::string line) {
    char *token = strtok((char *) line.c_str(), " ");
    std::vector<std::string> tokens;

    while (token != NULL) {
        tokens.push_back(token);
        token = strtok(NULL, " ");
    }

    if (tokens.size() != 2) 
        return;

    // decode the event from base64 before adding it into the events_map
    calendar_information event_date = this->parse_date(tokens[0]); 

    int decoded_length = Base64decode_len((const char *) tokens[1].c_str());
    char *event_decoded = (char *) malloc(decoded_length);

    Base64decode(event_decoded, tokens[1].c_str());

    this->events_map[event_date] = event_decoded;
    free(event_decoded);
}

void Engine::open_calendar(char *filename) {
    this->calendar_file.open(filename, std::ios::in);
    this->calendar_file_location = filename;

    // If the file does not exist, 
    // we'll create and reopen it later ('write' shortcut)
    if (this->calendar_file.fail()) 
        return;

    std::string line;
    while (std::getline(this->calendar_file, line)) 
        this->parse_line(line);
}

bool Engine::write_calendar() {
    this->calendar_file.close();
    this->calendar_file.open(this->calendar_file_location, std::ios::out | std::ios::trunc);

    if (this->calendar_file.fail()) {
        mvprintw(0, 0, "Error saving file.");        
        refresh();
        getch();
        return false;
    }

    for (auto const& event : this->events_map) {
        std::stringstream output_line;

        calendar_information date = event.first;

        int encoded_length = Base64encode_len(event.second.length());
        char *event_encoded = (char *) malloc(encoded_length);
        Base64encode(event_encoded, event.second.c_str(), event.second.length());

        output_line << date.current_day << ".";
        output_line << date.current_month << "."; 
        output_line << date.current_year << " "; 
        output_line << event_encoded;

        this->calendar_file << output_line.str() << std::endl;
        free(event_encoded);
    }
    
    return true;
}
