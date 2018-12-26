#include "SetupAttackState.h"
#include <random>

#include "Slash2AttackState.h"
#include "../DrawnState/DrawnWaitState.h"
#include "../AvoidanceState.h"

//! @def	当たり判定開始
static constexpr int COLLISION_START = 30;
//! @def	当たり判定終了
static constexpr int COLLISION_END = 45;
//! @def	移動距離
static constexpr float MOVE = 0.1f;
//! @def	移動開始
static constexpr int MOVE_START = 20;
//! @def	移動終了
static constexpr int MOVE_END = 25;

SetupAttackState::SetupAttackState(void) : 
	isDraw_(false)
{
}

SetupAttackState::~SetupAttackState(void)
{
}

void SetupAttackState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	animSpeed_		= ANIM_SPEED;
	animation_		= Player::Animation::SetupDrawn;
	changeFrame_	= ANIMATION_CHANGE_FRAME30;
	collisionStart_ = COLLISION_START;
	collisionEnd_	= COLLISION_END;
	moveStart_		= MOVE_START;
	moveEnd_		= MOVE_END;
	move_			= MOVE;

	AttackBaseState::Init(player, ctrl);

	isDraw_ = false;
}

void SetupAttackState::Uninit(void)
{
}

PlayerState* SetupAttackState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	if (auto wapon = player_->GetWapon())
	{
		int inv = (dir_.z >= 0) ? 1 : -1;
		wapon->SetRotation(0.8f * inv);
	}

	if (auto temp = AttackBaseState::Update())
	{
		return temp;
	}

	float pattern = meshAnim.mesh.GetPattern();
	// アニメーションの情報
	int animMax = meshAnim.mesh.GetMaxAnimation();
	// 終了前に
	if (pattern > (Quarter(animMax) * 3.0f))
	{
		// 回避コマンドで回避ステート
		if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
		{
			if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
			{
				return new AvoidanceState;
			}
		}

		if (next_)
		{
			// 次の攻撃を行う
			next_ = false;
			// アニメーションが最後まで行ったら最初に戻る
			meshAnim.animation = static_cast<int>(Player::Animation::Slash_2);
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

			return new Slash2AttackState;
		}
	}

	if (pattern > 30)
	{
		// 武器の状態切り替え
		if (const auto& wapon = player_->GetWapon())
		{
			if (isDraw_ == player_->IsDraw())
			{
				wapon->Setup(isDraw_);
				player_->SetDraw(!player_->IsDraw());
			}
		}
	}

	return nullptr;
}

void SetupAttackState::GuiUpdate(void)
{
	ImGui::Text("SetupAttack");

	AttackBaseState::GuiUpdate();
}
