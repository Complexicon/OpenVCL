#define OPENVCL_APP
#include "OpenVCL.h"

int __stdcall WinMain (HINSTANCE,HINSTANCE,LPSTR,int) {

	// Create a Window
	TWindow window("Demo Window", 300, 300);

	// Create a Button for example
	// 'window' is its owner, position is x:10 y:25 and the button-label is 'Test'
	TButton button(&window, 10, 25, "Test");

	// OnClick Handler
	// Can take a function pointer or lambda
	// example: Create a new window containing a label with text 'Ooh a Label!'
	button.OnClick = [](TControl* s) { 
		s->GetWindow()->MsgBox("Hey", "What's Up?");
	};

	// Add 'button' to the controls-list of the window
	window.controls.append(&button);

	// initialize and run the window.
	Application::Initialize();
	Application::Run(&window);
	return 0;
}