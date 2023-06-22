#include "PlaySong.h"
#include "Song.h"
#include "Column.h"
#include "Tap.h"
#include "Application.h"
#include "ScorePage.h"
#include <string>
using namespace Gdiplus;

PlaySong::PlaySong(Tap* taps, Application& app)
    : _taps(taps), _songStartTicks(GetTickCount64()), _nowTaps{ 0,0,0,0 }, _score(0), _startTick(GetTickCount64()), Page(app) {
}

void PlaySong::SetWindow(HWND window) {
    _window = window;

    mciSendStringW(L"open .\\res\\dBdoll.mp3", nullptr, 0, nullptr);
    mciSendStringW(L"play .\\res\\dBdoll.mp3 notify", nullptr, 0, _window);
}

void PlaySong::Paint() {
    PaintLines();
    PaintTaps();
    PaintScores();
}

void PlaySong::PaintLines() {
    Image image(L".\\res\\line.bmp");
    unsigned width{ image.GetWidth() };
    unsigned height{ image.GetHeight() };
    RECT clientRectangle;
    GetClientRect(_window, &clientRectangle);
    for (int i = 1; i <= 4; ++i) {
        Column col(i, _window);
        _app.GetGraphics().DrawImage(&image, col.LineRectangle(), 10, 10, width - 20, height - 20, UnitPixel);
    }
}

long long PlaySong::StartTick() {
    return _startTick;
}

void PlaySong::PaintTaps() {
    float const second{ (GetTickCount64() - _startTick) / 1000.f };
    Image image(L".\\res\\tap.bmp");
    unsigned width{ image.GetWidth() };
    unsigned height{ image.GetHeight() };
    RECT clientRectangle;
    GetClientRect(_window, &clientRectangle);
    for (auto tap{ _taps }; tap->column != 0; ++tap) {
        if (tap->column == -1) continue;
        if (tap->second > second + 4 && (_nowTaps[tap->column - 1] != nullptr)) break;
        if (tap->second + 1 - second < 0) _taps++;
        if ((_nowTaps[tap->column - 1] == nullptr) || !TapInGood(_nowTaps[tap->column - 1])) {
            _nowTaps[tap->column - 1] = tap;
        }
        Column col(tap->column, _window);
        _app.GetGraphics().DrawImage(&image, col.TapRectangle(second - tap->second), 10, 10, width - 20, height - 20, UnitPixel);
    }
}

void PlaySong::PaintScores() {
    PaintString(std::wstring{ L"score: " } + std::to_wstring(_score), PointF{ 30.f, 10.f });
}

Tap* const PlaySong::Beatmap() {
    return _taps;
}

void PlaySong::OnPress(WPARAM key) {
    if (key != KeyId::D && key != KeyId::F && key != KeyId::J && key != KeyId::K) return;

    float const second{ (GetTickCount64() - _songStartTicks) / 1000.f };
    int column;
    switch (key) {
    case KeyId::D: column = 0; break;
    case KeyId::F: column = 1; break;
    case KeyId::J: column = 2; break;
    case KeyId::K: column = 3; break;
    default: return;
    }
    Tap* now{ _nowTaps[column] };
    if (now->column == 0) return;
    if (TapInGood(now)) { // 100ms
        if (TapInGreat(now)) ++_score; // 25ms
        if (TapInPerfect(now)) ++_score; // 50ms
        UpdateNowTap(column + 1);
        now->column = -1;
        ++_score;
    }
}

void PlaySong::OnMciNotify(WPARAM w) {
    ScorePage* scorePage{ new ScorePage(_app, _score) };
    scorePage->SetWindow(_window);
    _app.ChangeScene(scorePage);
}

void PlaySong::UpdateNowTap(int const column) {
    Tap* tap{ _nowTaps[column - 1] + 1 };
    // 把这个 column 上的 nowTap 变为下一个.
    for (; tap->column != 0; ++tap) {
        if (tap->column == column) {
            _nowTaps[column - 1]->column = -1;
            _nowTaps[column - 1] = tap;
            break;
        }
    }
    if (tap->column == 0) _nowTaps[column - 1] = tap;
    if (tap == _taps + 1) ++_taps;
}

// 100ms 以内.
bool PlaySong::TapInGood(Tap* tap) {
    return TapInTime(tap, 0.1);
}
// 50ms 以内.
bool PlaySong::TapInGreat(Tap* tap) {
    return TapInTime(tap, 0.05);
}
// 25ms 以内.
bool PlaySong::TapInPerfect(Tap* tap) {
    return TapInTime(tap, 0.02);
}

bool PlaySong::TapInTime(Tap* tap, float time) {
    float const second{ (GetTickCount64() - _songStartTicks) / 1000.f };
    return second + time > tap->second && second - time < tap->second;
}