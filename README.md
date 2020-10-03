# OpenVCL Debug Project and Source

Current version: OpenVCL v0.0.45-alpha

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
	// example: Create a MessageBox on Button Click
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
```

## Demo Pictures
![Demo Button and Close Box](https://i.imgur.com/33twnb1.png)
![Multiple Controls (Buttons)](https://i.imgur.com/o7EViYl.png)



### The Current version is a very early Debug Version of OpenVCL and very unstable as it is at the start of Development right now!

### Credits
- Microsoft Documentation
