#ifndef OPENVCL_H_
#define OPENVCL_H_

/*

	OpenVCL v0.0.45-alpha
	MADE BY COMPLEXICON aka cmplx

*/


// Show Debug Overlay
//#define _DEBUG_OVCL

#define IN_BOUNDS(x,y,bx,by,szX,szY) x >= bx && x <= (bx+szX)&& y >= by && y <= (by+szY)
#define BYTE_TO_FLOAT(b) (b / 255.f)

#include <windows.h>

#include "Helper/PointerList.h"
#include "types.h"

class Application;
class TWindow;
class TControl;
class Renderer;

class TControl {
protected:
public:
	TWindow* owner;
	int x = 0;
	int y = 0;
	int sizeX = 0;
	int sizeY = 0;

	TControl(TWindow* owner) { this->owner = owner; };
	bool InBounds(int x, int y) { return IN_BOUNDS(x, y, this->x, this->y, this->sizeX, this->sizeY); }

	virtual void Draw(Renderer* renderer) = 0;
	virtual void UserInputEvent(InputType inputType, ulong64 param) {};

	friend class TWindow;
};

class TWindow {
public:

	PointerList<TControl> controls;
	Color background = Color(0x232323);

	//GETTER
	HWND Window() { return m_hwnd; };
	int GetWidth() { return Width; };
	int GetHeight() { return Height; };
	int GetWindowX() { return x; };
	int GetWindowY() { return y; };
	str GetWindowName() { return WindowName; };
	uint32 GetCurrentStyle() { return WindowStyle; };
	int GetLastMouseX() { return lastMouseX; };
	int GetLastMouseY() { return lastMouseY; };

	//SETTER
	void SetStyle(uint32 WindowStyle) {
		this->WindowStyle = WindowStyle;
		if (m_hwnd) SetWindowLongPtr(m_hwnd, GWL_STYLE, WindowStyle);
	};

	void SetSize(int Width, int Height) {
		this->Width = Width;
		this->Height = Height;
		if (m_hwnd) SetWindowPos(m_hwnd, nullptr, 0, 0, Width, Height, SWP_NOMOVE | SWP_NOZORDER);
	};

	void SetPos(int X, int Y) {
		this->x = X;
		this->y = Y;
		if (m_hwnd) SetWindowPos(m_hwnd, nullptr, X, Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	};

	void SetName(str WindowName) {
		this->WindowName = WindowName;
		if (m_hwnd) SetWindowTextA(m_hwnd, WindowName);
	};

	//Functions
	void Show(int type = SW_SHOWNORMAL);
	void Hide();
	void Update() { InvalidateRect(m_hwnd, nullptr, false); }

private:
	//Inner Workings
	Renderer* r;

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

	void OnPaint();
	void Resize();
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	friend class Application;
	friend class Renderer;
};

class Application {
private:
	static str ClassName;
public:
	static void Initialize();
	static void CreateTWindow(TWindow* window);
	static void Run(TWindow* window);
};

class Renderer {
public:
	virtual void DrawLine(int x0, int y0, int x1, int y1, const Color& color, float w = 1.f) = 0;
	virtual void DrawRoundedRect(int x, int y, int width, int height, const Color& color, float radius = 1.f) = 0;
	virtual void DrawString(int x, int y, int width, int height, const Color& color, str text) = 0;

	virtual void FillRoundedRect(int x, int y, int width, int height, const Color& color, float radius = 1.f) = 0;
	Renderer(TWindow* client) : client(client) {}
protected:
	TWindow* client;
	virtual bool Setup() = 0;
	virtual bool CreateTarget() = 0;
	virtual void DiscardTarget() = 0;
	virtual void Destroy() = 0;
	virtual void BeginDraw() = 0;
	virtual bool EndDraw() = 0;
	virtual void Resize() = 0;
	virtual void Clear(const Color& color) = 0;

	friend class TWindow;
};

#include "vclrenderer.h"
#include "vclcontrols.h"

#endif