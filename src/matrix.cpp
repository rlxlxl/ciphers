#include "matrix.hpp"


const int dr[] = {0, 1, 0, -1};
const int dc[] = {1, 0, -1, 0};

vector<vector<wchar_t>> fillMatrix(const wstring& text, int size) {
    vector<vector<wchar_t>> matrix(size, vector<wchar_t>(size, L' '));
    int idx = 0;
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            if (idx < (int)text.size())
                matrix[r][c] = text[idx++];
        }
    }
    return matrix;
}

wstring spiralRead(const vector<vector<wchar_t>>& matrix) {
    int n = matrix.size();
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    wstring result;

    int r = n / 2, c = n / 2; // начинаем с центра
    int dir = 0;
    int steps = 1;
    int stepCount = 0;
    int turnCount = 0;

    result += matrix[r][c];
    visited[r][c] = true;

    for (int i = 1; i < n * n; ++i) {
        r += dr[dir];
        c += dc[dir];

        result += matrix[r][c];
        visited[r][c] = true;

        stepCount++;
        if (stepCount == steps) {
            dir = (dir + 1) % 4;
            stepCount = 0;
            turnCount++;
            if (turnCount % 2 == 0)
                steps++;
        }
    }
    return result;
}

vector<vector<wchar_t>> spiralWrite(const wstring& text, int size) {
    vector<vector<wchar_t>> matrix(size, vector<wchar_t>(size, L' '));
    vector<vector<bool>> visited(size, vector<bool>(size, false));

    int r = size / 2, c = size / 2;
    int dir = 0;
    int steps = 1;
    int stepCount = 0;
    int turnCount = 0;
    int idx = 0;

    matrix[r][c] = (idx < (int)text.size() ? text[idx++] : L' ');
    visited[r][c] = true;

    for (int i = 1; i < size * size; ++i) {
        r += dr[dir];
        c += dc[dir];
        matrix[r][c] = (idx < (int)text.size() ? text[idx++] : L' ');
        visited[r][c] = true;

        stepCount++;
        if (stepCount == steps) {
            dir = (dir + 1) % 4;
            stepCount = 0;
            turnCount++;
            if (turnCount % 2 == 0)
                steps++;
        }
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
    try {
        int choice;
        int inputChoice;
        int outputChoice;

        wcout << L"1. Шифровка\n2. Дешифровка\nВыберите: ";
        if (!(wcin >> choice)) throw runtime_error("Некорректный ввод.");
        wcin.ignore();

        wcout << L"1. Ввод с клавиатуры\n2. Чтение из файла" << endl;
        wcout << L"Выберите способ ввода текста: ";
        if (!(wcin >> inputChoice)) throw runtime_error("Некорректный ввод.");
        wcin.ignore();

        wstring text;

        if (inputChoice == 2) {
            wstring wfilename;
            wcout << L"Введите имя файла: ";
            getline(wcin, wfilename);

            string filename(wfilename.begin(), wfilename.end());
            ifstream file(filename);
            if (!file.is_open()) throw runtime_error("Не удалось открыть файл.");

            string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();
            text = wstring(fileContent.begin(), fileContent.end());
        } else {
            wcout << L"Введите текст: ";
            getline(wcin, text);
        }

        int size = (int)ceil(sqrt(text.size()));
        wstring result;

        if (choice == 1) {
            auto m = fillMatrix(text, size);
            wstring encrypted = spiralRead(m);
            result = encrypted;
            wcout << L"Зашифрованный текст: " << encrypted << endl;
        } else if (choice == 2) {
            auto m = spiralWrite(text, size);
            wstring decrypted = readMatrix(m);
            while (!decrypted.empty() && decrypted.back() == L' ')
                decrypted.pop_back();
            result = decrypted;
            wcout << L"Расшифрованный текст: " << decrypted << endl;
        } else {
            throw runtime_error("Неверный выбор режима.");
        }

        wcout << L"1. Вывод в консоль\n2. Вывод в файл" << endl;
        wcout << L"Выберите способ вывода результата: ";
        if (!(wcin >> outputChoice)) throw runtime_error("Некорректный ввод.");
        wcin.ignore();

        if (outputChoice == 2) {
            wstring woutputFilename;
            wcout << L"Введите имя файла для вывода: ";
            getline(wcin, woutputFilename);

            string outputFilename(woutputFilename.begin(), woutputFilename.end());
            ofstream outputFile(outputFilename);
            if (!outputFile.is_open()) throw runtime_error("Не удалось создать файл для вывода.");

            string resultString(result.begin(), result.end());
            outputFile << resultString;
            outputFile.close();

            wcout << L"Результат успешно записан в файл: " << woutputFilename << endl;
        }

    } catch (const exception& e) {
        wcerr << L"Ошибка: " << e.what() << endl;
    }
}