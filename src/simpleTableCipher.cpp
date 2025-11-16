#include "simpleTableCipher.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <locale>
#include <codecvt> // deprecated, но для простоты работы с UTF-8 в большинстве сред OK
using namespace std;

static std::wstring utf8_to_wstring(const std::string& s) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

static std::string wstring_to_utf8(const std::wstring& ws) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(ws);
}

void simpleTableCipher() {
    try {
        int userChoice;
        int rows = 0, cols = 0;
        int inputChoice;
        int outputChoice;
        bool encrypt;

        wcout << L"1. Зашифровать\n2. Дешифровать\nВаш выбор: ";
        if (!(wcin >> userChoice)) throw runtime_error("Некорректный ввод режима.");
        wcin.ignore();
        if (userChoice != 1 && userChoice != 2) throw runtime_error("Неверный выбор режима.");
        encrypt = (userChoice == 1);

        wcout << L"1. Ввод с клавиатуры\n2. Чтение из файла\nВаш выбор: ";
        if (!(wcin >> inputChoice)) throw runtime_error("Некорректный ввод.");
        wcin.ignore();

        wstring text;

        if (inputChoice == 2) {
            wstring wfilename;
            wcout << L"Введите имя файла (UTF-8): ";
            getline(wcin, wfilename);

            string filename(wfilename.begin(), wfilename.end());
            ifstream file(filename, ios::binary);
            if (!file.is_open()) throw runtime_error("Не удалось открыть файл.");

            string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();

            // корректно конвертируем UTF-8 -> wstring
            text = utf8_to_wstring(content);
        } else {
            wcout << L"Введите текст: ";
            getline(wcin, text);
        }

        if (text.empty()) throw runtime_error("Текст пуст.");

        wcout << L"Введите количество строк: ";
        if (!(wcin >> rows) || rows <= 0) throw runtime_error("Некорректное количество строк.");

        wcout << L"Введите количество столбцов: ";
        if (!(wcin >> cols) || cols <= 0) throw runtime_error("Некорректное количество столбцов.");

        wcin.ignore();

        const int size = rows * cols;

        // Если текст длиннее, усекаем (или можно расширить матрицу — по желанию)
        if ((int)text.size() > size) {
            wcout << L"Внимание: длина текста (" << text.size() << L") больше, чем rows*cols (" << size << L"). Текст будет усечён.\n";
            text = text.substr(0, size);
        }

        // Дополняем пробелами если необходимо
        if ((int)text.size() < size) text += wstring(size - text.size(), L' ');

        wstring result;

        if (encrypt) {
            // Записываем по столбцам, читаем по строкам
            vector<vector<wchar_t>> table(rows, vector<wchar_t>(cols, L' '));
            int k = 0;
            for (int col = 0; col < cols; ++col) {
                for (int row = 0; row < rows; ++row) {
                    if (k >= size) break;
                    table[row][col] = text[k++];
                }
            }

            // Чтение по строкам
            wstring cipher;
            cipher.reserve(size);
            for (int row = 0; row < rows; ++row)
                for (int col = 0; col < cols; ++col)
                    cipher += table[row][col];

            result = cipher;
            wcout << L"\nЗашифрованный текст:\n" << cipher << endl;
        } else {
            // Дешифровка: записываем по строкам, читаем по столбцам
            vector<vector<wchar_t>> table(rows, vector<wchar_t>(cols, L' '));
            int k = 0;
            for (int row = 0; row < rows; ++row) {
                for (int col = 0; col < cols; ++col) {
                    if (k >= size) break;
                    table[row][col] = text[k++];
                }
            }

            wstring plain;
            plain.reserve(size);
            for (int col = 0; col < cols; ++col)
                for (int row = 0; row < rows; ++row)
                plain += table[row][col];

            // убрать только конечные пробелы, добавленные в padding
            while (!plain.empty() && plain.back() == L' ')
                plain.pop_back();

            result = plain;
            wcout << L"\nРасшифрованный текст:\n" << plain << endl;
        }

        // Вывод результата
        wcout << L"\n1. Вывод в консоль\n2. Вывод в файл\nВаш выбор: ";
        if (!(wcin >> outputChoice)) throw runtime_error("Некорректный ввод.");
        wcin.ignore();

        if (outputChoice == 2) {
            wstring woutputFilename;
            wcout << L"Введите имя файла для вывода (UTF-8): ";
            getline(wcin, woutputFilename);

            string filename(woutputFilename.begin(), woutputFilename.end());
            ofstream out(filename, ios::binary);
            if (!out.is_open()) throw runtime_error("Не удалось создать файл.");

            string outStr = wstring_to_utf8(result);
            out << outStr;
            out.close();

            wcout << L"Результат записан в файл: " << woutputFilename << endl;
        }
    }
    catch (const exception& e) {
        wcerr << L"Ошибка: " << e.what() << endl;
    }
}