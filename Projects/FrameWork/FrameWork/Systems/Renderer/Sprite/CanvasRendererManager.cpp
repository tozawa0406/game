//-----------------------------------------------------------------------------
//
//	2D•`‰æŠÇ—[CanvasRendererManager.cpp]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "CanvasRendererManager.h"
#include "CanvasRenderer.h"
#include <math.h>
#include "../../../Graphics/Graphics.h"
#include "../../GameSystems.h"

void CanvasRendererManager::Add(CanvasRenderer* obj)
{
	BaseManager::Add(obj);
	this->Sort();
}

void CanvasRendererManager::Sort(void)
{
	int size = (int)obj_.size() - 1;
	for (int i = 0; i < size; ++i)
	{
		// —Dæ‡ˆÊ‚É‰‚¶‚Ä“ü‚ê‘Ö‚¦
		if (obj_[i]->priority_ > obj_[i + 1]->priority_)
		{
			Swap(obj_[i], obj_[i + 1]);
			i = -1;		// ƒCƒ“ƒNƒŠƒƒ“ƒg‚³‚ê‚Ä0‚É–ß‚é
		}
	}
}

//ƒ|ƒŠƒSƒ“•`‰æˆ—
void CanvasRendererManager::Draw(void)
{
	const auto& dev = systems_->GetRenderer()->GetWrapper();

	dev->BeginDrawCanvasRenderer();

	for (auto& obj : obj_)
	{
		if (obj->enable)
		{
			Shader* shader = nullptr;
			if (obj->shader != Shader::ENUM::UNKOUWN) 
			{
				shader = systems_->GetShader()->GetShader(obj->shader);
				MATRIX m;
				m._11 = obj->angle;
				shader->SetParam(m, obj->color, VECTOR4(0, 0, 1, 1));
			}
			dev->SetTexture(0, (int)obj->texNum);
			dev->Draw(obj, shader);
			if (obj->shader != Shader::ENUM::UNKOUWN)
			{
				dev->BeginDrawCanvasRenderer();
			}
		}
	}

	dev->EndDrawCanvasRenderer();
}
