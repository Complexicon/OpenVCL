# OpenVCL Debug Project and Source

Current version: OpenVCL v0.0.37-alpha

## What is OpenVCL?
OpenVCL stands for Open Visual Component Library aiming to be a very robust Framework for creating Graphical C++ Applications for Windows whilst being as easy to use as possible.

The name is derived from "VCL" by Embarcadero / Borland as our IT Teacher currently forces us to use 
Borland C++ Builder 5.x which is from 2005. I want to create a better solution to that!

Features (hopefully):
- A very clean and modern UI look - "CleanUI" (working currently)
- GPU Hardware Acceleration as Direct2D is used for rendering (working currently)
- Very easy to learn and use
-  and much more!

## Usage Syntax:
```cpp
#include "OpenVCL.h"

class Window1 : public TWindow {
public:
	// NOTE: you dont have to put anything in the constructor
	Window1() {
		// example: Set window size
		SetSize(200, 100);
		// example: Set window title
		SetName(L"Test");
		// example: Set starting position
		SetPos(500, 500);

		// Add some Controls Maybe? x:50 y:20
		controls.append(new TButton(this, 50, 20, L"My first Button"));

	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
	Application::Initialize();
	Application::Run(new Window1());
	return 0;
}
```

## Pictures (The Yellow Square you can see is for debugging and showing the Cursor Position.)
![The debug button was pressed](https://i.imgur.com/6zOzCyL.png)
![Debug button not pressed](https://i.imgur.com/Dm5EAZB.png)



## The Current version is a very early Debug Version of OpenVCL and very unstable as it is at the start of Development right now!

### Credits
Microsoft Documentation
