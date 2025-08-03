#pragma once
#include <vector>
#include <string>

using namespace std;

vector<vector<wchar_t>> fillMatrix(const wstring& text, int size);
wstring spiralRead(const vector<vector<wchar_t>>& matrix);
vector<vector<wchar_t>> spiralWrite(const wstring& text, int size);
wstring readMatrix(const vector<vector<wchar_t>>& matrix);

void matrix();