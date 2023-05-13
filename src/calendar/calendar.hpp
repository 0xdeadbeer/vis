#ifndef CALENDAR_HPP
#define CALENDAR_HPP 

#include <tuple>

struct calendar_information {
    int current_day; 
    int current_month;
    int current_month_days; 
    int current_year; 

    calendar_information();
    calendar_information(int cday, int cmonth, int cyear);
}; 

const bool operator<(const calendar_information& first, const calendar_information& second) {
    return std::tie(first.current_day, first.current_month, first.current_month_days, first.current_year) < 
           std::tie(second.current_day, second.current_month, second.current_month_days, second.current_year);
}

class Calendar {
    public: 
        static int get_days_in_month(int month, int year);
        void set_day(int day); 
        void set_month(int month);
        void set_year(int year);

        calendar_information get_info();
        calendar_information get_current_date();
        Calendar(calendar_information *preinfo = nullptr); 

    private:
        calendar_information info; // mutable date that can be changed
        calendar_information current_date; // current date 
}; 

int get_weekday(calendar_information *date_info = nullptr);
int datecmp(calendar_information *one, calendar_information *two);

#endif
