#include "Contact.h"
#include <regex>
#include <algorithm>
#include <locale>

//валидация данных
bool Contact::startsWithLetter(const std::string& name) {
    if (name.empty()) return false;

    unsigned char firstChar = name[0];

    // Проверяем что не начинается на пробел или дефис
    if (firstChar == ' ' || firstChar == '-') {
        return false;
    }

    // Латинские буквы
    if ((firstChar >= 'A' && firstChar <= 'Z') ||
        (firstChar >= 'a' && firstChar <= 'z')) {
        return true;
        }

    // Русские буквы (проверяем первый байт UTF-8)
    if (firstChar >= 0xD0 && firstChar <= 0xDF) {
        return true;
    }

    return false;
}

bool Contact::noHyphenAtEdges(const std::string& name) {
    if (name.empty()) return true;
    return name.front() != '-' && name.back() != '-'; //проверка, что первый и последний символ не дефис
}

bool Contact::containsOnlyAllowedChars(const std::string& name) {
    if (name.empty()) return false;

    for (size_t i = 0; i < name.length(); i++) {
        unsigned char c = name[i];

        //проверяем однобайтовые символы
        if ((c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') ||
            (c == ' ') ||
            (c == '-') ||
            (c == '\'')) {
            continue;
            }

        //проверяем русские буквы (кириллица в UTF-8)
        if (c >= 0xD0 && c <= 0xDF) {
            //первый байт кириллического символа в UTF-8
            if (i + 1 < name.length()) {
                unsigned char next = name[i + 1];
                //второй байт должен быть в определенном диапазоне
                if (next >= 0x80 && next <= 0xBF) {
                    i++; //пропускаем следующий байт, так как мы его уже обработали
                    continue;
                }
            }
            return false;
        }

        return false;
    }
    return true;
}

std::string Contact::normalizeNameField(const std::string& name) {
    if (name.empty()) { //если полностью пустая строка
        return "";
    }

    //удаляем пробелы в начале и в конце
    //" \t\n\r\f\v" - все виды пробельных символов, а find_first_not_of - ищет первый не пробельный символ
    size_t start = name.find_first_not_of(" \t\n\r\f\v");
    size_t end = name.find_last_not_of(" \t\n\r\f\v"); //ищет последний не пробельный символ

    if (start == std::string::npos) { //find_first_not_of() возвращает std::string::npos или -1, если не находит
        return "";
    }
    std::string result = name.substr(start, end - start + 1); //вырезаем часть без пробелов справа и слева

    //заменяем множественные пробелы на один
    std::regex spaceRegex("\\s+"); //  \\s+ - регулярное выражение: "один или больше пробельных символов"
    result = std::regex_replace(result, spaceRegex, " ");

    return result;
}

bool Contact::validateNameField(const std::string &name) {
    if (name.empty()) {
        return false;
    }

    std::string normalized = normalizeNameField(name);

    if (normalized.empty()) {
        return false;
    }

    //проверка всех условий
    return containsOnlyAllowedChars(normalized) &&  //только разрешенные символы
           startsWithLetter(normalized) &&          //начинается с буквы
           noHyphenAtEdges(normalized);             //не начинается и не заканчивается дефисом
}

bool Contact::validateNames() const {
    return validateNameField(firstName) &&
           validateNameField(lastName) &&
           (middleName.empty() || validateNameField(middleName));
}

std::string Contact::normalizeEmail(const std::string& email) {
    if (email.empty()) return "";

    //удаление всех пробелы
    std::string noSpaces;
    for (char c : email) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            noSpaces += c;
        }
    }

    if (noSpaces.empty()) return "";

    //приведение к нижнему регистру
    std::string result;
    std::ranges::transform(noSpaces, std::back_inserter(result), ::tolower);
    //::tolower - глобальная функция из <cctype>, преобразует символ в нижний регистр

    //удаление множественных @ (оставляем только первую)
    size_t firstAt = result.find('@');
    if (firstAt != std::string::npos) {
        size_t secondAt = result.find('@', firstAt + 1);
        if (secondAt != std::string::npos) {
            //удаляем все @ после первой
            std::string cleanResult;
            bool atFound = false;
            for (char c : result) {
                if (c == '@') {
                    if (!atFound) {
                        cleanResult += c;
                        atFound = true;
                    }
                    //пропускаем последующие @
                } else {
                    cleanResult += c;
                }
            }
            result = cleanResult;
        }
    }

    return result;
}

bool Contact::validateEmail(const std::string& email) {
    if (email.empty()) return true;

    std::string normalized = normalizeEmail(email);
    if (normalized.empty()) return true;

    size_t atPos = normalized.find('@');

    //все проверки
    if (atPos == std::string::npos ||      //нет @
        atPos == 0 ||                      //@ в начале
        atPos == normalized.length() - 1 || //@ в конце
        std::ranges::count(normalized, '@') != 1) { // Не ровно одна @
        return false;
        }

    //проверяем домен
    std::string domain = normalized.substr(atPos + 1);
    size_t dotPos = domain.find('.');

    return !(dotPos == std::string::npos || dotPos == 0 || dotPos == domain.length() - 1);
}

bool Contact::validatePhoneNumbers() const {
    //регулярное выражение для конкретных форматов
    std::regex phonePattern(
        R"(^((\+7|8)\d{10}$|(\+7|8)\(\d{3}\)\d{7}$|(\+7|8)\(\d{3}\)\d{3}-\d{2}-\d{2}$))"
    );

    for (const auto& phone : phoneNumbers) {
        //удаляем пробелы в начале и конце
        std::string trimmedPhone = phone;
        trimmedPhone.erase(0, trimmedPhone.find_first_not_of(" \t\n\r\f\v"));
        trimmedPhone.erase(trimmedPhone.find_last_not_of(" \t\n\r\f\v") + 1);

        if (trimmedPhone.empty()) {
            continue; //пустые номера пропускаем
        }

        if (!std::regex_match(trimmedPhone, phonePattern)) {
            return false;
        }
    }
    return true;
}
bool Contact::isValidPhoneNumber(const std::string& phone) {
    if (phone.empty()) return false;

    std::string trimmedPhone = phone;
    trimmedPhone.erase(0, trimmedPhone.find_first_not_of(" \t\n\r\f\v"));
    trimmedPhone.erase(trimmedPhone.find_last_not_of(" \t\n\r\f\v") + 1);

    if (trimmedPhone.empty()) return false;

    std::regex phonePattern(
        R"(^((\+7|8)\d{10}$|(\+7|8)\(\d{3}\)\d{7}$|(\+7|8)\(\d{3}\)\d{3}-\d{2}-\d{2}$))"
    );

    return std::regex_match(trimmedPhone, phonePattern);
}

//конструкторы
Contact::Contact(std::string _firstName, std::string _lastName, std::string _middleName,
    const BirthDate& _birthDate, std::string _address, std::string _email):
    firstName(std::move(_firstName)), lastName(std::move(_lastName)),
    middleName(std::move(_middleName)), birthDate(_birthDate),
    address(std::move(_address)), email(std::move(_email)), phoneNumbers() {}

Contact::Contact(const Contact &contact) :
    firstName(contact.firstName), lastName(contact.lastName),
    middleName(contact.middleName), birthDate(contact.birthDate),
    address(contact.address), email(contact.email), phoneNumbers(contact.phoneNumbers) {}

//getters

std::string Contact::getFirstName() const {
    return firstName;
}

std::string Contact::getLastName() const {
    return lastName;
}

std::string Contact::getMiddleName() const {
    return middleName;
}

std::string Contact::getBirthDate() const {
    return birthDate.getDate();
}

std::string Contact::getAddress() const {
    return address;
}

std::string Contact::getEmail() const {
    return email;
}

std::vector<std::string> Contact::getPhoneNumbers() const {
    return phoneNumbers;
}

//МЕТОДЫ ДЛЯ РАБОТЫ С ТЕЛЕФОНАМИ

void Contact::addPhoneNumber(const std::string& phone) {
    if (!isValidPhoneNumber(phone)) {
        throw std::invalid_argument("Некорректный формат телефонного номера: " + phone);
    }
    phoneNumbers.push_back(phone);
}

void Contact::addPhoneNumbers(const std::vector<std::string>& phones) {
    //сначала проверяем все номера
    for (const auto& phone : phones) {
        if (!isValidPhoneNumber(phone)) {
            throw std::invalid_argument("Некорректный формат телефонного номера: " + phone);
        }
    }
    //если все валидны - добавляем
    phoneNumbers.insert(phoneNumbers.end(), phones.begin(), phones.end());
}

void Contact::removePhoneNumber(size_t index) {
    if (index < phoneNumbers.size()) {
        phoneNumbers.erase(phoneNumbers.begin() + static_cast<std::vector<std::string>::difference_type>(index));
    }
    else {
         throw std::out_of_range("Несуществующий индекс номера телефона.");
    }
}

void Contact::clearPhoneNumbers() {
    phoneNumbers.clear();
}

size_t Contact::getPhoneNumbersCount() const {
    return phoneNumbers.size();
}

bool Contact::hasPhoneNumbers() const {
    return !phoneNumbers.empty();
}

const std::string& Contact::getPhoneNumber(size_t index) const {
    if (index >= phoneNumbers.size()) {
        throw std::out_of_range("Несуществующий индекс номера телефона, введите число меньше.");
    }
    return phoneNumbers[index];
}

//SETTERS

void Contact::setFirstName(const std::string& _firstName) {
    std::string normalized = normalizeNameField(_firstName);
    if (!validateNameField(normalized)) {
        throw std::invalid_argument("Неверное имя: " + _firstName);
    }
    firstName = std::move(normalized);
}

void Contact::setLastName(const std::string& _lastName) {
    std::string normalized = normalizeNameField(_lastName);
    if (!validateNameField(normalized)) {
        throw std::invalid_argument("Неверная фамилия: " + _lastName);
    }
    lastName = std::move(normalized);
}

void Contact::setMiddleName(const std::string& _middleName) {
    //отчество может быть пустым
    if (_middleName.empty()) {
        middleName = "";
        return;
    }

    std::string normalized = normalizeNameField(_middleName);
    if (!validateNameField(normalized)) {
        throw std::invalid_argument("Неверное отчество: " + _middleName);
    }
    middleName = std::move(normalized);
}

void Contact::setBirthDate(const BirthDate& _birthDate) {
    //BirthDate уже должна быть валидной
    birthDate = _birthDate;
}

void Contact::setAddress(const std::string& _address) {
    if (_address.empty()) {
        address = "";
        return;
    }

    //удаляем пробелы в начале и конце
    size_t start = _address.find_first_not_of(" \t\n\r\f\v");
    size_t end = _address.find_last_not_of(" \t\n\r\f\v");

    if (start == std::string::npos) {
        address = "";
    } else {
        address = _address.substr(start, end - start + 1);
    }
}

void Contact::setEmail(const std::string& _email) {
    //Email может быть пустым
    if (_email.empty()) {
        email = "";
        return;
    }

    std::string normalized = normalizeEmail(_email);
    if (!validateEmail(normalized)) {
        throw std::invalid_argument("Неверный email: " + _email);
    }
    email = std::move(normalized);
}

void Contact::setPhoneNumbers(const std::vector<std::string>& _phoneNumbers) {
    // Проверяем каждый телефон перед установкой
    for (const auto& phone : _phoneNumbers) {
        if (!isValidPhoneNumber(phone)) {
            throw std::invalid_argument("Неверный формат телефона: " + phone);
        }
    }
    phoneNumbers = _phoneNumbers;
}