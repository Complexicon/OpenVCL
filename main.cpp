#include "OpenVCL.h"

class DebugWindow : public TWindow {
public:
	DebugWindow() {
		SetSize(250, 250);
		SetName(L"Test");
		SetPos(500, 500);

	}
};

void OnClickTest(TControl* s) {
	MessageBoxA(NULL, "test", "lol", MB_OK);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
	DebugWindow* w = new DebugWindow();
	TButton* b = new TButton(w, 10, 25, L"1st");

	b->OnClick = &OnClickTest;

	w->controls.append(b);

	Application::Initialize();
	Application::Run(w);
	return 0;
}