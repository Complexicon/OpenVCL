#include "OpenVCL.h"

void TButton::Draw(Renderer* renderer) {
	if (InBounds(owner->GetLastMouseX(), owner->GetLastMouseY())) {
		if (clicked)
			renderer->FillRoundedRect(x, y, sizeX, sizeY, Color(0xffffff, 25));
		renderer->DrawRoundedRect(x, y, sizeX, sizeY, Color(0xffffff));
	} else 
		renderer->DrawRoundedRect(x, y, sizeX, sizeY, Color(0x808080));

	renderer->DrawString(x, y, sizeX, sizeY, Color(0xffffff), label);
}

void TButton::UserInputEvent(InputType inputType, ulong64) {
	switch (inputType) {
	case InputType::MouseDown:
		clicked = true;
		break;
	case InputType::MouseUp:
		if (clicked && InBounds(owner->GetLastMouseX(), owner->GetLastMouseY()) && OnClick != nullptr)
			OnClick(this);
		clicked = false;
		break;
	}

}

TButton::TButton(TWindow* owner, int x, int y, str label) : TControl(owner) {
	this->label = label;
	this->x = x;
	this->y = y;
	sizeX = 60;
	sizeY = 24;
}
