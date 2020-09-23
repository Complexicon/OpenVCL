#include "OpenVCL.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, int nCmdShow) {

	// Create a Window
	TWindow window;

	// Set Window Parameters
	window.SetSize(250, 250);
	window.SetName("testing 1");
	window.SetPos(500, 500);

	// Create a Button for example
	// 'window' is its owner, position is x:10 y:25 and the button-label is 'Test'
	TButton button(&window, 10, 25, "Buttön");

	// OnClick Handler
	// Can take a function pointer or lambda
	// example: set sender's (button's) position to a random value between 0 and 180 for x and y
	button.OnClick = [](TControl* sender) { sender->SetPos(rand() % sender->owner->GetWidth(), rand() % sender->owner->GetHeight()); };

	// Add 'button' to the controls-list of the window
	window.controls.append(&button);

	// initialize and run the window.
	Application::Initialize();
	Application::Run(&window);
	return 0;
}