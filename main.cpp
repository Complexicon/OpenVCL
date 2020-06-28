#include "OpenVCL.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
	// Create a Window
	TWindow* window = new TWindow();

	// Set Window Parameters
	window->SetSize(250, 250);
	window->SetName(L"Test");
	window->SetPos(500, 500);

	// Create a Button for example
	// 'window' is its owner, position is x:10 y:25 and the button-label is 'Test'
	TButton* button = new TButton(window, 10, 25, L"Test");

	// OnClick Handler
	// Can take a function pointer or lambda
	// example: set sender's (button's) position to a random value between 0 and 180 for x and y
	button->OnClick = [](TControl* sender) { sender->SetPos(rand() % 180, rand() % 180); };

	// Add 'button' to the controls-list of the window
	window->controls.append(button);

	// initialize and run the window.
	Application::Initialize();
	Application::Run(window);
	return 0;
}