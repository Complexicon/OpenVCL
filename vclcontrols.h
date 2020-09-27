#pragma once

class TButton : public TControl {
private:
	bool clicked = false;
public:
	TButton(TWindow* owner, int x, int y, str label);
	void Draw(Renderer* renderer) override;
	void UserInputEvent(InputType inputType, ulong64 param) override;

	void (*OnClick)(TControl* Sender) = 0;

	str label = "Button";
};

class TLabel : public TControl {
public:
	TLabel(TWindow* owner, int x, int y, int width, int height, str label);
	void Draw(Renderer* renderer) override;

	str label = "Label";
};