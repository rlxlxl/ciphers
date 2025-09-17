#include "simpleTableCipher.hpp"


wstring removeSpaces(const wstring& text) {
    wstring result;
    for (wchar_t ch : text)
        if (!iswspace(ch))
            result += ch;
    return result;
}

void simpleTableCipher() {
    try {
        int userChoice;
        bool encrypt;
        int rows, cols;
        int inputChoice;
        int outputChoice;

        wcout << L"1. Зашифровать\n2. Дешифровать" << endl;
        wcout << L"Ваш выбор: ";
        if (!(wcin >> userChoice)) throw runtime_error("Некорректный ввод режима.");
        wcin.ignore();

        switch (userChoice) {
            case 1: encrypt = true; break;
            case 2: encrypt = false; break;
            default: throw runtime_error("Неверный выбор режима.");
        }

        wstring text;
        wcout << L"1. Ввод с клавиатуры\n2. Чтение из файла" << endl;
        wcout << L"Выберите способ ввода текста: ";
        if (!(wcin >> inputChoice)) throw runtime_error("Некорректный ввод.");
        wcin.ignore();

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

        if (text.empty()) throw runtime_error("Текст пуст.");

        wcout << L"Введите количество строк: ";
        if (!(wcin >> rows) || rows <= 0) throw runtime_error("Некорректное количество строк.");
        wcout << L"Введите количество столбцов: ";
        if (!(wcin >> cols) || cols <= 0) throw runtime_error("Некорректное количество столбцов.");
        wcin.ignore();

        int size = rows * cols;
        wstring result;

        if (encrypt) {
            wstring clean = removeSpaces(text);
            if (clean.size() < static_cast<size_t>(size)) {
                clean += wstring(size - clean.size(), L' ');
            }

            vector<vector<wchar_t>> table(rows, vector<wchar_t>(cols));
            size_t k = 0;
            for (int col = 0; col < cols; ++col)
                for (int row = 0; row < rows; ++row)
                    table[row][col] = clean[k++];

            wstring cipherText;
            for (int row = 0; row < rows; ++row)
                for (int col = 0; col < cols; ++col)
                    cipherText += table[row][col];

            result = cipherText;
            wcout << L"Результат шифровки: " << cipherText << endl;
        } else {
            wstring cipherText = removeSpaces(text);
            if (cipherText.size() < static_cast<size_t>(size)) {
                cipherText += wstring(size - cipherText.size(), L' ');
            }

            vector<vector<wchar_t>> table(rows, vector<wchar_t>(cols));
            size_t k = 0;
            for (int row = 0; row < rows; ++row)
                for (int col = 0; col < cols; ++col)
                    table[row][col] = cipherText[k++];

            wstring decipherText;
            for (int col = 0; col < cols; ++col)
                for (int row = 0; row < rows; ++row)
                    decipherText += table[row][col];

            result = decipherText;
            wcout << L"Результат дешифровки: " << decipherText << endl;
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
    } catch (...) {
        wcerr << L"Неизвестная ошибка." << endl;
    }
}