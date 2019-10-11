#pragma once
#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <vector>
#include <io.h>
#include <fcntl.h>
#include <thread>

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
namespace UI {
	void Setup();
	void ConsoleThread();
}