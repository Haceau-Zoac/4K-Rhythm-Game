#include "Util.h"
using namespace Gdiplus;

// https://blog.csdn.net/analogous_love/article/details/42295643
// 获取文字宽度和高度
Gdiplus::SizeF GetTextBounds(Gdiplus::Font const& font, Gdiplus::StringFormat& stringFormat,
	std::wstring_view text) {
	GraphicsPath graphicsPath;
	FontFamily fontFamily;
	font.GetFamily(&fontFamily);
	graphicsPath.AddString(text.data(), -1, &fontFamily, font.GetStyle(), font.GetSize(),
		PointF(0, 0), &stringFormat);
	RectF bound;
	graphicsPath.GetBounds(&bound);
	return SizeF(bound.Width, bound.Height);
}