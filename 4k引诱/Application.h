#pragma once
#include <thread>
#include <mutex>
#include <Windows.h>
#include <gdiplus.h>
#include "ResourceId.h"
#include "Page.h"
#include "DoubleBuffer.h"

extern bool gameIsRunning;

class Application {
public:
	Application(HINSTANCE instance);
	~Application();

	int Start(int showCommand);

	static LRESULT CALLBACK WindowProcedure(HWND window, unsigned message, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(unsigned message, WPARAM wParam, LPARAM lParam);
	Gdiplus::Graphics GetGraphics();
	void ChangeScene(Page* scene);

private:
	HWND _window;
	HBRUSH _brushes[BrushId::Total];
	ULONG_PTR _gdiplusToken;
	Page* _scene;
	DoubleBuffer _buffer;
	std::thread _gameLoop;
	std::mutex _gameLoopLock;

	void CreateResources();
	void Loop();
	void Paint();
};

