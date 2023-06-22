#include "DoubleBuffer.h"

DoubleBuffer::~DoubleBuffer() {
	if (_backbufferDc != nullptr) {
		DeleteDC(_backbufferDc);
	}
	if (_backbuffer != nullptr) {
		DeleteObject(_backbuffer);
	}
}

void DoubleBuffer::SetWindow(HWND window) {
	_window = window;
	_dc = GetDC(window);

	RECT clientRectangle;
	GetClientRect(window, &clientRectangle);
	_backbuffer = CreateCompatibleBitmap(_dc, clientRectangle.right - clientRectangle.left,
		clientRectangle.bottom - clientRectangle.top);
	_backbufferDc = CreateCompatibleDC(_dc);
	SelectObject(_backbufferDc, _backbuffer);
}

HDC DoubleBuffer::GetDc() {
	return _backbufferDc;
}

void DoubleBuffer::Paint() {
	RECT clientRectangle;
	GetClientRect(_window, &clientRectangle);
	BitBlt(_dc, 0, 0, clientRectangle.right - clientRectangle.left, clientRectangle.bottom - clientRectangle.top,
		_backbufferDc, 0, 0, SRCCOPY);
}