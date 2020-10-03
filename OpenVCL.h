#ifndef OPENVCL_H_
#define OPENVCL_H_

/*

	OpenVCL v0.0.48-alpha
	MADE BY COMPLEXICON aka cmplx

*/

// DEBUG FUNCTIONS
#ifdef DEBUG_VCL

#include "console.h"
#define LOG(str) Console::Log(str);
#define CONSOLE() Console::Alloc();

#else

#define LOG(str)
#define CONSOLE()

#endif


#define IN_BOUNDS(x,y,bx,by,szX,szY) x >= bx && x <= (bx+szX)&& y >= by && y <= (by+szY)
#define BYTE_TO_FLOAT(b) (b / 255.f)

#include <windows.h>

#include "PointerList.h"
#include "types.h"

class Application;
class TWindow;
class TControl;
class Renderer;

/////////////
// TWindow //
/////////////

class TWindow {
// ------------------------ VARIABLES ---------------------------------
public:
	PointerList<TControl> controls;
	Color background = Color(0x232323);

private:
	Renderer* r;

	HWND hwnd = 0;

	str WindowName = "Window";
	uint32 WindowStyle = WS_OVERLAPPEDWINDOW;
	uint32 ExtendedWindowStyle = 0;
	int Width = CW_USEDEFAULT;
	int Height = CW_USEDEFAULT;
	int x = CW_USEDEFAULT;
	int y = CW_USEDEFAULT;

	int lastMouseX = 0;
	int lastMouseY = 0;

// ------------------------ FUNCTIONS ---------------------------------
public:

// ------------------------- Getter -----------------------------------
	HWND Window() { return hwnd; };
	int GetWidth() { return Width; };
	int GetHeight() { return Height; };
	int GetWindowX() { return x; };
	int GetWindowY() { return y; };
	str GetWindowName() { return WindowName; };
	uint32 GetCurrentStyle() { return WindowStyle; };
	int GetLastMouseX() { return lastMouseX; };
	int GetLastMouseY() { return lastMouseY; };
	Renderer* GetRenderer() { return r; };

// ------------------------- Setter -----------------------------------
	void SetStyle(uint32 WindowStyle) {
		this->WindowStyle = WindowStyle;
		if (hwnd) SetWindowLongPtr(hwnd, GWL_STYLE, WindowStyle);
	};
// --------------------------------------------------------------------
	void SetSize(int Width, int Height) {
		this->Width = Width;
		this->Height = Height;
		if (hwnd) SetWindowPos(hwnd, nullptr, 0, 0, Width, Height, SWP_NOMOVE | SWP_NOZORDER);
	};
// --------------------------------------------------------------------
	void SetPos(int X, int Y) {
		this->x = X;
		this->y = Y;
		if (hwnd) SetWindowPos(hwnd, nullptr, X, Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	};
// --------------------------------------------------------------------
	void SetName(str WindowName) {
		this->WindowName = WindowName;
		if (hwnd) SetWindowTextA(hwnd, WindowName);
	};
// ---------------------- Window Functions ----------------------------
	void Show(int type = SW_SHOW) { ShowWindow(hwnd, type); };
	void Hide() { Show(SW_HIDE); };
	void Update() { InvalidateRect(hwnd, nullptr, false); }
	void MsgBox(str text, str caption, uint32 type = MB_OK) { MessageBoxA(hwnd, text, caption, type); };
// --------------------------------------------------------------------

	TWindow() {};
	TWindow(str WindowName) { SetName(WindowName); };
	TWindow(str WindowName, int Width, int Height) { SetName(WindowName); SetSize(Width, Height); };

private:

	void OnPaint();
	void Resize();
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	friend class Application;
	friend class Renderer;
};

////////////////////////
// Renderer Base Class//
////////////////////////

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

/////////////////////////
// TControl Base Class //
/////////////////////////

class TControl {
protected:
	TWindow* owner;
public:
	int x = 0;
	int y = 0;
	int sizeX = 0;
	int sizeY = 0;

	TControl(TWindow* owner) { this->owner = owner; };
	TWindow* GetWindow() { return this->owner; };
	bool InBounds(int x, int y) { return IN_BOUNDS(x, y, this->x, this->y, this->sizeX, this->sizeY); }

	virtual void Draw(Renderer* renderer) = 0;
	virtual void UserInputEvent(InputType inputType, ulong64 param) {};

	friend class TWindow;
};

////////////////
// Subclasses //
////////////////
#include "vclrenderer.h"
#include "vclcontrols.h"

/////////////////
// Application //
/////////////////

class Application {
// ------------------------ VARIABLES ---------------------------------
private:
	static str ClassName;
public:
// ------------------------ FUNCTIONS ---------------------------------
private:
// --------------------------------------------------------------------
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		TWindow* pThis = 0;

		if (uMsg == WM_NCCREATE) {
			pThis = (TWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
			pThis->hwnd = hwnd;
			SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
		} else pThis = (TWindow*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

		if (pThis) return pThis->HandleMessage(uMsg, wParam, lParam);
		else return DefWindowProcA(hwnd, uMsg, wParam, lParam);
	};
// --------------------------------------------------------------------
public:
// --------------------------------------------------------------------
	static void Initialize() {
		// register OpenVCLWindow
		WNDCLASS wc = { 0 };
		wc.lpfnWndProc = Application::WindowProc;
		wc.hInstance = GetModuleHandleA(0);
		wc.hCursor = LoadCursorA(0, IDC_ARROW);
		wc.lpszClassName = ClassName;
		RegisterClassA(&wc);
	};
// --------------------------------------------------------------------
	static void NewWindow(TWindow* w) {
		CreateWindowExA(
			w->ExtendedWindowStyle,
			ClassName,
			w->WindowName,
			w->WindowStyle,
			w->x, w->y,
			w->Width,
			w->Height,
			nullptr, nullptr,
			GetModuleHandleA(nullptr), w
		);
		w->Show();
	};
// --------------------------------------------------------------------
	static void Run(TWindow* window) {
		NewWindow(window);
		// main program loop
		MSG msg = { 0 };
		while (GetMessageA(&msg, 0, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	};
// --------------------------------------------------------------------
};

#ifdef OPENVCL_APP

str Application::ClassName = "OpenVCLWindow";

void TWindow::OnPaint() {
	if (r->CreateTarget()) {
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);
		r->BeginDraw();

		r->Clear(background);

		// Draw all Controls. draw in reverse order so element 0 has the highest z-order
		for (size_t i = controls.length(); 0 < i; i--)
			controls.get(i - 1)->Draw(r);

		// End Scene
		if (!r->EndDraw()) r->DiscardTarget();
		EndPaint(hwnd, &ps);
		}
	};

void TWindow::Resize() {
	RECT rc;
	GetClientRect(hwnd, &rc);
	r->Resize();
	this->Width = rc.right;
	this->Height = rc.bottom;
	Update();
};

LRESULT TWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// Window Message Handler
	switch (uMsg) {
	case WM_CREATE:
		// create Renderer
		r = new RendererWin(this);
		CONSOLE()

			return 0;

	case WM_DESTROY:
		r->DiscardTarget();
		r->Destroy();
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		OnPaint();
		return 0;

	case WM_SIZE:
		Resize();
		return 0;

	case WM_MOUSEMOVE:

		lastMouseX = LOWORD(lParam);
		lastMouseY = HIWORD(lParam);
		Update();
		return 0;

	case WM_LBUTTONDOWN:

		for (size_t i = 0; i < controls.length(); i++) {
			if (controls.get(i)->InBounds(LOWORD(lParam), HIWORD(lParam))) {
				controls.get(i)->UserInputEvent(InputType::MouseDown, 0);
				break;
			}
		}

		Update();
		return 0;

	case WM_LBUTTONUP:
		for (size_t i = 0; i < controls.length(); i++)
			controls.get(i)->UserInputEvent(InputType::MouseUp, 0);
		Update();
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
};

#endif

#endif