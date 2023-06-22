#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <string_view>

Gdiplus::SizeF GetTextBounds(Gdiplus::Font const& font, Gdiplus::StringFormat& stringFormat,
	std::wstring_view text);