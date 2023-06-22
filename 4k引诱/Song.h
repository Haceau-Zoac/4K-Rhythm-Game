#pragma once
class Song {
public:
	Song(wchar_t const song[]);

	void Play();
private:
	wchar_t const* _song;
};

