#include <time.h>
#include "calendar.hpp"

calendar_information::calendar_information() {
    time_t current_time = time(NULL);  
    struct tm *datetime = localtime(&current_time);
    
    current_day = datetime->tm_mday;
    current_month = datetime->tm_mon+1;
    current_year = datetime->tm_year + 1900;
    current_month_days = Calendar::get_days_in_month(current_month, current_year);
}

int Calendar::get_days_in_month(int month, int year) {
    tm date = {0};
    date.tm_year = year - 1900; 
    date.tm_mon = month; 
    date.tm_mday = 0;

    mktime(&date);
    return date.tm_mday; 
}

void Calendar::set_day(int day) {
    if (day > this->info.current_month_days) return; 
    this->info.current_day = day; 
}

void Calendar::set_month(int month) {
    if (month > 12) return;
    this->info.current_month = month;
    this->info.current_month_days = Calendar::get_days_in_month(this->info.current_month, this->info.current_year);
    if (this->info.current_day > this->info.current_month_days) 
        this->info.current_day = this->info.current_month_days; 
}

void Calendar::set_year(int year) {
    this->info.current_year = year; 
    this->info.current_month_days = Calendar::get_days_in_month(this->info.current_month, this->info.current_year); 
    if (this->info.current_day > this->info.current_month_days) 
        this->info.current_day = this->info.current_month_days; 
}

calendar_information Calendar::get_info() {
    return this->info;
}

Calendar::Calendar(calendar_information *preinfo) {
    this->info = *preinfo; 

    if (preinfo != nullptr)
        return; 

    this->info = calendar_information(); 
}
