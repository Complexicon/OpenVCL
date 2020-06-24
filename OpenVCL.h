#ifndef OPENVCL_H_
#define OPENVCL_H_

#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

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

	//SETTER
	void SetSize(int Width, int Height);
	void SetPos(int X, int Y);
	void SetName(PCWSTR WindowName);
	void SetStyle(DWORD WindowStyle);

	//Functions
	void Show(int type = SW_SHOWNORMAL);
	void Hide();

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

	// DEBUG VARS
	int testX = 0;
	int testY = 0;
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