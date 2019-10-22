#pragma once
#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <vector>
#include <io.h>
#include <fcntl.h>
#include <thread>

#define COLOR_MAIN 1
#define COLOR_ENABLED 6
#define COLOR_DISABLED 15

inline HANDLE hConsole = NULL;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
namespace UI {
	void Setup();
	void ConsoleThread();
}

inline void SetChar(TCHAR cCharacter, COORD dwWriteCoord, DWORD nLength) {
	DWORD count;
	FillConsoleOutputCharacterA(
		hConsole,
		cCharacter,
		nLength,
		dwWriteCoord,
		&count
	);
}

inline void SetCharW(WCHAR cCharacter, COORD dwWriteCoord, DWORD nLength) {
	DWORD count;
	FillConsoleOutputCharacterW(
		hConsole,
		cCharacter,
		nLength,
		dwWriteCoord,
		&count
	);
}

inline void SetColor(int colorValue, COORD dwWriteCoord, DWORD nLength) {
	DWORD count;
	FillConsoleOutputAttribute(
		hConsole,
		colorValue,
		nLength,
		dwWriteCoord,
		&count
	);
}