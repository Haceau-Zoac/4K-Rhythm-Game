#include <Windows.h>
#include "Application.h"

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int cmd) {
	Application app{ instance };

	return app.Start(cmd);
}