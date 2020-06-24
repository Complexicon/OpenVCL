#include "OpenVCL.h"

class DebugWindow : public TWindow {
public:
	DebugWindow() {
		SetSize(200, 100);
		SetName(L"Test");
		SetPos(500, 500);
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
	Application::Initialize();
	Application::Run(new DebugWindow());
	return 0;
}