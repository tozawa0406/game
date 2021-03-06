#include "EmergencyAvoidanceState.h"
#include <random>

#include "PaidState/PaidWaitState.h"
#include "PaidState/PaidMoveState.h"
#include "DrawnState/DrawnWaitState.h"
#include "DrawnState/DrawnMoveState.h"

//! @def	回避速度
static constexpr float AVOIDANCE_SPEED = 2.75f;
//! @def	回避アニメーション速度
static constexpr float AVOIDANCE_ANIM_SPEED = 0.5f;
//! @def	立ち上がるアニメーションの速度
static constexpr float STANDUP_ANIM_SPEED = 2;
//! @def	移動するフレーム
static constexpr int   MOVE_FRAME = 75;

EmergencyAvoidanceState::EmergencyAvoidanceState(void) : 
	dir_(VECTOR3(0))
	, cnt_(0)
{
}

EmergencyAvoidanceState::~EmergencyAvoidanceState(void)
{
}

void EmergencyAvoidanceState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// 攻撃中断
	if (const auto& wapon = player->GetWapon())
	{
		wapon->AttackEnd();
	}
	meshAnim.animSpeed = AVOIDANCE_ANIM_SPEED;
	meshAnim.animation = static_cast<int>(Player::Animation::Dive);
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

	if (auto collider = player->GetDefenseCollider())
	{
		collider->SetEnable(false);
	}

	player->SetStamina(player->GetStamina() - DEC_STAMINA);

	if (const auto& systems = Systems::Instance())
	{
		if (const auto& sound = systems->GetSound())
		{
			sound->Play(static_cast<int>(Resources::Sound::Camp::UNITYCHAN_AVOIDANCE));
		}
	}
}

void EmergencyAvoidanceState::Uninit(void)
{
}

PlayerState* EmergencyAvoidanceState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	player_->SetStamina(player_->GetStamina() - Player::ADD_STAMINA);

	cnt_++;
	//終了時
	if (player_->IsEndAnim())
	{
		VECTOR2 inputDir;
		// Input
		inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// 正規化
		inputDir = VecNorm(inputDir);

		VECTOR3 velocity = Half(player_->GetVelocity());
		player_->SetVelocity(velocity);

		if (auto collider = player_->GetDefenseCollider())
		{
			collider->SetEnable(true);
		}
		meshAnim.mesh.AnimEndPattern();
		// 入力があれば移動ステート
		if (inputDir != 0)
		{
			meshAnim.animation = static_cast<int>(Player::Animation::Walk);
			// アニメーションの変更
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15, true);
			return new PaidMoveState;
		}
		else
		{
			meshAnim.animation = static_cast<int>(Player::Animation::Wait);
			// アニメーションの変更
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15, true);
			return new PaidWaitState;
		}
	}

	if (cnt_ < MOVE_FRAME)
	{
		// 移動速度
		VECTOR3 velocity = player_->GetVelocity();
		velocity += dir_ * MOVE_SPEED * AVOIDANCE_SPEED;
		player_->SetVelocity(velocity);
	}
	else
	{
		meshAnim.animSpeed = STANDUP_ANIM_SPEED;
	}

	return nullptr;
}

void EmergencyAvoidanceState::GuiUpdate(void)
{
	ImGui::Text("EmergencyAvoidance");
	ImGui::Text("cnt : %d", cnt_);
}
