#include "simpleTableCipher.hpp"
using namespace std;

void simpleTableCipher() {
    try {
        int mode = 0, inputChoice = 0, outputChoice = 0;

        auto readInt = [](int& value, const wchar_t* prompt) {
            while (true) {
                wcout << prompt;
                if (wcin >> value) {
                    wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');
                    return;
                }
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');
                wcerr << L"Ошибка: введите число.\n";
            }
        };

        readInt(mode, L"1. Шифрование\n2. Дешифрование\nВыберите: ");

        if (mode != 1 && mode != 2)
            throw runtime_error("Неверный выбор режима.");

        readInt(inputChoice, L"1. Ввод вручную\n2. Чтение из файла\nВыберите способ ввода: ");

        wstring text;

        if (inputChoice == 1) {
            wcout << L"Введите текст: ";
            getline(wcin, text);
        }
        else if (inputChoice == 2) {
            wstring wfilename;
            wcout << L"Введите имя файла: ";
            getline(wcin, wfilename);

            string filename(wfilename.begin(), wfilename.end());
            ifstream in(filename);
            if (!in.is_open())
                throw runtime_error("Не удалось открыть файл.");

            string buffer((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
            text = wstring(buffer.begin(), buffer.end());
        }
        else {
            throw runtime_error("Неверный способ ввода.");
        }

        if (text.empty())
            throw runtime_error("Текст пуст.");

        int columns = 0;
        readInt(columns, L"Введите количество столбцов (ключ): ");
        if (columns <= 0)
            throw runtime_error("Количество столбцов должно быть положительным.");

        wstring result;

        if (mode == 1) {
            int rows = (text.size() + columns - 1) / columns;
            vector<wstring> table(rows, wstring(columns, L' '));

            int idx = 0;
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < columns; c++) {
                    if (idx < text.size())
                        table[r][c] = text[idx++];
                }
            }

            for (int c = 0; c < columns; c++)
                for (int r = 0; r < rows; r++)
                    result += table[r][c];

        } else {
            int rows = (text.size() + columns - 1) / columns;
            vector<wstring> table(rows, wstring(columns, L' '));

            int idx = 0;
            for (int c = 0; c < columns; c++)
                for (int r = 0; r < rows; r++)
                    if (idx < text.size())
                        table[r][c] = text[idx++];

            for (int r = 0; r < rows; r++)
                for (int c = 0; c < columns; c++)
                    result += table[r][c];

            while (!result.empty() && result.back() == L' ')
                result.pop_back();
        }

        readInt(outputChoice, L"1. Вывести в консоль\n2. Сохранить в файл\nВыберите: ");

        if (outputChoice == 1) {
            wcout << L"Результат:\n" << result << endl;
        }
        else if (outputChoice == 2) {
            wstring wout;
            wcout << L"Введите имя файла для сохранения: ";
            getline(wcin, wout);

            string filename(wout.begin(), wout.end());
            ofstream out(filename);
            if (!out.is_open())
                throw runtime_error("Не удалось создать файл.");

            string utf8(result.begin(), result.end());
            out << utf8;
            out.close();
            wcout << L"Сохранено: " << wout << endl;
        }
        else {
            throw runtime_error("Неверный выбор вывода.");
        }

    } catch (const exception& e) {
        wcerr << L"Ошибка: " << e.what() << endl;
    }
}