#include "CanvasRendererText.h"
#include "../../../Graphics/Utility/Font.h"

CanvasRenderer::Text::Text(void) : CanvasRendererBase(Type::Text)
	, string_("")
	, drawSize_(VECTOR2(0))
	, faldBack_(false)
{
}

CanvasRenderer::Text::~Text(void)
{
}

void CanvasRenderer::Text::Init(byte priority, const string& s, int fontSize)
{
	priority_	= priority;
	OnInit();
	string_		= s;
	size_		= VECTOR2(static_cast<float>(fontSize));

	if (manager_)
	{
		if (const auto& systems = manager_->GetSystems())
		{
			if (const auto& graphics = systems->GetRenderer())
			{
				if (const auto& wrapper = graphics->GetWrapper())
				{
					if (const auto& font = wrapper->GetFont())
					{
						font->CreateStringData(s, fontSize);
					}
				}
			}
		}
	}
}

void CanvasRenderer::Text::Uninit(void)
{
	OnUninit();
}
