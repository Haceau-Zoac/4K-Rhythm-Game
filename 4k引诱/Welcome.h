#pragma once
#include "Page.h"
#include <vector>
#include <optional>

extern std::vector<std::wstring_view> welcomeChoices;

class Welcome : public Page {
public:
	Welcome(Application& app);

	void Paint() override;
	void OnPress(WPARAM key) override;

private:
	std::size_t _choice;

	void PaintStrings();
	void PaintPolygon();
	std::optional<Gdiplus::PointF> GetTextPosition(std::size_t position);
};

