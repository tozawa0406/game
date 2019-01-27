#include "PaidMoveState.h"

#include "PaidWaitState.h"
#include "../AvoidanceState.h"
#include "../EmergencyAvoidanceState.h"
#include "../AttackState/SetupAttackState.h"
#include "HealState.h"

//! @def	消費スタミナ
static constexpr float DEC_STAMINA = 0.1f;

PaidMoveState::PaidMoveState(void) : 
	dash_(false)
{
}

PaidMoveState::~PaidMoveState(void)
{
}

void PaidMoveState::Init(Player* player, Controller* ctrl)
{
	PlayerState::Init(player, ctrl);
}

void PaidMoveState::Uninit(void)
{
}

PlayerState* PaidMoveState::Update(void)
{
	if (!player_ || !ctrl_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	// 入力
	VECTOR2 inputDir;
	// Input
	inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
	inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
	// 正規化
	inputDir = VecNorm(inputDir);

	if (ctrl_->Trigger(Input::GAMEPAD_L3, DIK_LSHIFT)) { dash_ = true; }

	float inputDash = 1.0f;
	if (ctrl_->Press(Input::GAMEPAD_R1, DIK_LSHIFT) || dash_)
	{
		float stamina = player_->GetStamina();
		if (stamina > 0)
		{
			inputDash = 2.5f;
			player_->SetStamina(stamina - (Player::ADD_STAMINA + DEC_STAMINA));
		}
	}

	inputDir *= inputDash;

	// アニメーション切り替え
	if (fabs(inputDir.x) + fabs(inputDir.y) > 0)
	{
		meshAnim.animSpeed = 0.55f;
		meshAnim.animation = static_cast<int>((inputDash <= 1) ? Player::Animation::Walk : Player::Animation::Run);
		int cnt = (meshAnim.animation == static_cast<int>(Player::Animation::Run)) ? 15 : ANIMATION_CHANGE_FRAME30;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, cnt);
	}
	// 入力がないと待機モーションへ
	else
	{
		return new PaidWaitState;
	}

	// 移動速度
	if (const auto& camera = player_->GetCamera())
	{
		VECTOR3 velocity = player_->GetVelocity();
		velocity += camera->GetFrontXPlane() * inputDir.y * MOVE_SPEED;
		velocity -= camera->GetRightXPlane() * inputDir.x * MOVE_SPEED;
		player_->SetVelocity(velocity);
	}

	// 回避コマンドで回避ステートへ
	if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
	{
		if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
		{
			if (inputDash == 2.5f)
			{
				return new EmergencyAvoidanceState;
			}
			else
			{
				return new AvoidanceState;
			}
		}
	}

	// 抜刀コマンドで抜刀ステートへ
	if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
	{
		return new SetupAttackState;
	}

	// アイテム使用
	if (ctrl_->Trigger(Input::GAMEPAD_SQUARE, DIK_H))
	{
		if (!ctrl_->Press(Input::GAMEPAD_L1, DIK_R))
		{
			return new HealState;
		}
	}
	
	return nullptr;
}

void PaidMoveState::GuiUpdate(void)
{
	ImGui::Text("PaidMove");
}
