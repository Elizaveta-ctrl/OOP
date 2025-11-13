#include "BirthDate.h"
#include <iostream>
#include <stdexcept>

bool BirthDate::ValidDate(const int _year, const int _month, const int _day) {
    if (_month < 1 || _month > 12) return false;
    if (_day < 1 || _day > 31) return false;
    // Проверка месяцев с 30 днями
    if ((_month == 4 || _month == 6 || _month == 9 || _month == 11) && _day > 30)
        return false;
    // Проверка февраля
    if (_month == 2) {
        //год високосный, если он либо делится на 4 и не делится на 100, либо делится на 400
        bool isLeap = (_year % 4 == 0 && _year % 100 != 0) || (_year % 400 == 0);
        if (_day > (isLeap ? 29 : 28)) return false;
    }
    // Проверка, что дата не в будущем
    std::time_t t = std::time(nullptr); //функция возвращает текущее время в секундах (t = число секунд с 1970-01-01)
    std::tm* now = std::localtime(&t); //tm - структура для хранения даты и времени, localtime(&t) - преобразует time_t в структуру t
    int currentYear = now->tm_year + 1900; //now->tm_year - содержит количество лет, прошедших с 1900 года
    int currentMonth = now->tm_mon + 1; //now->tm_mon - месяц в диапазоне [0-11] (0 = январь, 11 = декабрь)
    int currentDay = now->tm_mday; //now->tm_mday - день месяца

    if (_year > currentYear) return false;
    if (_year == currentYear && _month > currentMonth) return false;
    if (_year == currentYear && _month == currentMonth && _day > currentDay) return false;
    return true;
}

BirthDate::BirthDate(int _year, int _month, int _day) {
    //проверка валидности даты
    if (!ValidDate(_year, _month, _day)) {
        //создание сообщения об ошибке
        std::string error = "Invalid date in constructor: " +
            std::to_string(_day) + "." +
            std::to_string(_month) + "." +
            std::to_string(_year);
        //исключение
        throw std::invalid_argument(error);
    }
    //ошибок нет - сохраняем дату
    year = _year;
    month = _month;
    day = _day;
}

BirthDate::BirthDate(const BirthDate &OtherDate) {
    year = OtherDate.getYear();
    month = OtherDate.getMonth();
    day = OtherDate.getDay();
}

void BirthDate::SetDate(int _year, int _month, int _day) {
    if (!ValidDate(_year, _month, _day)) {
        std::string error = "Invalid date: " +
            std::to_string(_day) + "." +
            std::to_string(_month) + "." +
            std::to_string(_year) +
            ". Date must be valid and not in future";
        throw std::invalid_argument(error);
    }
    year = _year;
    month = _month;
    day = _day;
}

//getters
std::string BirthDate::getDate() const {
    std::string str_year = std::to_string(year);
    std::string str_month = std::to_string(month);
    std::string str_day = std::to_string(day);
    std::string str_date = str_day + '.' + str_month + '.' + str_year;
    return str_date;
}
int BirthDate::getYear() const {
    return year;
}
int BirthDate::getMonth() const {
    return month;
}
int BirthDate::getDay() const {
    return day;
}

