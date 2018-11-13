#include "KohakuSword.h"
#include "../GameObject/GameObject.h"

//! @def	納刀位置
static const VECTOR3 PAID_POSITION = VECTOR3(30, -10, 0);
//! @def	納刀回転
static const VECTOR3 PAID_ROTATION = VECTOR3(-1, 0, 1.57f);

//! @def	抜刀位置
static const VECTOR3 DRAWN_POSITION = VECTOR3(6, 0, 0);
//! @def	抜刀回転
static const VECTOR3 DRAWN_ROTATION = VECTOR3(-1.57f, 1.35f, 3.14f);

/* @fn		コンストラクタ
 * @brief	変数の初期化				*/
KohakuSword::KohakuSword(void) : GUI(Systems::Instance(), this, "Sword")
{
}

/* @fn		デストラクタ
 * @brief	...							*/
KohakuSword::~KohakuSword(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	なし
 * @return	なし
 * @detail	姿勢、メッシュ、当たり判定の初期化	　*/
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

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし						*/
void KohakuSword::Uninit(void)
{
	DeletePtr(collider_);
}

/* @fn		Update
 * @brief	更新処理
 * @param	なし
 * @return	なし						*/
void KohakuSword::Update(void)
{
	if (collider_)
	{
		debug_hit_ = false;
		auto object = collider_->Hit();

		COLOR c = COLOR(1);
		for (auto& o : object)
		{
			if (o->GetTag() == Object::Tag::ENEMY)
			{
				debug_hit_ = true;

				c = COLOR(0, 1, 0, 1);

				if (!hit_)
				{
					static_cast<GameObject*>(o)->Hit(100);
					hit_ = true;
				}
			}
		}
		mesh_.material.diffuse = c;
	}
}

void KohakuSword::PaidSword(void)
{
	transform_.position = PAID_POSITION;
	transform_.rotation = PAID_ROTATION;

	transform_.parentMtx = body_;
}

void KohakuSword::DrawnSword(void)
{
	// 右手が握る位置に合わせる
	transform_.position = DRAWN_POSITION;
	// 剣の向きを整える
	transform_.rotation = DRAWN_ROTATION;

	transform_.parentMtx = hand_;
}

void KohakuSword::GuiUpdate(void)
{
	ImGui::DragFloat3("rot", transform_.rotation, 0.1f, 0, 6.14f);

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
