#include "ConsoleUI.h"
#include "Utility.h"
#include "Shared.h"
#include "ESP.h"

#define COLOR_MAIN 1
#define COLOR_ENABLED 6
#define COLOR_DISABLED 3

std::vector<int> downKeys = std::vector<int>{};

//keybd_event('B', 0, 0, 0);
//keybd_event('B', 0, KEYEVENTF_KEYUP, 0);
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	BOOL keyHandled = FALSE;

	if (nCode == HC_ACTION)
	{
		switch (wParam)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
			if (keyHandled = IntVecHas(downKeys, p->vkCode)) break;
			if (keyHandled = (p->vkCode >= 0x70 && p->vkCode <= 0x7b)) {
				if (!IntVecHas(downKeys, p->vkCode)) {
					downKeys.push_back(p->vkCode);
				}
				CT::ID(p->vkCode - 112);
			}

			if ((p->vkCode == 0x70)) {
				//Print("ALL on?");
				
				//Print("F1 Pressed");
				break;
			}
			else

			if ((p->vkCode == 0x71)) {
				
				//Print("F2 Pressed");
				break;
			}
			else

			if ((p->vkCode == 0x72)) {
				//Print("F3 Pressed");
				break;
			}
			else

			if ((p->vkCode == 0x73)) {
				//Print("F4 Pressed");
				break;
			}
			else

			if ((p->vkCode == 0x74)) {
				//Print("F5 Pressed");
				break;
			}
			else

			if ((p->vkCode == 0x75)) {
				//Print("F6 Pressed");
				break;
			}
			else

			if ((p->vkCode == 0x76)) {
				//Print("F7 Pressed");
				break;
			}
			else

			if ((p->vkCode == 0x77)) {
				//Print("F8 Pressed");
				break;
			}
			else

			if ((p->vkCode == 0x78)) {
				//Print("F9 Pressed");
				break;
			}
			else

			if ((p->vkCode == 0x79)) {
				//Print("F10 Pressed");
				break;
			}
			else

			if ((p->vkCode == 0x7a)) {
				//Print("F11 Pressed");
				break;
			}
			else

			if ((p->vkCode == 0x7b)) {
				//Print("F12 Pressed");
				break;
			}
			//std::cout << std::hex << p->vkCode << std::endl;
			break;
		}

		switch (wParam)
		{
		case WM_KEYUP:
		case WM_SYSKEYUP:
			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
			if (IntVecHas(downKeys, p->vkCode)) {
				IntVecRemove(downKeys, p->vkCode);
			}
		}
	}
	return(keyHandled ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}

unsigned int rgb(double ratio)
{
	//we want to normalize ratio so that it fits in to 6 regions
	//where each region is 256 units long
	int normalized = int(ratio * 256 * 6);

	//find the distance to the start of the closest region
	int x = normalized % 256;

	int red = 0, grn = 0, blu = 0;
	switch (normalized / 256)
	{
	case 0: red = 255;      grn = x;        blu = 0;       break;//red
	case 1: red = 255 - x;  grn = 255;      blu = 0;       break;//yellow
	case 2: red = 0;        grn = 255;      blu = x;       break;//green
	case 3: red = 0;        grn = 255 - x;  blu = 255;     break;//cyan
	case 4: red = x;        grn = 0;        blu = 255;     break;//blue
	case 5: red = 255;      grn = 0;        blu = 255 - x; break;//magenta
	}

	return red + (grn << 8) + (blu << 16);
}

HANDLE hConsole;

void SetLucidaFont()
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 16;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

const int cwidth = 80;

std::vector<std::wstring> CheatNames = std::vector <std::wstring>{ L"All", L"NoSpread", L"NoRecoil", L"Glow", L"2x Bullets", L"Noclip", L"NoFlash", L"Cav ESP", L"TriggerBot", L"UnlockAll" };

void SetChar(TCHAR cCharacter, COORD dwWriteCoord, DWORD nLength) {
	DWORD count;
	FillConsoleOutputCharacterA(
		hConsole,
		cCharacter,
		nLength,
		dwWriteCoord,
		&count
	);
}

void UI::Setup() {
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFOEX consoleInfo = CONSOLE_SCREEN_BUFFER_INFOEX();
	consoleInfo.cbSize = 96;

	GetConsoleScreenBufferInfoEx(hConsole, &consoleInfo);
	consoleInfo.srWindow.Bottom += 1;
	consoleInfo.ColorTable[1] = 0x10109c; // a11f1f
	consoleInfo.ColorTable[2] = 0x00ff00;
	consoleInfo.ColorTable[3] = 0x0000ff;
	consoleInfo.ColorTable[4] = 0xf2c71d;
	consoleInfo.dwSize = COORD{ cwidth, (SHORT)CheatNames.capacity() + 4 };
	SetConsoleScreenBufferInfoEx(hConsole, &consoleInfo);

	SetConsoleTextAttribute(hConsole, 4); // White is 15
	SetConsoleOutputCP(65001);

	SetChar(' ', { 0, 0 }, cwidth * (SHORT)CheatNames.capacity() + 4);
	SetConsoleCursorPosition(hConsole, { 0, 0 });

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = true; // set the cursor visibility
	SetConsoleCursorInfo(hConsole, &cursorInfo);
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLongA(consoleWindow, GWL_STYLE, GetWindowLongA(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	
	//SetLucidaFont();
	//_setmode(_fileno(stdout), _O_U16TEXT);

	//SetConsoleScreenBufferSize(hConsole, COORD{ 1000, 1000 });
	//SetConsoleDisplayMode(hConsole, CONSOLE_WINDOWED_MODE, COORD{ 100, 100 });
}

void SetRainbowColor(unsigned int color) {
	CONSOLE_SCREEN_BUFFER_INFOEX consoleInfo = CONSOLE_SCREEN_BUFFER_INFOEX();
	consoleInfo.cbSize = 96;

	GetConsoleScreenBufferInfoEx(hConsole, &consoleInfo);
	//consoleInfo.srWindow.Right += 1;
	consoleInfo.srWindow.Bottom += 1;
	consoleInfo.ColorTable[6] = color;
	SetConsoleScreenBufferInfoEx(hConsole, &consoleInfo);
}

void SetColor(int colorValue, COORD dwWriteCoord, DWORD nLength) {
	DWORD count;
	FillConsoleOutputAttribute(
		hConsole,
		colorValue,
		nLength,
		dwWriteCoord,
		&count
	);
}

/*
void CPrint(const wchar_t* text) {
	DWORD written;
	WriteConsoleW(hConsole, text, std::wcslen(text), &written, nullptr);
	std::cout << "\n";
}
*/
void CPrint(std::wstring text) {
	DWORD written;
	//std::cout << "\n";
	WriteConsoleW(hConsole, text.c_str(), std::wcslen(text.c_str()), &written, nullptr);
}

void inputThread() {
	SetConsoleCursorPosition(hConsole, { 9, (SHORT)CheatNames.capacity() + 2 });
	

	while (true) {
		std::cin.get();
		AttemptOutlines();
		SetConsoleCursorPosition(hConsole, { 9, (SHORT)CheatNames.capacity() + 2 });
		SetChar(' ', { 9, (SHORT)CheatNames.capacity() + 2 }, cwidth - 9);
	}
}

void UI::ConsoleThread() {
	
	int sideSize = (cwidth - 10) / 2;
	CPrint(std::wstring{ L"┏" } + MultStr(std::wstring{ L"━" }, sideSize) + std::wstring{ L"<OnyxR6>" } +MultStr(std::wstring{ L"━" }, sideSize) + std::wstring{ L"┓" });
	std::wstring lineStart = L"┃ [┄] ";
	for (std::wstring name : CheatNames) {
		CPrint(lineStart + name + MultStr(std::wstring{ L" " }, cwidth - 1 - (lineStart.length() + name.length())) + std::wstring{ L"┃" });
	}
	CPrint(std::wstring{ L"┗" } + MultStr(std::wstring{ L"━" }, cwidth-2) + std::wstring{ L"┛" });
	
	
	//Print("                                                                                                                               X");
	SetColor(1, { 0, 0 }, cwidth);
	SetColor(1, { (SHORT)sideSize+1, 0 }, 8);
	for (SHORT i = 1; i <= CheatNames.capacity(); i++) // Set colors
	{
		SetColor(1, { 0, i }, 1);
		SetColor(1, { cwidth-1, i }, 1);
		SetColor(15, { 2, i }, 3);

		
		//SetChar(CS::ID(i) ? '+' : L'┄', { 3, i }, 1);
	}
	SetColor(1, { 0, (SHORT)CheatNames.capacity() + 1 }, cwidth);

	int placement = 36;
	int place_height = 0;

	
	DWORD out;
	SetConsoleCursorPosition(hConsole, { (SHORT)placement, (SHORT)place_height + 1 });
	const char* text = " ______    __   __    __  __    __  __";
	WriteConsoleA(hConsole, text, 40, &out, nullptr);
	SetColor(6, { (SHORT)placement, (SHORT)place_height + 1 }, 40);

	SetConsoleCursorPosition(hConsole, { (SHORT)placement, (SHORT)place_height + 2 });
	text = "/\\  __ \\  /\\ \"-.\\ \\  /\\ \\_\\ \\  /\\_\\_\\_\\";
	WriteConsoleA(hConsole, text, 40, &out, nullptr);
	SetColor(6, { (SHORT)placement, (SHORT)place_height + 2 }, 40);

	SetConsoleCursorPosition(hConsole, { (SHORT)placement, (SHORT)place_height + 3 });
	text = "\\ \\ \\/\\ \\ \\ \\ \\-.  \\ \\ \\____ \\ \\/_/\\_\\/_";
	WriteConsoleA(hConsole, text, 42, &out, nullptr);
	SetColor(6, { (SHORT)placement, (SHORT)place_height + 3 }, 42);

	SetConsoleCursorPosition(hConsole, { (SHORT)placement, (SHORT)place_height + 4 });
	text = " \\ \\_____\\ \\ \\_\\\\\"\\_\\ \\/\\_____\\  /\\_\\/\\_\\";
	WriteConsoleA(hConsole, text, 42, &out, nullptr);
	SetColor(6, { (SHORT)placement, (SHORT)place_height + 4 }, 42);

	SetConsoleCursorPosition(hConsole, { (SHORT)placement, (SHORT)place_height + 5 });
	text = "  \\/_____/  \\/_/ \\/_/  \\/_____/  \\/_/\\/_/";
	WriteConsoleA(hConsole, text, 42, &out, nullptr);
	SetColor(6, { (SHORT)placement, (SHORT)place_height + 5 }, 42);

	SetConsoleCursorPosition(hConsole, { (SHORT)cwidth-21, (SHORT)CheatNames.capacity() });
	text = "By: Cyrex & int3nse";
	WriteConsoleA(hConsole, text, 19, &out, nullptr);
	SetColor(15, { (SHORT)cwidth - 21, (SHORT)CheatNames.capacity() }, 19);
	/*
	" ______    __   __    __  __    __  __"
	"/\\  __ \\  /\\ \"-.\\ \\  /\\ \\_\\ \\  /\\_\\_\\_\\"
	"\\ \\ \\/\\ \\ \\ \\ \\-.  \\ \\ \\____ \\ \\/_/\\_\\/_"
	" \\ \\_____\\ \\ \\_\\\\\"\\_\\ \\/\\_____\\  /\\_\\/\\_\\"
	"  \\/_____/  \\/_/ \\/_/  \\/_____/  \\/_/\\/_/"
	*/

	SetConsoleCursorPosition(hConsole, { 1, (SHORT)CheatNames.capacity() + 2 });
	WriteConsoleA(hConsole, "Input >", 7, &out, nullptr);
	//SetChar('>', { 2, (SHORT)CheatNames.capacity() + 2 }, 1);

	//SetColor(2, );
	std::vector<unsigned int> rainbow;
	for (double i = 0; i < 300; i++)
	{
		rainbow.push_back(rgb(i / 300));
	}

	std::thread InputThread = std::thread(inputThread);
	InputThread.detach();

	int i = 0;
	while (true) {
		SetRainbowColor(rainbow[i % 300]);
		//SetColor(6, { 3, 1 }, 1);
		i++;

		SetColor(6, { (SHORT)placement, (SHORT)place_height + 1 }, 40);
		SetColor(6, { (SHORT)placement, (SHORT)place_height + 2 }, 40);
		SetColor(6, { (SHORT)placement, (SHORT)place_height + 3 }, 42);
		SetColor(6, { (SHORT)placement, (SHORT)place_height + 4 }, 42);
		SetColor(6, { (SHORT)placement, (SHORT)place_height + 5 }, 42);

		SetColor(6, { (SHORT)cwidth - 17, (SHORT)CheatNames.capacity() }, 5);
		SetColor(6, { (SHORT)cwidth - 9, (SHORT)CheatNames.capacity() }, 7);

		SetColor(15, { 0, (SHORT)CheatNames.capacity() + 2 }, cwidth);

		//Sleep(9000);
		for (SHORT i = 0; i <= CheatNames.capacity()-1; i++)
		{
			//SetConsoleCursorPosition(hConsole, { 3, i+1 });
			DWORD written;
			SetChar(CS::ID(i) ? '+' : '-', { 3, i + 1 }, 1);
			SetColor(CS::ID(i) ? COLOR_ENABLED : COLOR_DISABLED, { 3, i+1 }, 1);
		}
		Sleep(10);
				//std::cin.get();
	}
}