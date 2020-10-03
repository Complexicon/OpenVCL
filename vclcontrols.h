#pragma once

// If defined no Controls will Compiled into the App
#ifndef COMPILE_NO_CONTROLS

class TButton : public TControl {
private:
	bool clicked = false;

public:
	void (*OnClick)(TControl* Sender) = 0;
	str label = "Button";

private:

public:

	TButton(TWindow* owner, int x, int y, str label) : TControl(owner) {
		this->label = label;
		this->x = x;
		this->y = y;
		sizeX = 60;
		sizeY = 24;
	};

	void Draw(Renderer* renderer) {
		if (InBounds(owner->GetLastMouseX(), owner->GetLastMouseY())) {
			if (clicked)
				renderer->FillRoundedRect(x, y, sizeX, sizeY, Color(0xffffff, 25));
			renderer->DrawRoundedRect(x, y, sizeX, sizeY, Color(0xffffff));
		} else
			renderer->DrawRoundedRect(x, y, sizeX, sizeY, Color(0x808080));

		renderer->DrawString(x, y, sizeX, sizeY, Color(0xffffff), label);
	};

	void UserInputEvent(InputType inputType, ulong64 param) {
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

	};
};

class TLabel : public TControl {
public:
	str label = "Label";

public:

	TLabel(TWindow* owner, int x, int y, int width, int height, str label) : TControl(owner) {
		this->x = x;
		this->y = y;
		this->sizeX = width;
		this->sizeY = height;
		this->label = label;
	};

	void Draw(Renderer* renderer) {
		renderer->DrawString(x, y, sizeX, sizeY, Color(0xffffff), label);
	};


};

#endif