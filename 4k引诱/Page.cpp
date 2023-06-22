#include "Page.h"
#include "Application.h"
#include <gdiplus.h>
#include "Util.h"
using namespace Gdiplus;

Page::Page(Application& app)
    : _app(app), _window(nullptr) {
}

void Page::OnPress(WPARAM key) {
}

void Page::OnMciNotify(WPARAM key) {
}

void Page::SetWindow(HWND window) {
    _window = window;
}

Font Page::GetFont() {
    FontFamily fontFamily(L"Microsoft YaHei");
    return Font{ &fontFamily, 12, FontStyleBold, UnitPoint };
}

void Page::PaintString(std::wstring_view string, PointF point) {
    SolidBrush brush(Color(0, 0, 0));
    Font&& font{ GetFont() };
    _app.GetGraphics().DrawString(string.data(), -1, &font, point, nullptr, &brush);
}

std::optional<Gdiplus::SizeF> Page::GetTextBounds(std::wstring_view string) {
	StringFormat format{};
	if (format.GetLastStatus() == GdiplusNotInitialized) {
		return std::nullopt;
	}
	RECT client;
	GetClientRect(_window, &client);
	Font font{ GetFont() };
	return ::GetTextBounds(font, format, string);
}
