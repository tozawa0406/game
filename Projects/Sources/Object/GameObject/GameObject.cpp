#include "GameObject.h"
#include "../../Scene/GameScene.h"

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
GameObject::GameObject(Object::Tag tag) : Object(tag)
	, life_(0)
	, ded_(false)
	, isEndAnim_(false)
	, velocity_(VECTOR3(0))
	, front_(VECTOR3(0))
	, right_(VECTOR3(0))
{
}

/* @fn		デストラクタ
 * @brief	...						*/
GameObject::~GameObject(void)
{
}

/* @fn		Move
 * @brief	移動時の回転処理
 * @sa		Update()
 * @param	なし
 * @return	なし					*/
void GameObject::Move(void)
{
	// キャラクターの前ベクトルの生成
	CreateFrontVector();

	// 移動向きによりキャラクターの向きを変える
	if ((Abs(velocity_.x) + Abs(velocity_.z) > 0.02f))
	{
		VECTOR3 velocityNorm = VecNorm(velocity_);
		VECTOR3 frontVelocityCross = VecCross(front_, velocityNorm);
		float	dot = VecDot(front_, velocityNorm);

		// 前か後ろに進みたいとき
		int sign = 1;
		if (frontVelocityCross.y < 0) { sign = -1; }

		int upY = (int)(((frontVelocityCross.y * 10) + (5 * sign)) * 0.1f);
		// 内積が0以下の時(後ろに進むとき)
		if (upY == 0 && dot == 1)
		{
			// 強制的に回す
			frontVelocityCross.y = 1.0f * sign;
		}

		transform_.rotation.y += frontVelocityCross.y * 0.3f;
	}
	else if (Abs(velocity_.y) > 0.02f) {}
	else { velocity_ = VECTOR3(0); }

	transform_.position += velocity_;		// 移動

	velocity_ *= 0.8f;						// 慣性
}

/* @fn		CreateFrontVector
 * @brief	前ベクトルの生成
 * @sa		Move()
 * @param	なし
 * @return	なし					*/
void GameObject::CreateFrontVector(void)
{
	MATRIX frontObj;
	frontObj.Identity().Translation(VECTOR3(0, 0, 1));
	MATRIX mtx;
	mtx.Identity().Rotation(VECTOR3(0, transform_.rotation.y, 0));
	mtx.Translation(transform_.position);
	frontObj *= mtx;

	VECTOR3 tempTarget = VECTOR3(frontObj._41, transform_.position.y, frontObj._43);
	front_ = transform_.position - tempTarget;
	front_ = VecNorm(front_);

	right_ = VecNorm(VecCross(VECTOR3(0, 1, 0), front_));
}

/* @fn		OnGround
 * @brief	接地判定
 * @sa		Update()
 * @param	なし
 * @return	なし					*/
void GameObject::OnGround(void)
{
	transform_.position.y = 0;
	if (manager_)
	{
		if (const auto& scene = static_cast<GameScene*>(manager_->GetScene()))
		{
			if (const auto& meshfield = scene->GetMeshField())
			{
				float y = meshfield->Hit(transform_.position);
				if (y > 0) { transform_.position.y += y; }
			}
		}
	}
}
