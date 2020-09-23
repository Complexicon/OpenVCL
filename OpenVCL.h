#ifndef OPENVCL_H_
#define OPENVCL_H_

/*

	OpenVCL v0.0.41-alpha
	MADE BY COMPLEXICON aka cmplx

*/


// Show Debug Overlay
//#define _DEBUG_OVCL

#define IN_BOUNDS(x,y,rect) x > rect.left && x < rect.right&& y > rect.top && y < rect.bottom

#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#include "Helper/PointerList.h"
#include "types.h"

class Application;
class TWindow;
class TControl;
class Renderer;

enum class InputType { Key, Mouse };

class TControl {
protected:
	D2D1_RECT_F boundingBox = { 0 };
	int x = 0;
	int y = 0;
	int sizeX = 0;
	int sizeY = 0;

	void UpdateBoundingBox() {
		boundingBox.left = x;
		boundingBox.right = x + sizeX;
		boundingBox.top = y;
		boundingBox.bottom = y + sizeY;
	}

public:
	TWindow* owner;
	TControl(TWindow* owner) { this->owner = owner; };
	bool InBounds(int x, int y) { return IN_BOUNDS(x, y, boundingBox); }
	void SetSize(int width, int height) { sizeX = width; sizeY = height; UpdateBoundingBox(); }
	void SetPos(int x, int y) { this->x = x; this->y = y; UpdateBoundingBox(); }

	virtual void Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, IDWriteTextFormat* pTextFormat) = 0;
	virtual void UserInputEvent(InputType inputType, ulong64 param) {};

	friend class TWindow;
};

class TButton : public TControl {
private:
	bool clicked = false;
public:
	TButton(TWindow* owner, int x, int y, str label);
	void Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, IDWriteTextFormat* pTextFormat) override;
	void UserInputEvent(InputType inputType, ulong64 param) override;

	void (*OnClick)(TControl* Sender) = 0;

	str label = "Button";
	int radius = 0;
};

class TWindow {
public:

	//GETTER
	HWND Window();
	int GetWidth();
	int GetHeight();
	int GetWindowX();
	int GetWindowY();
	str GetWindowName();
	uint32 GetCurrentStyle();
	PointerList<TControl> controls;
	D2D1::ColorF background = D2D1::ColorF(0.1, 0.1, 0.1);

	int GetLastMouseX() { return lastMouseX; };
	int GetLastMouseY() { return lastMouseY; };

	//SETTER
	void SetSize(int Width, int Height);
	void SetPos(int X, int Y);
	void SetName(str WindowName);
	void SetStyle(uint32 WindowStyle);

	//Functions
	void Show(int type = SW_SHOWNORMAL);
	void Hide();
	void Update() { InvalidateRect(m_hwnd, nullptr, false); }

private:
	//Inner Workings
	ID2D1Factory* pFactory = 0;
	ID2D1HwndRenderTarget* pRenderTarget = 0;
	ID2D1SolidColorBrush* pBrush = 0;
	IDWriteTextFormat* pTextFormat = 0;
	IDWriteFactory* pDWriteFactory = 0;
	HWND m_hwnd = 0;

	str WindowName = "Window";
	uint32 WindowStyle = WS_OVERLAPPEDWINDOW;
	uint32 ExtendedWindowStyle = 0;
	int Width = CW_USEDEFAULT;
	int Height = CW_USEDEFAULT;
	int x = CW_USEDEFAULT;
	int y = CW_USEDEFAULT;

	int lastMouseX = 0;
	int lastMouseY = 0;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HRESULT CreateGraphicsResources();
	void DiscardGraphicsResources();
	void OnPaint();
	void Resize();
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	friend class Application;
};

class Application {
private:
	static str ClassName;
public:
	static void Initialize();
	static void Run(TWindow* window);
};

class Renderer {

};

class RendererWin : public Renderer {

};

#endif