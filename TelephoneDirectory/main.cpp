#include "ContactManager.h"
#include <iostream>

void simpleTest() {
    std::cout << "=== ТЕСТ ТЕЛЕФОННОГО СПРАВОЧНИКА ===" << std::endl;

    // Удаляем старый тестовый файл
    remove("test_contacts.txt");

    ContactManager manager("test_contacts.txt");

    try {
        std::cout << "\n1. СОЗДАЕМ КОНТАКТЫ:" << std::endl;

        // Контакт 1: полные данные
        BirthDate bd1(1990, 5, 15);
        Contact contact1("Иван", "Петров", "Сергеевич", bd1, "Москва", "ivan@mail.ru");
        contact1.addPhoneNumber("+79161234567");
        contact1.addPhoneNumber("8(495)123-45-67");
        manager.addContact(contact1);

        // Контакт 2: минимальные данные
        BirthDate bd2(1985, 10, 20);
        Contact contact2("Мария", "Сидорова", "", bd2, "", "maria@yandex.ru");
        contact2.addPhoneNumber("+79219876543");
        manager.addContact(contact2);

        std::cout << "   Создано контактов: " << manager.getContactsCount() << std::endl;

        std::cout << "\n2. ВЫВОД ВСЕХ КОНТАКТОВ:" << std::endl;
        manager.printAllContacts();

        std::cout << "\n3. ПРОВЕРКА ВАЛИДАЦИИ:" << std::endl;
        std::cout << "   Русское имя 'Иван': " << (Contact::validateNameField("Иван") ? "✓" : "✗") << std::endl;
        std::cout << "   Телефон '+79161234567': " << (Contact::isValidPhoneNumber("+79161234567") ? "✓" : "✗") << std::endl;

        std::cout << "\n4. СОХРАНЕНИЕ И ЗАГРУЗКА:" << std::endl;
        ContactManager manager2("test_contacts.txt");
        std::cout << "   Загружено контактов: " << manager2.getContactsCount() << std::endl;

    } catch (const std::exception& e) {
        std::cout << "✗ ОШИБКА: " << e.what() << std::endl;
    }

    std::cout << "\n=== ТЕСТ ЗАВЕРШЕН ===" << std::endl;
}

int main() {
    simpleTest();

    std::cout << "\nНажмите Enter для выхода...";
    std::cin.get();
    return 0;
}