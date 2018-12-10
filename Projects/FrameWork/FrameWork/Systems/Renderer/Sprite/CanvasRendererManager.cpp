//-----------------------------------------------------------------------------
//
//	2D�`��Ǘ�[CanvasRendererManager.cpp]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "CanvasRendererManager.h"
#include <math.h>
#include "../../../Graphics/Graphics.h"
#include "../../../Graphics/Utility/Font.h"
#include "../../GameSystems.h"

#include "../CanvasRenderer/CanvasRendererBase.h"
#include "../CanvasRenderer/CanvasRendererImage.h"

void CanvasRendererManager::Uninit(void)
{
	for (auto& obj : obj_)
	{
		if (obj) { obj->Uninit(); }
	}
}

void CanvasRendererManager::Add(CanvasRendererBase* obj)
{
	BaseManager::Add(obj);
	this->Sort();
}

void CanvasRendererManager::Sort(void)
{
	int size = (int)obj_.size() - 1;
	for (int i = 0; i < size; ++i)
	{
		// �D�揇�ʂɉ����ē���ւ�
		if (obj_[i]->GetPriority() > obj_[i + 1]->GetPriority())
		{
			Swap(obj_[i], obj_[i + 1]);
			i = -1;		// �C���N�������g�����0�ɖ߂�
		}
	}
}

//�|���S���`�揈��
void CanvasRendererManager::Draw(void)
{
	const auto& dev = systems_->GetRenderer()->GetWrapper();

	dev->BeginDrawCanvasRenderer();

	for (auto& obj : obj_)
	{
		if (obj->GetEnable())
		{
			const auto& type = obj->GetType();
			if (type == CanvasRendererBase::Type::Image)
			{
				const auto& img = static_cast<CanvasRenderer::Image*>(obj);
				Shader* shader = nullptr;
				Shader::ENUM draw = img->GetShader();
				if (draw != Shader::ENUM::UNKOUWN)
				{
					shader = systems_->GetShader()->GetShader(draw);
					MATRIX m;
					m._11 = img->GetAngle();
					shader->SetParam(m, img->GetColor(), VECTOR4(0, 0, 1, 1));
				}
				dev->SetTexture(0, img->GetTexNum());
				dev->Draw(img, shader);
				if (draw != Shader::ENUM::UNKOUWN)
				{
					dev->BeginDrawCanvasRenderer();
				}
			}
			else if (type == CanvasRendererBase::Type::Text)
			{
				if (const auto& font = dev->GetFont())
				{
					font->Draw("", obj->GetPosition(), obj->GetSize(), obj->GetColor());
				}
			}
		}
	}

	dev->EndDrawCanvasRenderer();
}
