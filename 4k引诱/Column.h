#pragma once
#include <windows.h>
#include <gdiplus.h>

class Column {
public:
	Column(int column, HWND window);

	Gdiplus::RectF LineRectangle();
	Gdiplus::RectF TapRectangle(float sec = 0);
private:
	RECT _clientRectangle;
	int _column;
};

