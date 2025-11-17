#include "matrix.hpp"
using namespace std;


const int dr[] = {0, 1, 0, -1};
const int dc[] = {1, 0, -1, 0};


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


void matrix() {
    try {
        int choice = 0, inputChoice = 0, outputChoice = 0;

        auto readInt = [](int& value, const wchar_t* prompt) {
            while (true) {
                wcout << prompt;
                if (wcin >> value) {
                    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                    return;
                }
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                wcerr << L"Некорректный ввод. Повторите.\n";
            }
        };

        readInt(choice, L"1. Шифровка\n2. Дешифровка\nВыберите: ");

        if (choice != 1 && choice != 2)
            throw runtime_error("Неверный выбор режима.");

        readInt(inputChoice, L"1. Ввод с клавиатуры\n2. Чтение из файла\nВыберите: ");

        wstring text;

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
        else if (inputChoice == 1) {
            wcout << L"Введите текст: ";
            getline(wcin, text);
        }
        else {
            throw runtime_error("Неверный выбор способа ввода.");
        }

        if (text.empty()) throw runtime_error("Текст пуст.");

        int n = (int)ceil(sqrt((double)text.size()));

        wstring result;

        if (choice == 1) {
            result = spiralEncrypt(text, n);
        }
        else {
            result = spiralDecrypt(text, n);
        }

        readInt(outputChoice, L"1. Вывод в консоль\n2. Вывод в файл\nВыберите: ");

        if (outputChoice == 1) {
            wcout << L"Результат:\n" << result << endl;
        } 
        else if (outputChoice == 2) {
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
        else {
            throw runtime_error("Неверный выбор способа вывода.");
        }

    } 
    catch (const exception& e) {
        wcerr << L"Ошибка: " << e.what() << endl;
    }
}