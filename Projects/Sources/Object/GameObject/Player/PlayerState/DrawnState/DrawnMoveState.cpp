#include "DrawnMoveState.h"

#include "DrawnWaitState.h"
#include "../AvoidanceState.h"
#include "../SetupState.h"
#include "../AttackState/Slash1AttackState.h"

//! @def	アニメーションの速度
static constexpr float ANIM_SPEED = 0.55f;
//! @def	移動を遅くする
static constexpr float MOVE_DELAY = 0.75f;

DrawnMoveState::DrawnMoveState(void)
{
}

DrawnMoveState::~DrawnMoveState(void)
{
}

void DrawnMoveState::Init(Player* player, Controller* ctrl)
{
	PlayerState::Init(player, ctrl);
}

void DrawnMoveState::Uninit(void)
{
}

PlayerState* DrawnMoveState::Update(void)
{
	if (!player_ || !ctrl_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	VECTOR2 inputDir = VECTOR2(0);
	// Input
	inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
	inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
	// 正規化
	inputDir = VecNorm(inputDir);

	inputDir *= MOVE_DELAY;

	// アニメーション切り替え
	if (fabs(inputDir.x) + fabs(inputDir.y) > 0)
	{
		meshAnim.animSpeed = ANIM_SPEED;
		meshAnim.animation = static_cast<int>(Player::Animation::SetupWalk);
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30);
	}
	// 入力がなければ待機処理
	else
	{
		return new DrawnWaitState;;
	}

	// 移動速度
	if (const auto& camera = player_->GetCamera())
	{
		VECTOR3 velocity = player_->GetVelocity();
		velocity += camera->GetFrontXPlane() * inputDir.y * MOVE_SPEED;
		velocity -= camera->GetRightXPlane() * inputDir.x * MOVE_SPEED;
		player_->SetVelocity(velocity);
	}

	// 回避コマンドで回避ステート
	if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
	{
		if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
		{
			return new AvoidanceState;
		}
	}

	// 納刀コマンドで納刀ステート
	if (ctrl_->Trigger(Input::GAMEPAD_SQUARE, DIK_H) || ctrl_->Trigger(Input::GAMEPAD_R1, DIK_H))
	{
		return new SetupState;
	}

	// 攻撃コマンドで攻撃ステート
	if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
	{
		return new Slash1AttackState;
	}

	return nullptr;
}

void DrawnMoveState::GuiUpdate(void)
{
	ImGui::Text("DrawnMove");
}
