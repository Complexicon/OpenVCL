#include "OpenVCL.h"

void TButton::Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, IDWriteTextFormat* pTextFormat) {

	D2D1_ROUNDED_RECT r = {boundingBox, radius, radius };

	if (InBounds(owner->GetLastMouseX(), owner->GetLastMouseY())) {
		if (clicked) {
			pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White, 0.1));
			pRenderTarget->FillRoundedRectangle(r, pBrush);
		}
		pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	} else 
		pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));

	pRenderTarget->DrawRoundedRectangle(r, pBrush, 2);
	pRenderTarget->DrawText(label, wcslen(label), pTextFormat, boundingBox, pBrush);
}

void TButton::MD() {
	clicked = true;
}

void TButton::MU() {
	if (clicked && InBounds(owner->GetLastMouseX(), owner->GetLastMouseY()) && OnClick != nullptr)
		OnClick(this);
	clicked = false;
}

TButton::TButton(TWindow* owner, int x, int y, PCWSTR label) : TControl(owner) {

	this->label = label;
	this->x = x;
	this->y = y;
	sizeX = 60;
	sizeY = 24;

	radius = sizeY / 2;

	UpdateBoundingBox();
}
