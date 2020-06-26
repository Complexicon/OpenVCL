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

		pRenderTarget->Clear(background);

		// Draw all Controls. draw in reverse order
		for (size_t i = controls.length(); 0 < i; i--)
			controls.get(i - 1)->Draw(pRenderTarget, pBrush, pTextFormat);


#ifdef _DEBUG_OVCL
		// DEBUG DRAWINGS
		pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow, 0.5));
		pRenderTarget->FillRectangle({ (float)lastMouseX -5, (float)lastMouseY-5, (float)lastMouseX + 5, (float)lastMouseY + 5 }, pBrush);

		// Draw Controls Bounding Boxes
		for (size_t i = 0; i < controls.length(); i++)
			pRenderTarget->DrawRectangle(controls.get(i)->boundingBox, pBrush);

		// END DEBUG DRAWINGS
#endif


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
	Update();
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
		lastMouseX = LOWORD(lParam);
		lastMouseY = HIWORD(lParam);
		Update();
		return 0;

	case WM_LBUTTONDOWN:
		for (size_t i = 0; i < controls.length(); i++) {
			if (controls.get(i)->InBounds(LOWORD(lParam), HIWORD(lParam))) {
				controls.get(i)->MD();
				break;
			}
		}
		Update();
		return 0;

	case WM_LBUTTONUP:
		for (size_t i = 0; i < controls.length(); i++)
			controls.get(i)->MU();
		Update();
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