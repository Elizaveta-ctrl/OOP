#ifndef DATE_H
#define DATE_H
#include <iostream>

class BirthDate {
    int year;
    int month;
    int day;
    static bool ValidDate(int _year, int _month, int _day);
public:
    BirthDate(int _year, int _month, int _day);
    BirthDate(const BirthDate &OtherDate);
    //getters
    std::string getDate() const;
    int getYear() const;
    int getMonth() const;
    int getDay() const;
    //setters
    void SetDate(int _year, int _month, int _day);
};

#endif //DATE_H
