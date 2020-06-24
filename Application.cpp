#include "OpenVCL.h"

PCWSTR Application::ClassName = L"OpenVCLWindow";

void Application::Initialize() {
	// register OpenVCLWindow
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = TWindow::WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = ClassName;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	RegisterClass(&wc);
}

void Application::Run(TWindow* w) {
	CreateWindowExW(
		w->ExtendedWindowStyle,
		ClassName,
		w->WindowName,
		w->WindowStyle,
		w->x, w->y,
		w->Width,
		w->Height,
		nullptr, nullptr,
		GetModuleHandle(nullptr), w
	);
	w->Show();

	// main program loop
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}
