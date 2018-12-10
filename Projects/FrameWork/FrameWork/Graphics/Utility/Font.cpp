#include "Font.h"

Font::Font(void) : graphics_(nullptr)
{
}

Font::~Font(void)
{
}

HRESULT Font::CreateStringData(const string& fonts, int fontSize)
{
	HRESULT hr;
	int max = static_cast<int>(fonts.size());
	bool next = false;
	for (int i = 0; i < max; ++i)
	{
		if (next) { next = false; continue; }

		auto& f = fonts[i];
		if (f == ' ') { continue; }
		if ((('a' <= f && f <= 'z') || ('A' <= f && f <= 'Z')) ||
			('0' <= f && f <= '9'))
		{
			string s = { f };
			char temp[2] = { f, 0 };
			hr = CreateFontData(temp, fontSize);
			if (hr == E_ABORT) { continue; }
			else if (FAILED(hr)) { return E_FAIL; }
		}
		else
		{
			if (i + 1 >= max) { continue; }
			string s = { f, fonts[i + 1] };
			next = true;
			char temp[3] = { f, fonts[i + 1], 0 };
			hr = CreateFontData(temp, fontSize);
			if (hr == E_ABORT) { continue; }
			else if (FAILED(hr)) { return E_FAIL; }
		}
	}
	return S_OK;
}

void Font::Draw(const string& fonts, VECTOR2 position, VECTOR2 size, COLOR color, bool faldBack, VECTOR2 allSize)
{
	int max = static_cast<int>(fonts.size());

	VECTOR2 startPos = position + VECTOR2(Half(size.x), Half(size.y));
	VECTOR2 drawPos  = VECTOR2(0);

	bool draw = false;
	for(int i = 0;i < max;++i)
	{
		if (draw) { draw = false; continue; }

		auto& f = fonts[i];	
		if ((('a' <= f && f <= 'z') || ('A' <= f && f <= 'Z')) ||
			(('0' <= f && f <= '9') || (f == ' ')))
		{
			string temp = { f };
			if (faldBack)
			{
				if (drawPos.x + Half(size.x) > allSize.x)
				{
					drawPos.x = 0;
					drawPos.y += size.y;
					if (drawPos.y >= allSize.y) { i = max; }
				}
			}

			drawPos.x -= Quarter(size.x);
			DrawFont(temp, startPos + drawPos, VECTOR2(Half(size.x), size.y), color);
			drawPos.x += Half(size.x) + Quarter(size.x);
		}
		else
		{
			if (faldBack)
			{
				if (drawPos.x + size.x > allSize.x)
				{
					drawPos.x = 0;
					drawPos.y += size.y;
					if (drawPos.y >= allSize.y) { i = max; }
				}
			}

			if (i + 1 >= max) { continue; }
			string temp = { fonts[i], fonts[i + 1] };
			DrawFont(temp, startPos + drawPos, size, color);
			draw = true;
			drawPos.x += size.x;
		}
	}
}
