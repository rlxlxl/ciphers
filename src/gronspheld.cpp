#include "gronspheld.hpp"

bool isRussian(const wchar_t symbol)
{
    return (symbol >= L'А' && symbol <= L'Я') || (symbol >= L'а' && symbol <= L'я');
}

bool isEnglish(const wchar_t symbol)
{
    return (symbol >= L'A' && symbol <= L'Z') || (symbol >= L'a' && symbol <= L'z');
}

void gronspheldEncrypt() {
    try {
        wstring text, key;

        int userChoice = 0;
        int inputChoice = 0;
        int outputChoice = 0;

        auto readInt = [](int& value, const wchar_t* prompt) {
            while (true) {
                wcout << prompt;
                if (wcin >> value) { 
                    wcin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return;
                }

                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), '\n');
                wcerr << L"Некорректный ввод, попробуйте ещё раз.\n";
            }
        };

        readInt(userChoice, L"1. Зашифровать\n2. Дешифровать\nВаш выбор: ");
        bool encrypt;
        if (userChoice == 1) encrypt = true;
        else if (userChoice == 2) encrypt = false;
        else throw runtime_error("Неверный выбор режима.");

        readInt(inputChoice, L"1. Ввод с клавиатуры\n2. Чтение из файла\nВаш выбор: ");

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
        } else if (inputChoice == 1) {
            wcout << L"Введите текст: ";
            getline(wcin, text);
        } else {
            throw runtime_error("Неверный выбор способа ввода.");
        }

        if (text.empty()) throw runtime_error("Текст пуст.");

        wcout << L"Введите ключ (цифры): ";
        getline(wcin, key);

        if (key.empty()) throw runtime_error("Ключ пуст.");
        for (wchar_t c : key) {
            if (!iswdigit(c)) throw runtime_error("Ключ должен содержать только цифры.");
        }

        wstring result = L"";
        int keyLen = key.length();

        const int RU = 32;
        const int EN = 26;

        for (size_t i = 0; i < text.length(); ++i)
        {
            wchar_t symbol = text[i];
            int shift = key[i % keyLen] - L'0';

            if (iswspace(symbol)) {
                result += symbol;
            }
            else if (isRussian(symbol)) {
                wchar_t base = iswupper(symbol) ? L'А' : L'а';
                if (encrypt)
                    result += (wchar_t)(base + (symbol - base + shift) % RU);
                else
                    result += (wchar_t)(base + (symbol - base - shift + RU) % RU);
            }
            else if (isEnglish(symbol)) {
                wchar_t base = iswupper(symbol) ? L'A' : L'a';
                if (encrypt)
                    result += (wchar_t)(base + (symbol - base + shift) % EN);
                else
                    result += (wchar_t)(base + (symbol - base - shift + EN) % EN);
            }
            else if (iswdigit(symbol)) {
                int base = L'0';
                if (encrypt)
                    result += (wchar_t)(base + (symbol - base + shift) % 10);
                else
                    result += (wchar_t)(base + (symbol - base - shift + 10) % 10);
            }
            else {
                result += symbol;
            }
        }

        readInt(outputChoice, L"1. Вывод в консоль\n2. Вывод в файл\nВаш выбор: ");

        if (outputChoice == 2) {
            wstring woutputFilename;
            wcout << L"Введите имя файла: ";
            getline(wcin, woutputFilename);

            string outputFilename(woutputFilename.begin(), woutputFilename.end());
            ofstream out(outputFilename);
            if (!out.is_open()) throw runtime_error("Не удалось создать файл.");
            string res(result.begin(), result.end());
            out << res;
            out.close();

            wcout << L"Результат записан в файл: " << woutputFilename << endl;
        } else if (outputChoice == 1) {
            wcout << L"Результат: " << result << endl;
        } else {
            throw runtime_error("Неверный выбор способа вывода.");
        }

    } catch (const exception& e) {
        wcerr << L"Ошибка: " << e.what() << endl;
    } catch (...) {
        wcerr << L"Неизвестная ошибка." << endl;
    }
}