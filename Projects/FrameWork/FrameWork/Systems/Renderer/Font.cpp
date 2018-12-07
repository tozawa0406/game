#include "Font.h"
#include <FrameWork/Graphics/DirectX11/DirectX11.h>
#include <Shlwapi.h>

static const string filename = "Resource/Data/ProggyClean.ttf";

Font::Font(void) : directX11_(nullptr)
{
}

Font::~Font(void)
{
}

HRESULT Font::Init(DirectX11* directX11)
{
	if (!directX11) { return E_FAIL; }
	directX11_ = directX11;

	// フォントを使えるようにする
	DESIGNVECTOR design;
	AddFontResourceEx(TEXT(filename.c_str()), FR_PRIVATE, &design);

	return S_OK;
}

HRESULT Font::CreateStringData(const string& fonts, int fontSize)
{
	int max = fonts.size();
	bool next = false;
	for (int i = 0; i < max; ++i)
	{
		if (next) { next = false; continue; }

		auto& f = fonts[i];
		if (('a' <= f && f <= 'z') || ('A' <= f && f <= 'Z'))
		{
			string s = { f };
			if (font_.count(s) != 0) { continue; }
			TCHAR temp[2] = { f, 0 };
			if (FAILED(CreateFontData(temp, fontSize))) { return E_FAIL; }
		}
		else
		{
			if (i + 1 >= max) { continue; }
			string s = { f, fonts[i + 1] };
			if (font_.count(s) != 0) { next = true; continue; }

			TCHAR temp[3] = { f, fonts[i + 1], 0 };
			if (FAILED(CreateFontData(temp, fontSize))) { return E_FAIL; }
			next = true;
		}
	}
	return S_OK;
}

HRESULT Font::CreateFontData(const TCHAR* font, int fontSize)
{
	const auto& window = directX11_->GetWindow();
	if (!window) { return E_FAIL; }

	BITMAPFONTDATA fontData;
	HRESULT hr = CreateBitMapFontData(fontData, font, fontSize);

	const auto& device  = directX11_->GetDevice();
	const auto& context = directX11_->GetDeviceContext();

	// CPUで書き込みができるテクスチャを作成
	// テクスチャ作成
	D3D11_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.Width				= fontData.GM.gmCellIncX;
	desc.Height				= fontData.TM.tmHeight;
	desc.MipLevels			= 1;
	desc.ArraySize			= 1;
	desc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;	// RGBA(255,255,255,255)タイプ
	desc.SampleDesc.Count	= 1;
	desc.Usage				= D3D11_USAGE_DYNAMIC;			// 動的（書き込みするための必須条件）
	desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;	// シェーダリソースとして使う
	desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;		// CPUからアクセスして書き込みOK

	ID3D11Texture2D* tex2D;
	hr = device->CreateTexture2D(&desc, 0, &tex2D);
	string temp = "のフォントテクスチャの作成に失敗しました。";
	if (window->ErrorMessage(string(font + temp).c_str(), "エラー", hr)) { return E_FAIL; }

	D3D11_MAPPED_SUBRESOURCE hMappedResource;
	hr = context->Map(tex2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &hMappedResource);
	temp = "のフォントテクスチャの書き込みに失敗しました。";
	if (window->ErrorMessage(string(font + temp).c_str(), "エラー", hr)) { return E_FAIL; }

	// ここで書き込む
	BYTE* pBits = (BYTE*)hMappedResource.pData;

	// フォント情報の書き込み
	// iOfs_x, iOfs_y : 書き出し位置(左上)
	// iBmp_w, iBmp_h : フォントビットマップの幅高
	// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
	int iOfs_x = fontData.GM.gmptGlyphOrigin.x;
	int iOfs_y = fontData.TM.tmAscent - fontData.GM.gmptGlyphOrigin.y;
	int iBmp_w = fontData.GM.gmBlackBoxX + (4 - (fontData.GM.gmBlackBoxX % 4)) % 4;
	int iBmp_h = fontData.GM.gmBlackBoxY;
	int Level = 17;
	int x, y;
	DWORD Alpha, Color;
	memset(pBits, 0, hMappedResource.RowPitch * fontData.TM.tmHeight);
	for (y = iOfs_y; y < iOfs_y + iBmp_h; ++y)
	{
		for (x = iOfs_x; x < iOfs_x + iBmp_w; ++x)
		{
			Alpha = (255 * fontData.ptr[x - iOfs_x + iBmp_w * (y - iOfs_y)]) / (Level - 1);
			Color = 0x00ffffff | (Alpha << 24);

			memcpy((BYTE*)pBits + hMappedResource.RowPitch * y + 4 * x, &Color, sizeof(DWORD));
		}
	}
	context->Unmap(tex2D, 0);
	delete[] fontData.ptr;

	// ShaderResourceViewの情報を作成する
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format						= desc.Format;
	srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip	= 0;
	srvDesc.Texture2D.MipLevels			= desc.MipLevels;

	// ShaderResourceViewを作成する
	ID3D11ShaderResourceView* shaderResourceView;
	hr = device->CreateShaderResourceView(tex2D, &srvDesc, &shaderResourceView);
	temp = "のシェーダーリソースビューの作成に失敗しました。";
	if (window->ErrorMessage(string(font + temp).c_str(), "エラー", hr)) { return E_FAIL; }

	font_[font] = shaderResourceView;

	return S_OK;
}

HRESULT Font::CreateBitMapFontData(BITMAPFONTDATA& fontData, const TCHAR* font, int fontSize)
{
	const auto& window = directX11_->GetWindow();
	if (!window) { return E_FAIL; }

	// フォントの生成
	LOGFONT lf = { fontSize,0,0,0,0,0,0,0,SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,FIXED_PITCH | FF_MODERN, TEXT("あんずもじ湛") };
	HFONT	hFont = CreateFontIndirect(&lf);

	if (!hFont)
	{
		window->ErrorMessage("フォントの作成に失敗しました。", "エラー", E_FAIL);
		return E_FAIL;
	}

	// デバイスコンテキスト取得
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	HDC		hdc = GetDC(NULL);
	HFONT	oldFont = (HFONT)SelectObject(hdc, hFont);

	// 文字コード取得
	UINT code = 0;
#if _UNICODE
	// unicodeの場合、文字コードは単純にワイド文字のUINT変換です
	code = (UINT)*c;
#else
	// マルチバイト文字の場合、
	// 1バイト文字のコードは1バイト目のUINT変換、
	// 2バイト文字のコードは[先導コード]*256 + [文字コード]です
	if (IsDBCSLeadByte(*font))
	{
		code = (BYTE)font[0] << 8 | (BYTE)font[1];
	}
	else
	{
		code = font[0];
	}
#endif

	// フォントビットマップ取得
	GetTextMetrics(hdc, &fontData.TM);
	const MAT2	Mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
	DWORD		size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &fontData.GM, 0, NULL, &Mat);
	fontData.ptr = new BYTE[size];
	GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &fontData.GM, size, fontData.ptr, &Mat);

	// デバイスコンテキストとフォントハンドルの開放
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	return S_OK;
}

void Font::Uninit(void)
{
	for (auto& f : font_)
	{
		if (f.second)
		{
			ReleasePtr(f.second);
		}
	}

	// リソース削除
	DESIGNVECTOR design;
	RemoveFontResourceEx(TEXT(filename.c_str()), FR_PRIVATE, &design);
}

void Font::DrawFont(const string& font, VECTOR2 position, VECTOR2 size, COLOR color)
{
	if (font_.count(font) == 0) { return; }

	const auto& context = directX11_->GetDeviceContext();
	const auto& wrapper = directX11_->GetWrapper();

	context->PSSetShaderResources(0, 1, &font_[font]);
	wrapper->DrawQuad(position, size, color);
}

void Font::Draw(const string& fonts, VECTOR2 position, VECTOR2 size, COLOR color)
{
	int max = fonts.size();

	float startX = position.x - (Half(size.x) * Half(max - 2));

	bool draw = false;
	for(int i = 0;i < max;++i)
	{
		if (draw) { draw = false; continue; }

		auto& f = fonts[i];
		if (('a' <= f && f <= 'z') || ('A' <= f && f <= 'Z'))
		{
			string temp = { f };
			DrawFont(temp, VECTOR2(startX + (Half(size.x) * i) - Quarter(size.x), position.y), VECTOR2(Half(size.x), size.y), color);
		}
		else
		{
			if (i + 1 >= max) { continue; }
			string temp = { fonts[i], fonts[i + 1] };
			DrawFont(temp, VECTOR2(startX + (Half(size.x) * i), position.y), size, color);
			draw = true;
		}
	}
}
