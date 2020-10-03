#include "OpenVCL.h"

#ifdef _DEBUG_OVCL
#include "Helper/console.h"
#endif

#pragma region Getter



#pragma endregion

#pragma region Setter



#pragma endregion

#pragma region WindowFunctions

void TWindow::Show(int type) { ShowWindow(Window(), type); };
void TWindow::Hide() { Show(SW_HIDE); }

#pragma endregion

#pragma region InternalMagic

// VERY BAD DEBUG CODE
void TWindow::OnPaint() {
	if (r->CreateTarget()) {
		PAINTSTRUCT ps;
		BeginPaint(m_hwnd, &ps);

		// dirty dirty testing code

		r->BeginDraw();

		r->Clear(background);


		// Draw all Controls. draw in reverse order so element 0 has the highest z-order
		for (size_t i = controls.length(); 0 < i; i--)
			controls.get(i - 1)->Draw(r);

#ifdef _DEBUG_OVCL

		Console::Log("update frame");
		Console::Log(this->GetWidth());
		Console::Log(this->GetHeight());

		// DEBUG DRAWINGS
		pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow, 0.5));
		pRenderTarget->FillRectangle({ (float)lastMouseX -5, (float)lastMouseY-5, (float)lastMouseX + 5, (float)lastMouseY + 5 }, pBrush);

		// Draw Controls Bounding Boxes
		for (size_t i = 0; i < controls.length(); i++)
			pRenderTarget->DrawRectangle(controls.get(i)->boundingBox, pBrush);

		// END DEBUG DRAWINGS
#endif


		// End Scene

		if (!r->EndDraw()) r->DiscardTarget();

		EndPaint(m_hwnd, &ps);
	}
}

void TWindow::Resize() {
	// Resize Direct2D Clientarea
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	r->Resize();
	this->Width = rc.right;
	this->Height = rc.bottom;
	Update();
}

LRESULT TWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// Window Message Handler
	switch (uMsg) {
	case WM_CREATE:
		// create Renderer
		r = new RendererWin(this);
		// DEBUG CONSOLE
#ifdef _DEBUG_OVCL
		Console::Alloc();
#endif

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
