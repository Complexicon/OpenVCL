#include "OpenVCL.h"

str Application::ClassName = "OpenVCLWindow";

void Application::Initialize() {
	// register OpenVCLWindow
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = TWindow::WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.lpszClassName = ClassName;
	RegisterClass(&wc);
}

void Application::CreateTWindow(TWindow* w) {
	CreateWindowExA(
		w->ExtendedWindowStyle,
		ClassName,
		w->WindowName,
		w->WindowStyle,
		w->x, w->y,
		w->Width,
		w->Height,
		nullptr, nullptr,
		GetModuleHandleA(nullptr), w
	);
	w->Show();
}

void Application::Run(TWindow* w) {
	CreateTWindow(w);
	// main program loop
	MSG msg = { 0 };
	while (GetMessageA(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

}
