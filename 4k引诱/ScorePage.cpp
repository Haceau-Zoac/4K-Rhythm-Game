#include "ScorePage.h"
#include <string>
#include <string_view>

ScorePage::ScorePage(Application& app, int score)
	: Page(app), _score(score) {
}

void ScorePage::Paint() {
	std::wstring string{ L"SCORE: " };
	string += std::to_wstring(_score);

	auto opt{ GetTextBounds(string) };
	if (!opt.has_value()) return;
	auto textBounds{ opt.value() };

	RECT client;
	GetClientRect(_window, &client);
	PaintString(string, { client.right / 2.f - textBounds.Width / 2.f, client.bottom / 2.f - textBounds.Height / 2.f });
}