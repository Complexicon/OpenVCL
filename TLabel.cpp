#include "OpenVCL.h"

TLabel::TLabel(TWindow* owner, int x, int y, int width, int height, str label) : TControl(owner) {
	this->x = x;
	this->y = y;
	this->sizeX = width;
	this->sizeY = height;
	this->label = label;
}

void TLabel::Draw(Renderer* renderer) {
	renderer->DrawString(x, y, sizeX, sizeY, Color(0xffffff), label);
}
