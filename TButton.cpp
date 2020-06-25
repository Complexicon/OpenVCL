#include "OpenVCL.h"

void TButton::Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, IDWriteTextFormat* pTextFormat) {

	D2D1_ROUNDED_RECT r;
	r.radiusX = r.radiusY = sizeY / 2;
	r.rect = boundingBox;

	/*
	
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
	
	*/

	if (InBounds(owner->GetLastMouseX(), owner->GetLastMouseY())) {
		if (clicked) {
			pBrush->SetColor(D2D1::ColorF(0.15, 0.15, 0.15));
			pRenderTarget->FillRoundedRectangle(r, pBrush);
		}
		pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	} else 
		pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));

	pRenderTarget->DrawRoundedRectangle(r, pBrush, 2);
	// TODO-FIX: inline stringlength   v
	pRenderTarget->DrawText(L"Button", 6, pTextFormat, boundingBox, pBrush);
}

void TButton::OnClick(bool release) {
	clicked = !release;
}

TButton::TButton(TWindow* owner, int x, int y) : TControl(owner) {

	this->x = x;
	this->y = y;
	sizeX = 96;
	sizeY = 32;

	boundingBox.left = x;
	boundingBox.right = x + sizeX;
	boundingBox.top = y;
	boundingBox.bottom = y + sizeY;
}
