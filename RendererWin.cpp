#include "OpenVCL.h"

#define TO_BOUNDINGBOX(x,y,width,height) {(float)x,(float)y,(float)x+width,(float)y+height}
#define TO_POINT(x,y) {(float)x,(float)y}
#define TO_RGBFLOAT(color) { BYTE_TO_FLOAT(color.r),BYTE_TO_FLOAT(color.g) ,BYTE_TO_FLOAT(color.b) ,BYTE_TO_FLOAT(color.a) }

void RendererWin::DrawLine(int x0, int y0, int x1, int y1, const Color& color, float w) {
	pBrush->SetColor(TO_RGBFLOAT(color));
	pRenderTarget->DrawLine(TO_POINT(x0,y0), TO_POINT(x1, y1), pBrush, w);
}

void RendererWin::DrawRoundedRect(int x, int y, int width, int height, const Color& color, float radius) {
	pBrush->SetColor(TO_RGBFLOAT(color));
	radius = ((height / 2) * radius);
	pRenderTarget->DrawRoundedRectangle({ TO_BOUNDINGBOX(x,y,width,height), radius, radius }, pBrush);
}

void RendererWin::DrawString(int x, int y, int width, int height, const Color& color, str text) {
	pBrush->SetColor(TO_RGBFLOAT(color));
	const size_t cSize = strlen(text) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, text, cSize);

	pRenderTarget->DrawTextA(wc, cSize, pTextFormat, TO_BOUNDINGBOX(x, y, width, height), pBrush);
	delete wc;
}

void RendererWin::FillRoundedRect(int x, int y, int width, int height, const Color& color, float radius) {
	pBrush->SetColor(TO_RGBFLOAT(color));
	radius = ((height / 2) * radius);
	pRenderTarget->FillRoundedRectangle({ TO_BOUNDINGBOX(x,y,width,height), radius, radius }, pBrush);
}

bool RendererWin::Setup() {

	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))) return false;
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory)))) return false;
	return true;

}

bool RendererWin::CreateTarget() {
	HRESULT hr = S_OK;
	if (pRenderTarget == NULL) {
		RECT rc;
		GetClientRect(client->Window(), &rc);

		//Create all graphic resources

		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(client->Window(), { (UINT32)rc.right, (UINT32)rc.bottom }),
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
			hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &pBrush);

		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	}

	return SUCCEEDED(hr);
}

void RendererWin::DiscardTarget() {
	// release d2d
	pRenderTarget->Release();
	pBrush->Release();
	pRenderTarget = 0;
	pBrush = 0;
}

void RendererWin::Destroy() {
	pDWriteFactory->Release();
	pFactory->Release();
}

void RendererWin::BeginDraw() {
	pRenderTarget->BeginDraw();
}

bool RendererWin::EndDraw() {
	HRESULT res = pRenderTarget->EndDraw();
	return res == D2DERR_RECREATE_TARGET ? false : SUCCEEDED(res);
}

void RendererWin::Resize() {
	if (!CreateTarget()) return;
	RECT rc;
	GetClientRect(client->Window(), &rc);
	pRenderTarget->Resize({ (uint32)rc.right, (uint32)rc.bottom });
}

void RendererWin::Clear(const Color& color) {
	pRenderTarget->Clear({BYTE_TO_FLOAT(color.r),BYTE_TO_FLOAT(color.g) ,BYTE_TO_FLOAT(color.b) ,BYTE_TO_FLOAT(color.a) });
}
