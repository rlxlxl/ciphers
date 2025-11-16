#include "matrix.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
using namespace std;

// Направления (вправо, вниз, влево, вверх)
const int dr[] = {0, 1, 0, -1};
const int dc[] = {1, 0, -1, 0};

//
// Генерация порядка обхода спирали
//
vector<pair<int,int>> buildSpiralOrder(int n) {
    vector<pair<int,int>> order;
    order.reserve(n * n);

    vector<vector<bool>> visited(n, vector<bool>(n, false));

    int r = n / 2;
    int c = n / 2;

    int dir = 0;
    int steps = 1;
    int stepCount = 0;
    int turnCount = 0;

    order.push_back({r, c});
    visited[r][c] = true;

    for (int i = 1; i < n * n; i++) {
        r += dr[dir];
        c += dc[dir];

        order.push_back({r, c});
        visited[r][c] = true;

        stepCount++;

        if (stepCount == steps) {
            dir = (dir + 1) % 4;
            stepCount = 0;
            turnCount++;
            if (turnCount % 2 == 0) steps++;
        }
    }

    return order;
}

//
// Шифрование — чтение исходного текста по спирали
//
wstring spiralEncrypt(const wstring& text, int n) {
    auto order = buildSpiralOrder(n);

    wstring encrypted;
    encrypted.reserve(n * n);

    for (auto [r, c] : order) {
        int idx = r * n + c;
        encrypted += (idx < (int)text.size() ? text[idx] : L' ');
    }

    return encrypted;
}

//
// Дешифровка — раскладывание текста по спирали обратно
//
wstring spiralDecrypt(const wstring& encrypted, int n) {
    auto order = buildSpiralOrder(n);

    vector<wchar_t> flat(n * n, L' ');

    for (int i = 0; i < n * n; i++) {
        auto [r, c] = order[i];
        flat[r * n + c] = encrypted[i];
    }

    wstring result(flat.begin(), flat.end());

    while (!result.empty() && result.back() == L' ')
        result.pop_back();

    return result;
}

//
// Главная функция
//
void matrix() {
    try {
        int choice, inputChoice, outputChoice;

        wcout << L"1. Шифровка\n2. Дешифровка\nВыберите: ";
        if (!(wcin >> choice)) throw runtime_error("Некорректный ввод.");
        wcin.ignore();

        wcout << L"1. Ввод с клавиатуры\n2. Чтение из файла\nВыберите: ";
        if (!(wcin >> inputChoice)) throw runtime_error("Некорректный ввод.");
        wcin.ignore();

        wstring text;

        // Ввод текста
        if (inputChoice == 2) {
            wstring wfilename;
            wcout << L"Введите имя файла: ";
            getline(wcin, wfilename);

            string filename(wfilename.begin(), wfilename.end());
            ifstream file(filename);
            if (!file.is_open()) throw runtime_error("Не удалось открыть файл.");

            string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();

            text = wstring(content.begin(), content.end());
        } 
        else {
            wcout << L"Введите текст: ";
            getline(wcin, text);
        }

        if (text.empty()) throw runtime_error("Текст пуст.");

        // Размер матрицы
        int n = (int)ceil(sqrt(text.size()));

        wstring result;

        if (choice == 1) {
            result = spiralEncrypt(text, n);
            wcout << L"Зашифрованный текст:\n" << result << endl;
        } 
        else if (choice == 2) {
            result = spiralDecrypt(text, n);
            wcout << L"Расшифрованный текст:\n" << result << endl;
        } 
        else {
            throw runtime_error("Неверный выбор.");
        }

        // Вывод результата
        wcout << L"1. Вывод в консоль\n2. Вывод в файл\nВыберите: ";
        if (!(wcin >> outputChoice)) throw runtime_error("Некорректный ввод.");
        wcin.ignore();

        if (outputChoice == 2) {
            wstring woutputFilename;
            wcout << L"Введите имя файла для вывода: ";
            getline(wcin, woutputFilename);

            string filename(woutputFilename.begin(), woutputFilename.end());
            ofstream out(filename);
            if (!out.is_open()) throw runtime_error("Не удалось создать файл.");

            string outStr(result.begin(), result.end());
            out << outStr;
            out.close();

            wcout << L"Результат сохранён в файл: " << woutputFilename << endl;
        }

    } 
    catch (const exception& e) {
        wcerr << L"Ошибка: " << e.what() << endl;
    }
}