#pragma once

#include <cctype>
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;


bool isRussian(const wchar_t symbol);
bool isEnglish(const wchar_t symbol);
void gronspheldEncrypt();
