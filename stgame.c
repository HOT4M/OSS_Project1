#include <Windows.h>
#include <stdio.h>
#include "stgame.h"

HANDLE hConsole;

void Initial() {
	CONSOLE_CURSOR_INFO csCursor;

	csCursor.bVisible = FALSE;
	csCursor.dwSize = 1;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &csCursor);
}

void MoveCursor(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;

	SetConsoleCursorPosition(hConsole, coord);

}

void ClearScreen() {
	int x, y;

	for (y = 0; y < HEIGHT; y++) {
		MoveCursor(0, y);
		for (x = 0; x < WIDTH; x++) {
			printf("%c", ' ');
		}
	}
}