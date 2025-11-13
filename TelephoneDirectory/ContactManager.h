#ifndef CONTACTMANAGER_H
#define CONTACTMANAGER_H

#include "Contact.h"
#include <vector>
#include <fstream>
#include <string>

class ContactManager {
    std::vector<Contact> contacts;
    std::string filename;

public:
    ContactManager(std::string _filename = "contacts.txt");

    // Основные операции с контактами
    void addContact(const Contact& contact);
    void removeContact(size_t index);
    void editContact(size_t index, const Contact& newContact);
    const std::vector<Contact>& getContacts() const;

    // Работа с файлами
    bool saveToFile();
    bool loadFromFile();

    // Вспомогательные методы
    void printAllContacts() const;
    size_t getContactsCount() const;
    bool hasContacts() const;
};

#endif //CONTACTMANAGER_H
