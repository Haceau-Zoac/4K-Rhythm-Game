#pragma once
#include <string_view>
#include <optional>
#include <Windows.h>
#include <gdiplus.h>

class Application;

class Page {
public:
	Page(Application& app);
	void virtual Paint() = 0;
	void virtual OnPress(WPARAM key);
	void virtual OnMciNotify(WPARAM key);
	void virtual SetWindow(HWND window);
	Gdiplus::Font virtual GetFont();

	std::optional<Gdiplus::SizeF> GetTextBounds(std::wstring_view string);
protected:
	Application& _app;
	HWND _window;

	void PaintString(std::wstring_view string, Gdiplus::PointF point);
};

