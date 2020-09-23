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

	const size_t cSize = strlen(label) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, label, cSize);

	pRenderTarget->DrawTextA(wc, cSize, pTextFormat, boundingBox, pBrush);
}

void TButton::UserInputEvent(InputType inputType, ulong64 param) {
	switch (inputType) {
	case InputType::Mouse:
		if (param == 1) clicked = true;
		else {
			if (clicked && InBounds(owner->GetLastMouseX(), owner->GetLastMouseY()) && OnClick != nullptr)
				OnClick(this);
			clicked = false;
		}
		break;
	}

}

TButton::TButton(TWindow* owner, int x, int y, str label) : TControl(owner) {

	this->label = label;
	this->x = x;
	this->y = y;
	sizeX = 60;
	sizeY = 24;

	radius = sizeY / 2;

	UpdateBoundingBox();
}
