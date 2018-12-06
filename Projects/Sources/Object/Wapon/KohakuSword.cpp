#include "KohakuSword.h"
#include "../GameObject/GameObject.h"
#include <FrameWork/Object/ObjectManager.h>
#include "../Billboard/BloodSplash.h"

//! @def	納刀位置
static const VECTOR3 PAID_POSITION = VECTOR3(30, -10, 0);
//! @def	納刀回転
static const VECTOR3 PAID_ROTATION = VECTOR3(-1, 0, 1.57f);

//! @def	抜刀位置
static const VECTOR3 DRAWN_POSITION = VECTOR3(6, 0, 0);
//! @def	抜刀回転
static const VECTOR3 DRAWN_ROTATION = VECTOR3(-1.2f, 1.35f, 3.14f);

/* @brief	コンストラクタ			*/
KohakuSword::KohakuSword(void) : GUI(Systems::Instance(), this, "Sword")
	, debug_hit_(false)
{
}

/* @brief	デストラクタ			*/
KohakuSword::~KohakuSword(void)
{
}

/* @brief	初期化処理
 * @param	なし
 * @return	なし	　				*/
void KohakuSword::Init(void)
{
	// メッシュ
	mesh_.Init(Systems::Instance(), (int)Model::Game::SWORD, &transform_);

	// 当たり判定
	collider_ = new Collider3D::OBB(this);
	if (collider_)
	{
		collider_->SetOffsetPosition(VECTOR3(0, 0, 60));
		collider_->SetSize(VECTOR3(2, 20, 105));
		collider_->SetRendererColor(COLOR(1, 0, 0, 1));
		collider_->SetEnable(false);
	}
}

/* @brief	後処理
 * @param	なし
 * @return	なし					*/
void KohakuSword::Uninit(void)
{
	DeletePtr(collider_);
}

/* @brief	更新処理
 * @param	なし
 * @return	なし					*/
void KohakuSword::Update(void)
{
	if (collider_)
	{
		debug_hit_ = false;
		auto object = collider_->Hit();

		for (auto& o : object)
		{
			if (o->GetTag() == Object::Tag::ENEMY)
			{
				debug_hit_ = true;

				if (!isHit_)
				{
					static_cast<GameObject*>(o)->Hit(100);
					VECTOR3 p = transform_.globalPosition + collider_->GetDirect(2) * collider_->GetLen(2);
					manager_->Create<BloodSplash>(p, effectRotation_);
					isHit_ = true;
				}
			}
		}
	}
}

/* @brief	納刀処理
 * @param	なし
 * @return	なし					*/
void KohakuSword::PaidSword(void)
{
	// 姿勢の調整
	transform_.position = PAID_POSITION;
	transform_.rotation = PAID_ROTATION;

	transform_.parentMtx = body_;
}

/* @brief	抜刀処理
 * @param	なし
 * @return	なし					*/
void KohakuSword::DrawnSword(void)
{
	// 姿勢の調整
	transform_.position = DRAWN_POSITION;
	transform_.rotation = DRAWN_ROTATION;

	transform_.parentMtx = hand_;
}

/* @brief	Guiの更新処理
 * @param	なし
 * @return	なし					*/
void KohakuSword::GuiUpdate(void)
{
	ImGui::Text("effect : %.2f", effectRotation_);

	ImGui::DragFloat3("rot", transform_.rotation, 0.01f, -6.14f, 6.14f);

	auto g = transform_.globalPosition;
	ImGui::Text("global : %.2f, %.2f, %.2f", g.x, g.y, g.z);

	if (debug_hit_)
	{
		ImGui::Text("hit");
	}

	if (collider_)
	{
		string t[3] = { "x", "y", "z" };
		for (int i = 0; i < 3; ++i)
		{
			auto v = collider_->GetDirect(i);
			ImGui::Text(string(t[i] + " dir : %.2f, %.2f, %.2f").c_str(), v.x, v.y, v.z);
			float f = collider_->GetLen(i);
			ImGui::Text("  len : %.2f", f);
		}
	}
}
