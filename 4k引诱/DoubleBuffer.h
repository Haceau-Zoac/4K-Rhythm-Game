#pragma once
#include <Windows.h>
#include <gdiplus.h>

class DoubleBuffer {
public:
	~DoubleBuffer();

	void SetWindow(HWND window);
	HDC GetDc();
	void Paint();

private:
	HWND _window;
	HBITMAP _backbuffer;
	HDC _backbufferDc;
	HDC _dc;
};

