#include "OpenVCL.h"
#include <cstdio>

// Demo for own Control
class TLabelDebug : public TControl {
public:
	strbuf buffer;
	TLabelDebug(TWindow* owner) : TControl(owner){
		this->x = 50;
		this->y = 50;
		this->sizeX = 100;
		this->sizeY = 30;
		buffer = new char[500];
	};
	void Draw(Renderer* renderer) override {
		sprintf(buffer, "sizeX: %i, sizeY: %i", owner->GetWidth(), owner->GetHeight());
		renderer->DrawString(x,y,sizeX,sizeY, Color(0x808080), buffer);
	};
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, int nCmdShow) {

	// Create a Window
	TWindow window;

	// Set Window Parameters
	window.SetSize(250, 250);
	window.SetName("Demo");
	window.SetPos(500, 500);

	// Create a Button for example
	// 'window' is its owner, position is x:10 y:25 and the button-label is 'Test'
	TButton button(&window, 10, 25, "Test");

	// OnClick Handler
	// Can take a function pointer or lambda
	// example: Create a new window containing a label with text 'Ooh a Label!'
	button.OnClick = [](TControl* sender) { 
		// Create a new Window Object
		TWindow* popup = new TWindow();
		// Set Parameters
		popup->SetSize(500, 200);
		popup->SetName("I'm a second Window!");
		// Populate Controls
		popup->controls.append(new TLabel(popup, 20, 20, 300, 100, "Ooh a Label!"));
		// And Spawn it. Boom new window.
		Application::CreateTWindow(popup);
	};

	// Add 'button' to the controls-list of the window
	window.controls.append(&button);
	// Add the Demo Custom Control to the contols-list
	window.controls.append(new TLabelDebug(&window));

	// initialize and run the window.
	Application::Initialize();
	Application::Run(&window);
	return 0;
}