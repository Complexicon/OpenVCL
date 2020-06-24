#include "OpenVCL.h"

#pragma region Getter

HWND TWindow::Window() { return m_hwnd; }
int TWindow::GetWidth() { return Width; }
int TWindow::GetHeight() { return Height; }
int TWindow::GetWindowX() { return x; }
int TWindow::GetWindowY() { return y; }
PCWSTR TWindow::GetWindowName() { return WindowName; }
DWORD TWindow::GetCurrentStyle() { return WindowStyle; }

#pragma endregion

#pragma region Setter

void TWindow::SetStyle(DWORD WindowStyle) {
	this->WindowStyle = WindowStyle;
	if (m_hwnd) SetWindowLongPtr(m_hwnd, GWL_STYLE, WindowStyle);
}

void TWindow::SetSize(int Width, int Height) {
	this->Width = Width;
	this->Height = Height;
	if (m_hwnd) SetWindowPos(m_hwnd, nullptr, 0, 0, Width, Height, SWP_NOMOVE | SWP_NOZORDER);
}

void TWindow::SetPos(int X, int Y) {
	this->x = X;
	this->y = Y;
	if (m_hwnd) SetWindowPos(m_hwnd, nullptr, X, Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void TWindow::SetName(PCWSTR WindowName) {
	this->WindowName = WindowName;
	if (m_hwnd) SetWindowText(m_hwnd, WindowName);
}

#pragma endregion

#pragma region WindowFunctions

void TWindow::Show(int type) { ShowWindow(Window(), type); };
void TWindow::Hide() { Show(SW_HIDE); }

#pragma endregion

#pragma region InternalMagic

HRESULT TWindow::CreateGraphicsResources() {
	HRESULT hr = S_OK;
	if (pRenderTarget == NULL) {
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		//Create all graphic resources

		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, { (UINT32)rc.right, (UINT32)rc.bottom }),
			&pRenderTarget);

		// TODO: relocate directwrite

		if (SUCCEEDED(hr))
			hr = pDWriteFactory->CreateTextFormat(
				L"Verdana", // clean ui font imo
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				12,
				L"", //locale
				&pTextFormat
			);

		if (SUCCEEDED(hr))
			hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0,0,0), &pBrush);

		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	}

	return hr;
}

void TWindow::DiscardGraphicsResources() {
	// release d2d
	pRenderTarget->Release();
	pBrush->Release();
	// release directwrite

}

// VERY BAD DEBUG CODE
void TWindow::OnPaint() {
	HRESULT hr = CreateGraphicsResources();
	if (SUCCEEDED(hr)) {
		PAINTSTRUCT ps;
		BeginPaint(m_hwnd, &ps);

		pRenderTarget->BeginDraw();

		pRenderTarget->Clear(D2D1::ColorF(0.1, 0.1, 0.1));

		int x = 50;
		int y = 50;
		int sizeX = 96;
		int sizeY = 32;

		D2D1_RECT_F rct;
		rct.top = y;
		rct.bottom = y + sizeY;
		rct.left = x;
		rct.right = x + sizeX;

		D2D1_ROUNDED_RECT r;
		r.radiusX = r.radiusY = sizeY / 2;
		r.rect = rct;

		// Intersect Test Begin
		// Check if mouse is in Rect Bounding Box

		bool isInRectBounds = testX > rct.left && testX < rct.right&& testY > rct.top && testY < rct.bottom;

		if (isInRectBounds) {
			// Green Base if mbutton left down
			if(mb_down) {
				pBrush->SetColor(D2D1::ColorF(0.15, 0.15, 0.15));
				pRenderTarget->FillRoundedRectangle(r, pBrush);
				pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
				// Set the ButtonControl pressed state; last state is used in WM_LBUTTONUP.
				buttonPressed = true;
			}
			else {
				pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
				buttonPressed = false;
			}
		} else {
			pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));
			buttonPressed = false;
		}

		// END

		// TODO: Optimize a lot.

		pRenderTarget->DrawRoundedRectangle(&r, pBrush, 2);
		// TODO-FIX: inline stringlength   v
		pRenderTarget->DrawText(L"Button", 6, pTextFormat, rct, pBrush);

		// DEBUG DRAWINGS
		pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow, 0.5));
		pRenderTarget->FillRectangle({ (float)testX -5, (float)testY-5, (float)testX + 5, (float)testY + 5 }, pBrush);

		// Debug Button OnClick Event
		if (executeEvent)
			pRenderTarget->DrawText(L"Debug Buttonpress Triggered", 27, pTextFormat, D2D1::RectF(0,0,200,50), pBrush);

		// End Scene

		hr = pRenderTarget->EndDraw();

		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) DiscardGraphicsResources();

		EndPaint(m_hwnd, &ps);
	}
}

void TWindow::Resize() {
	// Resize Direct2D Clientarea
	if (FAILED(CreateGraphicsResources())) return;
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	pRenderTarget->Resize({ (UINT32)rc.right, (UINT32)rc.bottom });
	InvalidateRect(m_hwnd, nullptr, false);
}

LRESULT TWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// Window Message Handler
	switch (uMsg) {
	case WM_CREATE:
		// create d2d & directwrite factories
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))) return -1;
		if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory)))) return -1;
		return 0;

	case WM_DESTROY:
		DiscardGraphicsResources();
		pDWriteFactory->Release();
		pFactory->Release();
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		OnPaint();
		return 0;

	case WM_SIZE:
		Resize();
		return 0;

	case WM_MOUSEMOVE:
		testX = lParam & 0xffff;
		testY = (lParam >> 16) & 0xffff;
		InvalidateRect(m_hwnd, NULL, FALSE);
		return 0;
	case WM_LBUTTONDOWN:
		mb_down = true;
		InvalidateRect(m_hwnd, NULL, FALSE);
		return 0;

	case WM_LBUTTONUP:
		mb_down = false;
		// Debug Button Presses
		// TODO: Create Classes for Controls.
		if (buttonPressed) {
			executeEvent = !executeEvent;
			buttonPressed = false;
		}
		InvalidateRect(m_hwnd, NULL, FALSE);
		return 0;
	}

	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK TWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	TWindow* pThis = NULL;

	if (uMsg == WM_NCCREATE) {
		pThis = (TWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		pThis->m_hwnd = hwnd;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
	} else pThis = (TWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (pThis) return pThis->HandleMessage(uMsg, wParam, lParam);
	else return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#pragma endregion