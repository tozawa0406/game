//-----------------------------------------------------------------------------
//
//	2D“–‚½‚è”»’èŠÇ—[Collider2DManager.cpp]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Collider2D.h"
#include <math.h>
#include "../../Object/Object.h"
#include "../../Scene/SceneManager.h"
#include "../Camera/CameraManager.h"
#include "../GameSystems.h"
#include "Collider2D.h"

Collision2DManager::Collision2DManager(Systems* systems) : BaseManager(systems)
{
}

Collision2DManager::~Collision2DManager(void)
{
	for (UINT i = 0; i < obj_.size();)
	{
		this->RemovePtr(obj_[i]);
	}
}

//‰~‚Ì“–‚½‚è”»’è
bool Collision2DManager::HitCircle(float x0, float y0, float r0, float x1, float y1, float r1)
{
	//ƒIƒuƒWƒFƒNƒg“¯m‚Ì‹——£‚ğæ‚é(‚Qæ)
	float l = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);

	//ƒIƒuƒWƒFƒNƒg“¯m‚Ì”¼Œa‚Ì˜a(‚Qæ)‚Æã‹L‚ğ”äŠr
	return l < (r0 + r1) * (r0 + r1) ? true : false;

}

// ‹éŒ`‚Ì“–‚½‚è”»’è
bool Collision2DManager::HitRectangle(float x0, float y0, float w0, float h0, float x1, float y1, float w1, float h1)
{
	//ã‰º¶‰E‚Ì•Ó
	float hw0 = w0 * 0.5f;
	float hh0 = h0 * 0.5f;
	float hw1 = w1 * 0.5f;
	float hh1 = h1 * 0.5f;
	SidesVec4 pos0 = { x0 - hw0, x0 + hw0, y0 - hh0, y0 + hh0 };
 	SidesVec4 pos1 = { x1 - hw1, x1 + hw1, y1 - hh1, y1 + hh1 };
	
	//¶•Ó‚ª‰E•Ó‚æ‚è‰E ‚© ‰E•Ó‚ª¶•Ó‚æ‚è¶ ‚© ã•Ó‚ª‰º•Ó‚æ‚è‰º ‚© ‰º•Ó‚ªã•Ó‚æ‚èã
	if (pos0.l >= pos1.r || pos0.r <= pos1.l || pos0.u >= pos1.d || pos0.d <= pos1.u)
	{
		return false;
	}

	return true;
}

// ‰~‚Æ‹éŒ`‚Ì“–‚½‚è”»’è
bool Collision2DManager::HitCircleToRectangle(float x0, float y0, float r0, float x1, float y1, float w1, float h1)
{
	//lŠp‚Ìã‰º¶‰E
	float hw1 = w1 * 0.5f;
	float hh1 = h1 * 0.5f;
	SidesVec4 pos1 = { x1 - hw1, x1 + hw1, y1 - hh1, y1 + hh1 };

	//lŠp‚Ì‰¡•‚Ì’†‚É‚¢‚é‚Æ‚«
	if (pos1.l <= x0 && x0 <= pos1.r)
	{
		//‰~‚ÌÅ‰º“_‚ªlŠp‚Ìc•‚Ì’†‚É‚¢‚é‚Æ‚« ‚© ‰~‚ÌÅã“_‚ªlŠp‚Ìc•‚Ì’†‚É‚¢‚é‚Æ‚«
		if ((pos1.u <= (y0 + r0) && (y0 + r0) <= pos1.d) || (pos1.u <= (y0 - r0) && (y0 - r0) <= pos1.d))
		{
			return true;
		}
	}
	//lŠp‚Ìc•‚Ì’†‚É‚¢‚é‚Æ‚«
	else if (pos1.u <= y0 && y0 <= pos1.d)
	{
		//‰~‚ÌÅ‰E“_‚ªlŠp‚Ì‰¡•‚Ì’†‚É‚¢‚é‚Æ‚« ‚© ‰~‚ÌÅ¶“_‚ªlŠp‚Ì‰¡•‚Ì’†‚É‚¢‚é‚Æ‚«
		if ((pos1.l <= (x0 + r0) && (x0 + r0) <= pos1.r) || (pos1.l <= (x0 - r0) && (x0 - r0) <= pos1.r))
		{
			return true;
		}
	}

	//¶ã’¸“_‚Æ‰~‚Ì“–‚½‚è”»’è ‚© ‰Eã’¸“_‚Æ‰~‚Ì“–‚½‚è”»’è ‚© ¶‰º’¸“_‚Æ‰~‚Ì“–‚½‚è”»’è ‚© ‰E‰º’¸“_‚Æ‰~‚Ì“–‚½‚è”»’è
	if (HitCircle(x0, y0, r0, pos1.l, pos1.u, 0.05f) || HitCircle(x0, y0, r0, pos1.r, pos1.u, 0.05f)
	 || HitCircle(x0, y0, r0, pos1.l, pos1.d, 0.05f) || HitCircle(x0, y0, r0, pos1.r, pos1.d, 0.05f))
	{
		return true;
	}

	return false;
}

void Collision2DManager::Update(void)
{
	for (auto col : obj_)
	{
		col->list_.clear();
		if (!systems_->GetDebug()->GetDebug())
		{
			col->renderer_.enable = false;
		}
	}

	for (auto col1 : obj_)
	{
		if (!col1->enable_) { continue; }
		for (auto col2 : obj_)
		{
			if (!col2->enable_) { continue; }
			if (col1 == col2) { continue; }

			bool isList = false;
			for (auto obj : col2->list_)
			{
				if (obj == col1->object_) { isList = true; }
			}
			if (isList) { continue; }

			if (col1->type_ == Collider2DBase::Type::CIRCLE)
			{
				if (col2->type_ == Collider2DBase::Type::CIRCLE)
				{
					if (this->HitCircle(col1->position_.x, col1->position_.y, Half(col1->size_.x), col2->position_.x, col2->position_.y, Half(col2->size_.x)))
					{
						col1->list_.emplace_back(col2->object_);
						col2->list_.emplace_back(col1->object_);
					}
				}
				else if (col2->type_ == Collider2DBase::Type::RECTANGLE)
				{
					if (this->HitCircleToRectangle(col1->position_.x, col1->position_.y, Half(col1->size_.x), col2->position_.x, col2->position_.y, col2->size_.x, col2->size_.y))
					{
						col1->list_.emplace_back(col2->object_);
						col2->list_.emplace_back(col1->object_);
					}
				}
			}
			else if (col1->type_ == Collider2DBase::Type::RECTANGLE)
			{
				if (col2->type_ == Collider2DBase::Type::CIRCLE)
				{
					if (this->HitCircleToRectangle(col2->position_.x, col2->position_.y, Half(col2->size_.x), col1->position_.x, col1->position_.y, col1->size_.x, col1->size_.y))
					{
						col1->list_.emplace_back(col2->object_);
						col2->list_.emplace_back(col1->object_);
					}
				}
				else if (col2->type_ == Collider2DBase::Type::RECTANGLE)
				{
					if (this->HitRectangle(col1->position_.x, col1->position_.y, col1->size_.x, col1->size_.y, col2->position_.x, col2->position_.y, col2->size_.x, col2->size_.y))
					{
						col1->list_.emplace_back(col2->object_);
						col2->list_.emplace_back(col1->object_);
					}
				}
			}

		}
	}
}
