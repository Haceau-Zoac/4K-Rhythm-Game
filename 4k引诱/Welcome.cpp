#include "Welcome.h"
#include "Application.h"
#include "PlaySong.h"
#include "ResourceId.h"
#include "Util.h"
using namespace Gdiplus;

std::vector<std::wstring_view> welcomeChoices = {
	L"开始！",
	L"退出！"
};

// !!! second 必须 有序
Tap beatmap[] = {
	{.column = 1, .second = 0.8 },
	{.column = 2, .second = 1.9 },
	{.column = 3, .second = 2.7 },
	{.column = 4, .second = 3.7 },
	{.column = 2, .second = 3.9 },
	{.column = 4, .second = 4.2 },
	{.column = 1, .second = 4.8 },
	{.column = 3, .second = 5.6 },
	{.column = 2, .second = 5.8 },
	{.column = 2, .second = 6.2 },
	{.column = 4, .second = 6.3 },
	{.column = 4, .second = 6.5 },
	{.column = 0, .second = 0 } // end
};

float HalfLength(std::wstring_view string) {
	return string.length() / 2.f;
}

Welcome::Welcome(Application& app)
	: Page(app), _choice(0) {
}

void Welcome::Paint() {
	PaintStrings();
	PaintPolygon();
}

void Welcome::OnPress(WPARAM key) {
	if (key == KeyId::Enter) {
		if (_choice == 0) {
			PlaySong* page{ new PlaySong(beatmap, _app) };
			page->SetWindow(_window);
			_app.ChangeScene(page);
		} else {
			PostMessage(_window, WM_CLOSE, 0, 0);
		}
	} else if (key == KeyId::Up) {
		if (_choice-- == 0) {
			_choice = welcomeChoices.size() - 1;
		}
	}
	else if (key == KeyId::Down) {
		if (++_choice == welcomeChoices.size()) {
			_choice = 0;
		}
	}
}

void Welcome::PaintStrings() {
	RECT client;
	GetClientRect(_window, &client);
	for (std::size_t ix{ 0 }; ix != welcomeChoices.size(); ++ix) {
		auto textPositionOptional{ GetTextPosition(ix) };
		if (!textPositionOptional.has_value()) return;
		auto textPosition{ textPositionOptional.value() };
		PaintString(welcomeChoices[ix].data(), textPosition);
	}
}

void Welcome::PaintPolygon() {
	Pen pen(Color(255, 0, 0, 0), 3);
	RECT client;
	GetClientRect(_window, &client);

	auto textBoundsOptional{ GetTextBounds(welcomeChoices[_choice]) };
	if (!textBoundsOptional.has_value()) return;
	auto textBounds{ textBoundsOptional.value() };
	auto textPosition{ GetTextPosition(_choice).value() };

	PointF points[3] = {
		{ textPosition.X - 15.f, textPosition.Y + 5.f },
		{ textPosition.X - 5.f, textPosition.Y + textBounds.Height / 2.f + 5.f },
		{ textPosition.X - 15.f, textPosition.Y + textBounds.Height + 5.f }
	};
	_app.GetGraphics().DrawPolygon(&pen, points, 3); // https://learn.microsoft.com/en-us/windows/win32/api/gdiplusgraphics/nf-gdiplusgraphics-graphics-drawpolygon(constpen_constpointf_int)
}

std::optional<Gdiplus::PointF> Welcome::GetTextPosition(std::size_t position) {
	RECT client;
	GetClientRect(_window, &client);
	auto stringSizeOptinal{ GetTextBounds(welcomeChoices[position]) };
	if (!stringSizeOptinal.has_value()) return std::nullopt;
	auto stringSize{ stringSizeOptinal.value() };
	return PointF{ client.right / 2.f - stringSize.Width / 2.f,
			client.bottom / 2.f - stringSize.Height / 2.f + (stringSize.Height + 10.f) * position };
}