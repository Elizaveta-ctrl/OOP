#ifndef CONTACT_H
#define CONTACT_H

#include <iostream>
#include <string>
#include "BirthDate.h"

class Contact {
    std::string firstName;
    std::string lastName;
    std::string middleName;
    BirthDate birthDate;
    std::string address;
    std::string email;
    std::vector<std::string> phoneNumbers;

public:
    Contact(std::string _firstName, std::string _lastName, std::string _middleName, const BirthDate& _birthDate, std::string _address, std::string _email);
    Contact(const Contact &contact);
    //SETTERS
    void setFirstName(const std::string& _firstName);
    void setLastName(const std::string& _lastName);
    void setMiddleName(const std::string& _middleName);
    void setBirthDate(const BirthDate& _birthDate);
    void setAddress(const std::string& _address);
    void setEmail(const std::string& _email);
    void setPhoneNumbers(const std::vector<std::string>& _phoneNumbers);
    //GETTERS
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getMiddleName() const;
    std::string getBirthDate() const;
    std::string getAddress() const;
    std::string getEmail() const;
    std::vector<std::string> getPhoneNumbers() const;
    //МЕТОДЫ ДЛЯ РАБОТЫ С ТЕЛЕФОНАМИ
    void addPhoneNumber(const std::string& phone); //добавить один телефон
    void addPhoneNumbers(const std::vector<std::string>& phones); //добавить несколько телефонов
    void removePhoneNumber(size_t index); //удалить телефон по индексу
    void clearPhoneNumbers(); //очистить все телефоны
    size_t getPhoneNumbersCount() const; //получить количество телефонов
    bool hasPhoneNumbers() const; //проверить есть ли телефоны
    const std::string& getPhoneNumber(size_t index) const; //получить телефон по индексу
    //ВАЛИДАЦИЯ ДАННЫХ
    static bool startsWithLetter(const std::string& name); //начинается с буквы
    static bool noHyphenAtEdges(const std::string& name); //не начинается и не заканчивается дефисом
    static bool containsOnlyAllowedChars(const std::string& name); //только разрешенные символы
    static std::string normalizeNameField(const std::string& name); //удаляем лишние пробелы
    static bool validateNameField(const std::string& name); //проверка одного поля на соответствие требованиям
    static std::string normalizeEmail(const std::string& email);
    static bool validateEmail(const std::string& email);
    bool validateNames() const; //проверка всех полей (фио) на соответствие требованиям
    bool validatePhoneNumbers() const;
    static bool isValidPhoneNumber(const std::string& phone); // Вспомогательный метод для проверки одного номера
};



#endif //CONTACT_H
