#include "Song.h"
#include <Windows.h>
#include <mmsystem.h>

Song::Song(wchar_t const* song) : _song(song) {}

void Song::Play() {
	// https://blog.csdn.net/PandaOS/article/details/50410155
	MCI_OPEN_PARMS mciOpen{};
	mciOpen.lpstrDeviceType = L"mpegvideo";
	mciOpen.lpstrElementName = _song;
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, reinterpret_cast<DWORD>(&mciOpen));

	MCI_PLAY_PARMS mciPlay{};
	mciPlay.dwCallback = reinterpret_cast<DWORD>(nullptr);
	mciPlay.dwFrom = 0;
	mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM, reinterpret_cast<DWORD>(&mciPlay));
}