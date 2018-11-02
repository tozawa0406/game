//-----------------------------------------------------------------------------
//
//	Auther : 戸澤翔太
//-----------------------------------------------------------------------------
#include "PlayerHunter.h"
#include <FrameWork/Systems/Input/Controller.h>
#include "../../Scene/EachScene/GameScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Object/ObjectManager.h>
#include <random>

#include <FrameWork/Graphics/DirectX11/DirectX11Wrapper.h>

//! @def	回避アニメーションの終了フレーム
static constexpr int END_AVOIDANCE_ANIMATION = 30;

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
PlayerHunter::PlayerHunter(void)
{
}

/* @fn		デストラクタ
 * @brief	...						*/
PlayerHunter::~PlayerHunter(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	なし
 * @return	なし
 * @detail	基底クラスの処理呼び出し		*/
void PlayerHunter::Init(void)
{
	PlayerMove::Init();
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし
 * @detail	基底クラスの処理呼び出し		*/
void PlayerHunter::Uninit(void)
{
	PlayerMove::Uninit();
}

/* @fn		Update
 * @brief	更新処理
 * @param	なし
 * @return	なし
 * @detail	基底クラスの処理呼び出し、
			納刀抜刀処理、回避処理、攻撃処理		*/
void PlayerHunter::Update(void)
{
	PlayerMove::Update();

	Setup();

	Avoidance();

	Attack();

}

/* @fn		Setup
 * @brief	納刀抜刀処理
 * @sa		Update()
 * @param	なし
 * @return	なし							*/
void PlayerHunter::Setup(void)
{
	// 回避中はできない
	if (BitCheck(flag_, IS_AVOIDANCE)) { return; }

	// 良く使うので一時変数に入れる
	bool isDraw = BitCheck(flag_, IS_DRAWN);

	// 納刀時と抜刀時でキーとコントローラボタンを変える
	uint key  = (isDraw) ? DIK_H: DIK_U;
	WORD ctrl = (isDraw) ? Input::GAMEPAD_SQUARE : Input::GAMEPAD_TRIANGLE;

	// 入力
	if (GetCtrl(0)->Trigger(ctrl, key))
	{
		// アニメーション
		animation_ = Animation::Setup;
		mesh_.ChangeAnimation((int)animation_,30);

		// 納刀時は逆再生
		if (isDraw) { animCnt_ = -0.75f; }

		// 納刀抜刀中フラグを立てる
		BitAdd(flag_, IS_SETUP);
	}

	// アニメーション中
	if (animation_ == Animation::Setup)
	{
		// 納刀抜刀中であり、アニメーションが一定以下
		if (BitCheck(flag_, IS_SETUP) && fabs(mesh_.GetPattern()) >= 30)
		{
			// 武器の状態切り替え
			if (wapon_)
			{
				wapon_->Setup(isDraw);
			}
			// 納刀抜刀終了
			BitSub(flag_, IS_SETUP);
			isDraw ? BitSub(flag_, IS_DRAWN) : BitAdd(flag_, IS_DRAWN);
		}
	}
}

// 回避処理
/* @fn		Avoidance
 * @brief	回避処理
 * @sa		Update()
 * @param	なし
 * @return	なし							*/
void PlayerHunter::Avoidance(void)
{
	// 入力
	if (GetCtrl(0)->Trigger(Input::GAMEPAD_CROSS, DIK_M))
	{
		if (wapon_)
		{
			wapon_->AttackEnd();
		}
		animCnt_     =	ANIMATION_DEFAULT;
		BitAdd(flag_, IS_AVOIDANCE);
		animation_   = Animation::Roll;
		BitSub(flag_, IS_SETUP);
		BitSub(flag_, IS_ATTACK);
		mesh_.ChangeAnimation((int)animation_, 15);

		// 入力方向に回避、入力がないときは前に回避
		if (inputDir_ == 0)
		{
			avoidanceDir_ = -front_;
		}
		else
		{
			if (camera_)
			{
				avoidanceDir_ = VECTOR3(0);
				avoidanceDir_ += camera_->GetFrontXPlane() * inputDir_.y;
				avoidanceDir_ -= camera_->GetRightXPlane() * inputDir_.x;
				avoidanceDir_ = VecNorm(avoidanceDir_);
			}
		}
	}

	if (animation_ == Animation::Roll)
	{
		if (BitCheck(flag_, IS_AVOIDANCE) && mesh_.GetPattern() >= END_AVOIDANCE_ANIMATION)
		{				
			if (BitCheck(flag_, IS_DRAWN))
			{
				animation_ = (inputDir_ != 0) ? Animation::SetupWalk : Animation::SetupWait;
			}
			else
			{
				animation_ = (inputDir_ != 0) ? (inputDash_ == 1) ? Animation::Walk : Animation::Run : Animation::Wait;
			}

			mesh_.ChangeAnimation((int)animation_, 15, false);
			velocity_ *= 0.5f;
			BitSub(flag_, IS_AVOIDANCE);
		}
	}
}

/* @fn		Attack
 * @brief	攻撃処理
 * @sa		Update()
 * @param	なし
 * @return	なし			*/
void PlayerHunter::Attack(void)
{
	// 回避中である時、抜刀中でないときは返る
	if (BitCheck(flag_, IS_AVOIDANCE) || !BitCheck(flag_, IS_DRAWN)) { return; }

	// 入力
	if (GetCtrl(0)->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
	{
		// 攻撃していない時
		if (!BitCheck(flag_, IS_ATTACK))
		{
			if (wapon_)
			{
				wapon_->AttackStart();
			}
			// 最初の攻撃モーション
			animCnt_ = 0.75f;
			BitAdd(flag_, IS_ATTACK);
			animation_ = Animation::Slash_1;
			mesh_.ChangeAnimation((int)animation_, 15);
		}
		else
		{
			// 攻撃中なら次の攻撃を行う
			BitAdd(flag_, IS_NEXT_SLASH);
		}
	}

	// 攻撃中
	if (BitCheck(flag_, IS_ATTACK))
	{
		// アニメーションの情報
		int animMax = mesh_.GetMaxAnimation();
		int pattern = static_cast<int>(mesh_.GetPattern());

		// 終了前に
		if (pattern > (Quarter(animMax) * 3))
		{
			// 次の入力がある
			if (BitCheck(flag_, IS_NEXT_SLASH))
			{
				if (wapon_)
				{
					wapon_->AttackStart();
				}
				// 次の攻撃を行う
				BitSub(flag_, IS_NEXT_SLASH);
				// アニメーションが最後まで行ったら最初に戻る
				animation_ = (animation_ < Animation::Slash_3) ? static_cast<Animation>(static_cast<int>(animation_) + 1) : Animation::Slash_1;
				mesh_.ChangeAnimation((int)animation_, 15);
			}
		}

		// アニメーションの終了
		if (BitCheck(flag_, IS_END_ANIMATION))
		{
			if (wapon_)
			{
				wapon_->AttackEnd();
			}
			// 抜刀待機状態に戻る
			BitSub(flag_, IS_ATTACK);
			animation_ = Animation::SetupWait;
			mesh_.ChangeAnimation((int)animation_, 15, true);
		}
	}
}

/* @fn		GuiUpdate
 * @brief	デバッグ用描画更新
 * @param	なし
 * @return	なし					*/
void PlayerHunter::GuiUpdate(void)
{
	PlayerMove::GuiUpdate();
}
