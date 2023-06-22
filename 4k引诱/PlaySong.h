#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include "ResourceId.h"
#include "Tap.h"
#include "Page.h"

class PlaySong : public Page {
public:
	PlaySong(Tap* tap, Application& app);

	long long StartTick();
	void Paint() override;
	void SetWindow(HWND window) override;
	void OnPress(WPARAM key) override;
	void OnMciNotify(WPARAM w) override;
	Tap* const Beatmap();

private:
	Tap* _taps;
	Tap* _nowTaps[4];
	long long const _songStartTicks;
	int _score;
	int const _startTick;

	void PaintLines();
	void PaintTaps();
	void PaintScores();
	void UpdateNowTap(int column);
	bool TapInGood(Tap* tap);
	bool TapInGreat(Tap* tap);
	bool TapInPerfect(Tap* tap);
	bool TapInTime(Tap* tap, float time);
};

