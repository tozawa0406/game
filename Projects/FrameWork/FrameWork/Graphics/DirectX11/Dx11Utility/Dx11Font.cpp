#include "Dx11Font.h"
#include "../DirectX11.h"
#include <Shlwapi.h>

static const string filename = "Resource/Data/ProggyClean.ttf";

Dx11Font::Dx11Font(void)
{
}

Dx11Font::~Dx11Font(void)
{
}

HRESULT Dx11Font::Init(Graphics* graphics)
{
	if (!graphics) { return E_FAIL; }
	graphics_ = graphics;

	// �t�H���g���g����悤�ɂ���
	DESIGNVECTOR design;
	AddFontResourceEx(TEXT(filename.c_str()), FR_PRIVATE, &design);

	return S_OK;
}

HRESULT Dx11Font::CreateFontData(const char* font, int fontSize)
{
	if (font_.count(font) != 0) { return E_ABORT; }

	const auto& window = graphics_->GetWindow();
	if (!window) { return E_FAIL; }

	BITMAPFONTDATA fontData;
	HRESULT hr = CreateBitMapFontData(fontData, font, fontSize);

	const auto& directX11 = static_cast<DirectX11*>(graphics_);
	const auto& device  = directX11->GetDevice();
	const auto& context = directX11->GetDeviceContext();

	// CPU�ŏ������݂��ł���e�N�X�`�����쐬
	// �e�N�X�`���쐬
	D3D11_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.Width				= fontData.GM.gmCellIncX;
	desc.Height				= fontData.TM.tmHeight;
	desc.MipLevels			= 1;
	desc.ArraySize			= 1;
	desc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;	// RGBA(255,255,255,255)�^�C�v
	desc.SampleDesc.Count	= 1;
	desc.Usage				= D3D11_USAGE_DYNAMIC;			// ���I�i�������݂��邽�߂̕K�{�����j
	desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;	// �V�F�[�_���\�[�X�Ƃ��Ďg��
	desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;		// CPU����A�N�Z�X���ď�������OK

	ID3D11Texture2D* tex2D;
	hr = device->CreateTexture2D(&desc, 0, &tex2D);
	string temp = "�̃t�H���g�e�N�X�`���̍쐬�Ɏ��s���܂����B";
	if (window->ErrorMessage(string(font + temp).c_str(), "�G���[", hr)) { return E_FAIL; }

	D3D11_MAPPED_SUBRESOURCE hMappedResource;
	hr = context->Map(tex2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &hMappedResource);
	temp = "�̃t�H���g�e�N�X�`���̏������݂Ɏ��s���܂����B";
	if (window->ErrorMessage(string(font + temp).c_str(), "�G���[", hr)) { return E_FAIL; }

	// �����ŏ�������
	byte* pBits = (byte*)hMappedResource.pData;

	// �t�H���g���̏�������
	// iOfs_x, iOfs_y : �����o���ʒu(����)
	// iBmp_w, iBmp_h : �t�H���g�r�b�g�}�b�v�̕���
	// Level : ���l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K)
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

			memcpy((byte*)pBits + hMappedResource.RowPitch * y + 4 * x, &Color, sizeof(DWORD));
		}
	}
	context->Unmap(tex2D, 0);
	delete[] fontData.ptr;

	// ShaderResourceView�̏����쐬����
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format						= desc.Format;
	srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip	= 0;
	srvDesc.Texture2D.MipLevels			= desc.MipLevels;

	// ShaderResourceView���쐬����
	ID3D11ShaderResourceView* shaderResourceView;
	hr = device->CreateShaderResourceView(tex2D, &srvDesc, &shaderResourceView);
	temp = "�̃V�F�[�_�[���\�[�X�r���[�̍쐬�Ɏ��s���܂����B";
	if (window->ErrorMessage(string(font + temp).c_str(), "�G���[", hr)) { return E_FAIL; }

	font_[font] = shaderResourceView;

	return S_OK;
}

HRESULT Dx11Font::CreateBitMapFontData(BITMAPFONTDATA& fontData, const char* font, int fontSize)
{
	const auto& window = graphics_->GetWindow();
	if (!window) { return E_FAIL; }

	// �t�H���g�̐���
	LOGFONT lf = { fontSize,0,0,0,0,0,0,0,SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,FIXED_PITCH | FF_MODERN, TEXT("���񂸂����X") };
	HFONT	hFont = CreateFontIndirect(&lf);

	if (!hFont)
	{
		window->ErrorMessage("�t�H���g�̍쐬�Ɏ��s���܂����B", "�G���[", E_FAIL);
		return E_FAIL;
	}

	// �f�o�C�X�R���e�L�X�g�擾
	// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
	HDC		hdc = GetDC(NULL);
	HFONT	oldFont = (HFONT)SelectObject(hdc, hFont);

	// �����R�[�h�擾
	uint code = 0;
#if _UNICODE
	// unicode�̏ꍇ�A�����R�[�h�͒P���Ƀ��C�h������uint�ϊ��ł�
	code = (uint)*c;
#else
	// �}���`�o�C�g�����̏ꍇ�A
	// 1�o�C�g�����̃R�[�h��1�o�C�g�ڂ�uint�ϊ��A
	// 2�o�C�g�����̃R�[�h��[�擱�R�[�h]*256 + [�����R�[�h]�ł�
	if (IsDBCSLeadByte(*font))
	{
		code = (byte)font[0] << 8 | (byte)font[1];
	}
	else
	{
		code = font[0];
	}
#endif

	// �t�H���g�r�b�g�}�b�v�擾
	GetTextMetrics(hdc, &fontData.TM);
	const MAT2	Mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
	DWORD		size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &fontData.GM, 0, NULL, &Mat);
	fontData.ptr = new byte[size];
	GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &fontData.GM, size, fontData.ptr, &Mat);

	// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	return S_OK;
}

void Dx11Font::Uninit(void)
{
	for (auto& f : font_)
	{
		if (f.second)
		{
			ReleasePtr(f.second);
		}
	}

	// ���\�[�X�폜
	DESIGNVECTOR design;
	RemoveFontResourceEx(TEXT(filename.c_str()), FR_PRIVATE, &design);
}

void Dx11Font::DrawFont(const string& font, VECTOR2 position, VECTOR2 size, COLOR color)
{
	if (font_.count(font) == 0) { return; }

	const auto& context = static_cast<DirectX11*>(graphics_)->GetDeviceContext();
	const auto& wrapper = graphics_->GetWrapper();

	context->PSSetShaderResources(0, 1, &font_[font]);
	wrapper->DrawQuad(position, size, color);
}
