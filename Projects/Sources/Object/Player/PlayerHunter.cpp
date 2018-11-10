//-----------------------------------------------------------------------------
//
//	Auther : 戸澤翔太
//-----------------------------------------------------------------------------
#include "PlayerHunter.h"
#include <FrameWork/Systems/Input/Controller.h>
#include "../../Scene/GameScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Object/ObjectManager.h>
#include <random>

#include <FrameWork/Graphics/DirectX11/DirectX11Wrapper.h>

#include "PlayerState/PaidState.h"
#include "PlayerState/DrawnState.h"

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

	state_ = new PaidState;
	if (state_) { state_->Init(this, GetCtrl(0)); }
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

	COLOR c = COLOR(1, 1, 1, 1);
	const auto& hits = collider_->HitCollider();
	for (auto& hit : hits)
	{
		if (const auto& o = hit->GetParent())
		{
			if (o->GetTag() == Object::Tag::ENEMY && hit->GetRendererColor() == COLOR(1, 0, 0, 1))
			{
				c = COLOR(1, 0, 0, 1);
			}
		}
	}
	meshAnim_.mesh.material.diffuse = c;

#ifdef _SELF_DEBUG
	// デバッグ用、敵の操作中はプレイヤーの操作はしない
	if (cameraManager_ && cameraManager_->GetMainNum() != 0) { return; }
#endif

	Setup();

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
		meshAnim_.animation = static_cast<int>(Animation::Setup);
		meshAnim_.mesh.ChangeAnimation(meshAnim_.animation,30);

		// 納刀時は逆再生
		if (isDraw) { meshAnim_.animSpeed = -0.75f; }

		// 納刀抜刀中フラグを立てる
		BitAdd(flag_, IS_SETUP);
	}

	// アニメーション中
	if (meshAnim_.animation == static_cast<int>(Animation::Setup))
	{
		// 納刀抜刀中であり、アニメーションが一定以下
		if (BitCheck(flag_, IS_SETUP) && fabs(meshAnim_.mesh.GetPattern()) >= 30)
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
			meshAnim_.animSpeed = 0.6f;
			BitAdd(flag_, IS_ATTACK);
			meshAnim_.animation = static_cast<int>(Animation::Slash_1);
			meshAnim_.mesh.ChangeAnimation(meshAnim_.animation, 15);
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
		int animMax = meshAnim_.mesh.GetMaxAnimation();
		int pattern = static_cast<int>(meshAnim_.mesh.GetPattern());

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
				meshAnim_.animation = (meshAnim_.animation < static_cast<int>(Animation::Slash_3)) ? static_cast<int>(meshAnim_.animation) + 1 : static_cast<int>(Animation::Slash_1);
				meshAnim_.mesh.ChangeAnimation(meshAnim_.animation, 15);
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
			meshAnim_.animation = static_cast<int>(Animation::SetupWait);
			meshAnim_.mesh.ChangeAnimation(meshAnim_.animation, 15, true);
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
