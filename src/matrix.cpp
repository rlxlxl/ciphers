#include "matrix.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;


const int dr[] = {0, 1, 0, -1};
const int dc[] = {1, 0, -1, 0};

vector<vector<wchar_t>> fillMatrix(const wstring& text, int size) {
    vector<vector<wchar_t>> matrix(size, vector<wchar_t>(size, L' '));
    int idx = 0;
    for (int r = 0; r < size; ++r)
        for (int c = 0; c < size; ++c)
            if (idx < (int)text.size())
                matrix[r][c] = text[idx++];
    return matrix;
}

wstring spiralRead(const vector<vector<wchar_t>>& matrix) {
    int n = matrix.size();
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    wstring result;

    int r = 0, c = 0, dir = 0;
    for (int i = 0; i < n * n; ++i) {
        result += matrix[r][c];
        visited[r][c] = true;

        int nr = r + dr[dir];
        int nc = c + dc[dir];
        if (nr < 0 || nr >= n || nc < 0 || nc >= n || visited[nr][nc]) {
            dir = (dir + 1) % 4;
            nr = r + dr[dir];
            nc = c + dc[dir];
        }
        r = nr;
        c = nc;
    }
    return result;
}

vector<vector<wchar_t>> spiralWrite(const wstring& text, int size) {
    vector<vector<wchar_t>> matrix(size, vector<wchar_t>(size, L' '));
    vector<vector<bool>> visited(size, vector<bool>(size, false));

    int r = 0, c = 0, dir = 0;
    int idx = 0;

    for (int i = 0; i < size * size; ++i) {
        if (idx < (int)text.size())
            matrix[r][c] = text[idx++];
        else
            matrix[r][c] = L' ';

        visited[r][c] = true;
        int nr = r + dr[dir];
        int nc = c + dc[dir];
        if (nr < 0 || nr >= size || nc < 0 || nc >= size || visited[nr][nc]) {
            dir = (dir + 1) % 4;
            nr = r + dr[dir];
            nc = c + dc[dir];
        }
        r = nr;
        c = nc;
    }
    return matrix;
}

wstring readMatrix(const vector<vector<wchar_t>>& matrix) {
    wstring result;
    for (const auto& row : matrix)
        for (wchar_t ch : row)
            result += ch;
    return result;
}

void matrix() {
    
    int choice;
    int inputChoice;

    wcout << L"1. Шифровка\n2. Дешифровка\nВыберите: ";
    wcin >> choice;
    wcin.ignore();

    wcout << L"1. Ввод с клавиатуры\n2. Чтение из файла" << endl;
    wcout << L"Выберите способ ввода текста: ";
    wcin >> inputChoice;
    wcin.ignore();

    wstring text;

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

    int size = (int)ceil(sqrt(text.size()));

    if (choice == 1) 
    {
        auto matrix = fillMatrix(text, size);
        wstring encrypted = spiralRead(matrix);
        wcout << L"Зашифрованный текст: " << encrypted << endl;
    }
    else if (choice == 2) 
    {
        auto matrix = spiralWrite(text, size);
        wstring decrypted = readMatrix(matrix);
        while (!decrypted.empty() && decrypted.back() == L' ')
            decrypted.pop_back();
        wcout << L"Расшифрованный текст: " << decrypted << endl;
    }
    else {
        wcout << L"Неверный выбор" << endl;
    }
}