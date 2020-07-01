#ifndef OPENVCL_H_
#define OPENVCL_H_

/*

	OpenVCL v0.0.38-alpha
	MADE BY COMPLEXICON aka cmplx

*/


// Show Debug Overlay
//#define _DEBUG_OVCL

#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#include "Helper/PointerList.h"

#include "typedefs.h"

class TWindow;

class TControl {
protected:
	D2D1_RECT_F boundingBox = { 0 };
	TWindow* owner;
	int x = -1;
	int y = -1;
	int sizeX = 0;
	int sizeY = 0;

	void UpdateBoundingBox() {
		boundingBox.left = x;
		boundingBox.right = x + sizeX;
		boundingBox.top = y;
		boundingBox.bottom = y + sizeY;
	}

public:
	TControl(TWindow* owner) { this->owner = owner; };
	bool InBounds(int x, int y) { return x > boundingBox.left && x < boundingBox.right&& y > boundingBox.top && y < boundingBox.bottom; }
	void SetSize(int length, int height) { sizeX = length; sizeY = height; UpdateBoundingBox(); }
	void SetPos(int x, int y) { this->x = x; this->y = y; UpdateBoundingBox(); }

	virtual void Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, IDWriteTextFormat* pTextFormat) = 0;


	// Mouse Button Handling POC
	virtual void MD() {};
	virtual void MU() {};


	friend class TWindow;
};

class TButton : public TControl {
private:
	bool clicked = false;
public:
	TButton(TWindow* owner, int x, int y, PCWSTR label);
	void Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, IDWriteTextFormat* pTextFormat) override;
	void MD() override;
	void MU() override;

	ClickEvent OnClick = 0;

	PCWSTR label = L"Button";
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
	PCWSTR GetWindowName();
	DWORD GetCurrentStyle();
	PointerList<TControl> controls;
	D2D1::ColorF background = D2D1::ColorF(0.1, 0.1, 0.1);

	int GetLastMouseX() { return lastMouseX; };
	int GetLastMouseY() { return lastMouseY; };

	//SETTER
	void SetSize(int Width, int Height);
	void SetPos(int X, int Y);
	void SetName(PCWSTR WindowName);
	void SetStyle(DWORD WindowStyle);

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

	PCWSTR WindowName = L"Window";
	DWORD WindowStyle = WS_POPUP;
	DWORD ExtendedWindowStyle = 0;
	int Width = CW_USEDEFAULT;
	int Height = CW_USEDEFAULT;
	int x = CW_USEDEFAULT;
	int y = CW_USEDEFAULT;

	int lastMouseX = 0;
	int lastMouseY = 0;

	// DEBUG VARS
	bool mb_down = false;
	bool buttonPressed = false;
	bool executeEvent = false;
	// END DEBUG VARS

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
	static PCWSTR ClassName;
public:
	static void Initialize();
	static void Run(TWindow* window);
};

#endif