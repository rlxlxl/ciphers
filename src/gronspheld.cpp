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
        wstring text;
        wstring key;
        bool encrypt;
        int userChoice;
        int inputChoice;
        int outputChoice;
        
        wcout << L"1. Зашифровать\n2. Дешифровать" << endl;
        wcout << L"Ваш выбор: " ;
        if (!(wcin >> userChoice)) throw runtime_error("Некорректный ввод режима.");
        wcin.ignore();

        switch(userChoice)
        {
            case 1: encrypt = true; break;
            case 2: encrypt = false; break;
            default:
                throw runtime_error("Неверный выбор.");
        }

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

        wcout << L"Введите ключ (цифры): ";
        getline(wcin, key);

        if (key.empty()) throw runtime_error("Ключ пуст.");
        for (wchar_t c : key) {
            if (!iswdigit(c)) throw runtime_error("Ключ должен содержать только цифры.");
        }

        wstring result = L"";
        int keyLen = key.length();

        const int RU_ALPHABET_SIZE = 32;
        const int EN_ALPHABET_SIZE = 26;

        for (size_t i = 0; i < text.length(); ++i)
        {
            wchar_t symbol = text[i];
            int shift = key[i % keyLen] - L'0';

            if (iswspace(symbol))
            {
                result += symbol;
            }
            else if (isRussian(symbol))
            {
                wchar_t base = iswupper(symbol) ? L'А' : L'а';
                if (encrypt)
                    result += static_cast<wchar_t>(base + (symbol - base + shift) % RU_ALPHABET_SIZE);
                else
                    result += static_cast<wchar_t>(base + (symbol - base - shift + RU_ALPHABET_SIZE) % RU_ALPHABET_SIZE);
            }
            else if (isEnglish(symbol))
            {
                wchar_t base = iswupper(symbol) ? L'A' : L'a';
                if (encrypt)
                    result += static_cast<wchar_t>(base + (symbol - base + shift) % EN_ALPHABET_SIZE);
                else
                    result += static_cast<wchar_t>(base + (symbol - base - shift + EN_ALPHABET_SIZE) % EN_ALPHABET_SIZE);
            }
            else 
            {
                result += symbol; // не буква — оставляем как есть
            }
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
        } else {
            wcout << L"Результат: " << result << endl;
        }

    } catch (const exception& e) {
        wcerr << L"Ошибка: " << e.what() << endl;
    } catch (...) {
        wcerr << L"Неизвестная ошибка." << endl;
    }
}