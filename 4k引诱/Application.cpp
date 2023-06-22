#include "Application.h"
#include "Welcome.h"
#include "Tap.h"

bool gameIsRunning = true;

Application::Application(HINSTANCE instance)
    : _window{ nullptr }, _brushes{}, _gameLoop{&Application::Loop, this}, _scene(new Welcome(*this)){
    WNDCLASS windowClass{};

    windowClass.lpfnWndProc = Application::WindowProcedure;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = L"4k";
    windowClass.style = CS_OWNDC; // Ë«»º³å https://www.cnblogs.com/zjutzz/p/10925397.html

    RegisterClass(&windowClass);

    _window = CreateWindowEx(0, L"4k", L"4k ÒýÓÕ", WS_OVERLAPPEDWINDOW ^ WS_SIZEBOX ^ WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, instance, this);

    _scene->SetWindow(_window);
    _buffer.SetWindow(_window);

    Gdiplus::GdiplusStartupInput gdiplusStartupInput{};
    Gdiplus::GdiplusStartup(&_gdiplusToken, &gdiplusStartupInput, nullptr);
    _gameLoop.detach();
}

Application::~Application() {
    Gdiplus::GdiplusShutdown(_gdiplusToken);
    if (_scene != nullptr) {
        delete _scene;
    }
}

int Application::Start(int showCommand) {

    ShowWindow(_window, showCommand);

    MSG message{};
    while (GetMessage(&message, _window, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return 0;
}

LRESULT CALLBACK Application::WindowProcedure(HWND window, unsigned message, WPARAM wParam, LPARAM lParam) {
	Application* self{ nullptr };

	if (message == WM_NCCREATE) {
		CREATESTRUCT* create{ reinterpret_cast<CREATESTRUCT*>(lParam) };
        self = reinterpret_cast<Application*>(create->lpCreateParams);
        SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)self);
        self->CreateResources();
        self->_window = window;
    } else {
        self = (Application*)GetWindowLongPtr(window, GWLP_USERDATA);
    }
    if (self && (message != WM_PAINT)) {
        return self->HandleMessage(message, wParam, lParam);
    } else {
        return DefWindowProc(window, message, wParam, lParam);
    }
}

LRESULT Application::HandleMessage(unsigned message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        gameIsRunning = false;
        _window = nullptr;
        return 0;
    case WM_ERASEBKGND: // https://blog.csdn.net/weixin_45699916/article/details/105620765
        break;
    case WM_KEYDOWN:
        _scene->OnPress(wParam);
        break;
    case MM_MCINOTIFY:
        _scene->OnMciNotify(wParam);
        break;
    default:
        return DefWindowProc(_window, message, wParam, lParam);
    }
    return TRUE;
}

Gdiplus::Graphics Application::GetGraphics() {
    return _buffer.GetDc();
}

void Application::ChangeScene(Page* scene) {
    std::lock_guard<std::mutex> lockGuard{ _gameLoopLock };
    if (_scene != nullptr) delete _scene;
    _scene = scene;
}

void Application::CreateResources() {
    _brushes[BrushId::Gray] = static_cast<HBRUSH>(GetStockObject(GRAY_BRUSH));
}

void Application::Loop() {
    constexpr auto framesPerSecond{ 60l };
    constexpr auto skipTicks{ 1000 / framesPerSecond };
    int sleepTime{ 0 };
    auto nextGameTick = GetTickCount64();
    auto const startGameTick = GetTickCount64();

    while (gameIsRunning) {
        InvalidateRgn(_window, nullptr, false); // https://learn.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-invalidatergn
        Paint();
        nextGameTick += skipTicks;
        sleepTime = nextGameTick - GetTickCount64();
        if (sleepTime > 0) {
            Sleep(sleepTime);
        }
    }
}

void Application::Paint() {
    std::lock_guard<std::mutex> lockGuard{ _gameLoopLock };
    RECT client;
    GetClientRect(_window, &client);
    FillRect(_buffer.GetDc(), &client, _brushes[BrushId::Gray]);
    _scene->Paint();
    _buffer.Paint();
}