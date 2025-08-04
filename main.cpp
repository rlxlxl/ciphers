#include <iostream>
#include <limits>
#include <locale>
#include "gronspheld.hpp"
#include "matrix.hpp"
#include "simpleTableCipher.hpp"

using namespace std;

int main() {

    locale::global(locale(""));
    wcin.imbue(locale());
    wcout.imbue(locale());
    
    int choice;

    while (true) {
        wcout << L"Выберите способ шифрования:\n";
        wcout << L"1 - Матричный шифр\n";
        wcout << L"2 - Шифр Гронсфельда\n";
        wcout << L"3 - Простая табличная перестановка\n";
        wcout << L"0 - Выход\n";
        wcout << L"Ваш выбор: ";

        wcin >> choice;
        wcin.ignore();

        switch (choice) {
            case 1:
                matrix();
                break;
            case 2:
                gronspheldEncrypt();
                break;
            case 3:
                simpleTableCipher();
                break;
            case 0:
                wcout << L"Выход из программы." << endl; 
                return 0;
            default:
                wcout << L"Неверный выбор, попробуйте ещё раз." << endl;
        }
    }
}