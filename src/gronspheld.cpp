#include "gronspheld.hpp"
#include <cctype>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

bool isRussian(const wchar_t symbol)
{
    return (symbol >= L'А' && symbol <= L'Я') || (symbol >= L'а' && symbol <= L'я');
}

bool isEnglish(const wchar_t symbol)
{
    return (symbol >= L'A' && symbol <= L'Z') || (symbol >= L'a' && symbol <= L'z');
}


void gronspheldEncrypt() {

    wstring text;
    wstring key;
    bool encrypt;
    bool readFromFile;
    bool writeToFile;
    int userChoice;
    int inputChoice;
    
    wcout << L"1. Зашифровать\n2. Дешифровать" << endl;
    wcout << L"Ваш выбор: " ;
    wcin >> userChoice;
    wcin.ignore();

    switch(userChoice)
    {
        case 1: encrypt = true; break;
        case 2: encrypt = false; break;
        default:
            wcout << L"Неверный выбор" << endl;
            return;
    }

    wcout << L"1. Ввод с клавиатуры\n2. Чтение из файла" << endl;
    wcout << L"Выберите способ ввода текста: ";
    wcin >> inputChoice;
    wcin.ignore();

    if (inputChoice == 2) {
        string filename;  
        wcout << L"Введите имя файла: ";
        getline(wcin, text);  
        filename = string(text.begin(), text.end());  
        text.clear();  
        
        ifstream file(filename);  
        if (!file.is_open()) {
            wcout << L"Ошибка: не удалось открыть файл." << endl;
            return;
        }
        
       
        string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
        
        
        text = wstring(fileContent.begin(), fileContent.end());
    } else {
        wcout << L"Введите текст: ";
        getline(wcin, text);
    }

    if (text.empty()) {
        wcout << L"Ошибка: текст пуст." << endl;
        return;
    }

    wcout << L"Введите ключ: ";
    getline(wcin, key);

    if (key.empty()) {
        wcout << L"Ошибка: ключ не должен быть пустым." << endl;
        return;
    }

    for (char c : key) {
        if (!isdigit(c)) {
            wcout << L"Ошибка: ключ должен содержать только цифры." << endl;
            return;
        }
    }

    wstring result = L"";
    int keyLen = key.length();

    const int RU_ALPHABET_SIZE = 32;
    const int EN_ALPHABET_SIZE = 26;

    for (size_t i = 0; i < text.length(); ++i)
    {
        wchar_t symbol = text[i];
        int shift = key[i % keyLen] - '0';

        if (isspace(symbol))
        {
            result += symbol;
        }
        else if (isRussian(symbol))
        {
            wchar_t base = iswupper(symbol) ? L'А' : L'а';

            if (encrypt)
            {
                result += static_cast<wchar_t>(base + (symbol - base + shift) % RU_ALPHABET_SIZE);
            }
            else
            {
                result += static_cast<wchar_t>(base + (symbol - base - shift + RU_ALPHABET_SIZE) % RU_ALPHABET_SIZE);
            }
        }
        else if (isEnglish(symbol))
        {
             wchar_t base = iswupper(symbol) ? L'A' : L'a';

            if (encrypt)
            {
                result += static_cast<wchar_t>(base + (symbol - base + shift) % EN_ALPHABET_SIZE);
            }
            else
            {
                result += static_cast<wchar_t>(base + (symbol - base - shift + EN_ALPHABET_SIZE) % EN_ALPHABET_SIZE);
            }
        }
        else 
        {
            result += symbol; // Оставляем символ без изменений
        }
    }

    wcout << L"Результат: " << result << endl;
}