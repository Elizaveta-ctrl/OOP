#include "ContactManager.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>  // для std::move

// Исправление 1: Pass by value and use std::move
ContactManager::ContactManager(std::string _filename) : filename(std::move(_filename)) {
    loadFromFile();
}

void ContactManager::addContact(const Contact& contact) {
    contacts.push_back(contact);
    if (!saveToFile()) {
        std::cerr << "Ошибка: не удалось сохранить контакт в файл" << std::endl;
        contacts.pop_back(); //откатываем изменения в памяти
    }
}

void ContactManager::removeContact(size_t index) {
    if (index >= contacts.size()) {
        throw std::out_of_range("Индекс контакта вне диапазона");
    }

    //Явное преобразование типа для избежания narrowing conversion
    auto difference = static_cast<std::vector<Contact>::difference_type>(index);
    contacts.erase(contacts.begin() + difference);

    if (!saveToFile()) {
        std::cerr << "Ошибка: не удалось сохранить изменения в файл" << std::endl;
    }
}

void ContactManager::editContact(size_t index, const Contact& newContact) {
    if (index >= contacts.size()) {
        throw std::out_of_range("Индекс контакта вне диапазона");
    }

    Contact oldContact = contacts[index]; //сохраняем старый контакт на случай ошибки сохранения
    contacts[index] = newContact; //заменяем контакт

    if (!saveToFile()) {
        std::cerr << "Ошибка: не удалось сохранить изменения в файл" << std::endl;
        contacts[index] = oldContact;
        throw std::runtime_error("Не удалось сохранить изменения контакта");
    }
}

const std::vector<Contact>& ContactManager::getContacts() const {
    return contacts;
}

bool ContactManager::saveToFile() {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл " << filename << std::endl;
        return false;
    }

    for (const auto& contact : contacts) {
        file << contact.getLastName() << "|"
             << contact.getFirstName() << "|"
             << contact.getMiddleName() << "|"
             << contact.getBirthDate() << "|"
             << contact.getAddress() << "|"
             << contact.getEmail() << "|";

        size_t phoneCount = contact.getPhoneNumbersCount();
        for (size_t i = 0; i < phoneCount; ++i) {
            file << contact.getPhoneNumber(i);
            if (i < phoneCount - 1) file << ";";
        }
        file << "\n";
    }

    file.close();
    std::cout << "Успешно сохранено контактов: " << contacts.size() << std::endl;
    return true;
}

bool ContactManager::loadFromFile() {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Файл " << filename << " не найден. Будет создан при сохранении данных в него." << std::endl;
        return false;
    }

    std::vector<Contact> newContacts;
    std::string line;
    int loadedCount = 0;
    int errorCount = 0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 6) {
            try {
                const std::string& dateStr = tokens[3];
                size_t pos1 = dateStr.find('.');
                size_t pos2 = dateStr.find('.', pos1 + 1);

                if (pos1 != std::string::npos && pos2 != std::string::npos) {
                    int day = std::stoi(dateStr.substr(0, pos1));
                    int month = std::stoi(dateStr.substr(pos1 + 1, pos2 - pos1 - 1));
                    int year = std::stoi(dateStr.substr(pos2 + 1));

                    BirthDate birthDate(year, month, day);
                    Contact contact(tokens[1], tokens[0], tokens[2], birthDate, tokens[4], tokens[5]);

                    if (tokens.size() > 6 && !tokens[6].empty()) {
                        std::istringstream phoneStream(tokens[6]);
                        std::string phone;
                        while (std::getline(phoneStream, phone, ';')) {
                            if (!phone.empty()) {
                                contact.addPhoneNumber(phone);
                            }
                        }
                    }
                    newContacts.push_back(contact);
                    loadedCount++;
                } else {
                    std::cerr << "Ошибка формата даты в строке: " << line << std::endl;
                    errorCount++;
                }
            } catch (const std::exception& e) {
                std::cerr << "Ошибка создания контакта из строки: " << line << std::endl;
                errorCount++;
            }
        } else {
            std::cerr << "Недостаточно данных в строке: " << line << std::endl;
            errorCount++;
        }
    }

    file.close();

    if (loadedCount > 0) {
        contacts = newContacts;
    }

    std::cout << "Загрузка завершена. Успешно: " << loadedCount
              << ", с ошибками: " << errorCount << std::endl;

    return loadedCount > 0;
}

void ContactManager::printAllContacts() const {
    std::cout << "\n ТЕЛЕФОННЫЙ СПРАВОЧНИК (" << contacts.size() << " контактов)" << std::endl;

    if (contacts.empty()) {
        std::cout << "Справочник пуст" << std::endl;
        return;
    }

    for (size_t i = 0; i < contacts.size(); ++i) {
        std::cout << "Контакт #" << i + 1 << ":\n"
                  << "  ФИО: " << contacts[i].getLastName() << " "
                  << contacts[i].getFirstName() << " "
                  << contacts[i].getMiddleName() << "\n"
                  << "  Дата рождения: " << contacts[i].getBirthDate() << "\n"
                  << "  Адрес: " << contacts[i].getAddress() << "\n"
                  << "  Email: " << contacts[i].getEmail() << "\n";

        size_t phoneCount = contacts[i].getPhoneNumbersCount();
        if (phoneCount > 0) {
            std::cout << "  Телефоны: ";
            for (size_t j = 0; j < phoneCount; ++j) {
                std::cout << contacts[i].getPhoneNumber(j);
                if (j < phoneCount - 1) std::cout << ", ";
            }
            std::cout << "\n";
        } else {
            std::cout << "  Телефоны: нет\n";
        }
        std::cout << "----------------------------\n";
    }
}

size_t ContactManager::getContactsCount() const {
    return contacts.size();
}

bool ContactManager::hasContacts() const {
    return !contacts.empty();
}