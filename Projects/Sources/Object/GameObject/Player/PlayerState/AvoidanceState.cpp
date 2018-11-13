#include "AvoidanceState.h"
#include <random>

#include "PaidState/PaidWaitState.h"
#include "PaidState/PaidMoveState.h"
#include "DrawnState/DrawnWaitState.h"
#include "DrawnState/DrawnMoveState.h"

//! @def	回避速度
static constexpr float AVOIDANCE_SPEED = 2.75f;
//! @def	回避アニメーションの終了フレーム
static constexpr int   END_AVOIDANCE_ANIMATION = 30;

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
AvoidanceState::AvoidanceState(void) : dir_(VECTOR3(0)), isDraw_(false)
{
}

/* @fn		デストラクタ
 * @brief	...						*/
AvoidanceState::~AvoidanceState(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	(player)	プレイヤーへのポインタ
 * @param	(ctrl)		コントローラへのポインタ
 * @return	なし					*/
void AvoidanceState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// 回避前の状態
	isDraw_ = (meshAnim.animation >= static_cast<int>(Player::Animation::SetupWait)) ? true: false;

	// 攻撃中断
	if (const auto& wapon = player->GetWapon())
	{
		wapon->AttackEnd();
	}
	meshAnim.animSpeed = ANIMATION_DEFAULT;
	meshAnim.animation = static_cast<int>(Player::Animation::Roll);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

	// 入力方向に回避、入力がないときは前に回避
	if (ctrl_)
	{
		VECTOR2 inputDir;
		// Input
		inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// 正規化
		inputDir = VecNorm(inputDir);

		// 方向決め
		if (inputDir == 0)
		{
			dir_ -= player->GetFront();
		}
		else
		{
			if (const auto& camera_ = player->GetCamera())
			{
				dir_ = VECTOR3(0);
				dir_ += camera_->GetFrontXPlane() * inputDir.y;
				dir_ -= camera_->GetRightXPlane() * inputDir.x;
				dir_ = VecNorm(dir_);
			}
		}
	}
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし					*/
void AvoidanceState::Uninit(void)
{
}

/* @fn		Update
 * @brief	更新処理
 * @param	なし
 * @return	次のステート			*/
PlayerState* AvoidanceState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	//終了時
	if (meshAnim.mesh.GetPattern() >= END_AVOIDANCE_ANIMATION)
	{
		VECTOR2 inputDir;
		// Input
		inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// 正規化
		inputDir = VecNorm(inputDir);

		VECTOR3 velocity = player_->GetVelocity();
		velocity *= 0.5f;
		player_->SetVelocity(velocity);

		// 入力があれば移動ステート
		if (inputDir != 0)
		{
			if (isDraw_) { return new DrawnMoveState; }
			else		 { return new PaidMoveState;  }
		}
		else
		{
			// 納刀状態と抜刀状態でアニメーションの切り替え
			meshAnim.animation = static_cast<int>((isDraw_) ? Player::Animation::SetupWait : Player::Animation::Wait);
			// アニメーションの変更
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15, false);

			if (isDraw_) { return new DrawnWaitState; }
			else		 { return new PaidWaitState;  }
		}
	}

	// 移動速度
	VECTOR3 velocity = player_->GetVelocity();
	velocity += dir_ * MOVE_SPEED * AVOIDANCE_SPEED;
	player_->SetVelocity(velocity);

	return nullptr;
}

/* @fn		GuiUpdate
 * @brief	Guiの更新処理
 * @param	なし
 * @return	なし
 * @detail	プレイヤーから呼び出される		*/
void AvoidanceState::GuiUpdate(void)
{
	ImGui::Text("Avoidance");
}
