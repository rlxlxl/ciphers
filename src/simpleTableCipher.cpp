#include "simpleTableCipher.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

wstring removeSpaces(const wstring& text) {
    wstring result;
    for (wchar_t ch : text)
        if (!iswspace(ch))
            result += ch;
    return result;
}

void simpleTableCipher() {
    int userChoice;
    bool encrypt;
    int rows, cols;
    int inputChoice;

    wcout << L"1. Зашифровать\n2. Дешифровать" << endl;
    wcout << L"Ваш выбор: " ;
    wcin >> userChoice;
    wcin.ignore();

    switch (userChoice) {
        case 1: encrypt = true; break;
        case 2: encrypt = false; break;
        default:
            wcout << L"Ошибка: неверный выбор." << endl;
            return;
    }

    wstring text;
    
    wcout << L"1. Ввод с клавиатуры\n2. Чтение из файла" << endl;
    wcout << L"Выберите способ ввода текста: ";
    wcin >> inputChoice;
    wcin.ignore();

    if (inputChoice == 2) {
        string filename;
        wcout << L"Введите имя файла: ";
        getline(wcin, text);  // Временно используем text для ввода имени файла
        filename = string(text.begin(), text.end());  // Преобразуем wstring в string
        text.clear();  // Очищаем text для дальнейшего использования
        
        ifstream file(filename);
        if (!file.is_open()) {
            wcout << L"Ошибка: не удалось открыть файл." << endl;
            return;
        }
        
        // Читаем содержимое файла
        string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
        
        // Преобразуем в wstring
        text = wstring(fileContent.begin(), fileContent.end());
    } else {
        wcout << L"Введите текст: ";
        getline(wcin, text);
    }

    if (text.empty()) {
        wcout << L"Ошибка: вы ничего не ввели." << endl;
        return;
    }

    wcout << L"Введите количество строк: ";
    wcin >> rows;

    wcout << L"Введите количество столбцов: ";
    wcin >> cols;
    wcin.ignore(); 
    int size = rows * cols;

    if (encrypt) 
    {
        wstring clean = removeSpaces(text);

        if (clean.size() < static_cast<size_t>(size)) 
        {
            clean += wstring(size - clean.size(), L' ');
        }

        vector<vector<wchar_t>> table(rows, vector<wchar_t>(cols));
        size_t k = 0;
        for (int col = 0; col < cols; ++col)
        {
            for (int row = 0; row < rows; ++row)
            {
                table[row][col] = clean[k++];
            }
        }
            
        wstring cipherText;
        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                cipherText += table[row][col];
            }
        }
        wcout << L"Результат шифровки: " << cipherText << endl;
    } 
    else
    {
        wstring cipherText = removeSpaces(text); 

        if (cipherText.size() < static_cast<size_t>(size))
        {
            cipherText += wstring(size - cipherText.size(), L' ');
        }
        
        vector<vector<wchar_t>> table(rows, vector<wchar_t>(cols));
        size_t k = 0;
        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                table[row][col] = cipherText[k++];
            }
        }
        
        wstring decipherText;
        for (int col = 0; col < cols; ++col)
        {
            for (int row = 0; row < rows; ++row)
            {
                 decipherText += table[row][col];
            }
        }
        wcout << L"Результат дешифровки: " << decipherText << endl;
    }
}