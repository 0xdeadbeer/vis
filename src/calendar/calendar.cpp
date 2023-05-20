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

calendar_information::calendar_information(int cday, int cmonth, int cyear) {
    current_day = cday;
    current_month = cmonth;
    current_year = cyear;
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

calendar_information Calendar::get_current_date() {
    return this->current_date;
}

Calendar::Calendar(calendar_information *preinfo) {
    this->current_date = calendar_information();
    this->info = *preinfo; 

    if (preinfo != nullptr)
        return; 

    this->info = calendar_information(); 
}

// https://stackoverflow.com/questions/6054016/c-program-to-find-day-of-week-given-date
// RETURNS:
// Number of the given day in its week. 
//  Note: it starts with sunday being 0
int get_weekday(calendar_information *date_info) {
    if (date_info == nullptr) 
        return 0;
    
    int d = date_info->current_day;
    int m = date_info->current_month;
    int y = date_info->current_year;

    return (d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7;  

}

int datecmp(calendar_information *one, calendar_information *two) {
    if (one->current_day != two->current_day) return 0;
    if (one->current_month != two->current_month) return 0;
    if (one->current_month_days != two->current_month_days) return 0;
    if (one->current_year != two->current_year) return 0;
    return 1;
}

