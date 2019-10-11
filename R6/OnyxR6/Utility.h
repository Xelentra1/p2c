#pragma once
#include <iostream>
#include <vector>

void Print(const char* text);
void Print(std::string text);
void Print(const wchar_t* text);
void PrintHex(uintptr_t hex);
std::string MultStr(std::string toMult, int mult);
std::wstring MultStr(std::wstring toMult, int mult);
char to_lowercase(char c);
bool IntVecHas(std::vector<int> vec, int x);
void IntVecRemove(std::vector<int>& vec, int x);