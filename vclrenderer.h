#pragma once

class RendererWin : public Renderer {
public:
	RendererWin(TWindow* client) : Renderer(client) { Setup(); }
	void DrawLine(int x0, int y0, int x1, int y1, const Color& color, float w = 1.f) override;
	void DrawRoundedRect(int x, int y, int width, int height, const Color& color, float radius = 1.f) override;
	void DrawString(int x, int y, int width, int height, const Color& color, str text) override;

	void FillRoundedRect(int x, int y, int width, int height, const Color& color, float radius = 1.f) override;
private:

	ID2D1Factory* pFactory = 0;
	ID2D1HwndRenderTarget* pRenderTarget = 0;
	ID2D1SolidColorBrush* pBrush = 0;
	IDWriteTextFormat* pTextFormat = 0;
	IDWriteFactory* pDWriteFactory = 0;

	bool Setup() override;
	bool CreateTarget() override;
	void DiscardTarget() override;
	void Destroy() override;
	void BeginDraw() override;
	bool EndDraw() override;
	void Resize() override;
	void Clear(const Color& color) override;
};