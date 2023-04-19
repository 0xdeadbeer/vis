struct calendar_information {
    int current_day; 
    int current_month;
    int current_month_days; 
    int current_year; 

    calendar_information();
}; 

class Calendar {
    public: 
        static int get_days_in_month(int month, int year);
        void set_day(int day); 
        void set_month(int month);
        void set_year(int year);
        calendar_information get_info();

        Calendar(calendar_information *preinfo = nullptr); 
    private: 
        calendar_information info; 
}; 
