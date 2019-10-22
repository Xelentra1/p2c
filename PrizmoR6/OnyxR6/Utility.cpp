#include "Utility.h"

void Print(const char* text) {
	std::cout << text << "\n";
}

void Print(std::string text) {
	std::cout << text << "\n";
}

void Print(const wchar_t* text) {
	std::cout << text << "\n";
}

void PrintHex(uintptr_t hex) {
	std::cout << std::hex << hex << "\n";
}

std::string MultStr(std::string toMult, int mult) {
	std::string str = "";
	for (int i = 0; i < mult; i++)
	{
		str += toMult;
	}
	return str;
}

std::wstring MultStr(std::wstring toMult, int mult) {
	std::wstring str = L"";
	for (int i = 0; i < mult; i++)
	{
		str += toMult;
	}
	return str;
}

char to_lowercase(char c)
{
	if (c >= 'A' && c <= 'Z')
		return c + 32;

	return c;
}

bool IntVecHas(std::vector<int> vec, int x) {
	for (int i : vec) {
		if (i == x) return true;
	}
	return false;
}

int IntVecLength(std::vector<int> vec) {
	int x = 0;
	for (int i : vec) {
		x++;
	}
	return x;
}

void IntVecRemove(std::vector<int> &vec, int x) {
	for (size_t i = 0; i < IntVecLength(vec); i++)
	{
		if (vec[i] == x) {
			vec.erase(vec.begin() + i);
		}
	}
}