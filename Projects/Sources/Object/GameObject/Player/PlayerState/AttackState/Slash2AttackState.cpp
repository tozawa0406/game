#include "Slash2AttackState.h"
#include <random>

#include "Slash3AttackState.h"
#include "../DrawnState/DrawnWaitState.h"
#include "../AvoidanceState.h"

//! @def	�R���W�����J�n
static constexpr int COLLISION_START = 21;
//! @def	�R���W�����I��
static constexpr int COLLISION_END = 35;
//! @def	�ړ�����
static constexpr float MOVE = 0.1f;
//! @def	�G�t�F�N�g�̉�]�p�x
static constexpr float EFFECT_ROTATION = -1.57f;
//! @def	�ړ��J�n
static constexpr int MOVE_START = 10;
//! @def	�ړ��I��
static constexpr int MOVE_END = 15;


Slash2AttackState::Slash2AttackState(void)
{
}

Slash2AttackState::~Slash2AttackState(void)
{
}

void Slash2AttackState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }
	
	animSpeed_		= ANIM_SPEED;
	animation_		= Player::Animation::Slash_2;
	changeFrame_	= ANIMATION_CHANGE_FRAME30;
	collisionStart_ = COLLISION_START;
	collisionEnd_	= COLLISION_END;
	effectRot_		= EFFECT_ROTATION;
	moveStart_		= MOVE_START;
	moveEnd_		= MOVE_END;
	move_			= MOVE;
	voice_			= static_cast<int>(Resources::Sound::Camp::UNITYCHAN_ATTACK02);

	AttackBaseState::Init(player, ctrl);

	if (auto* wapon = player_->GetWapon())
	{
		wapon->SetOffensivePower(75);
	}
}

void Slash2AttackState::Uninit(void)
{
	AttackBaseState::Uninit();
}

PlayerState* Slash2AttackState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	if (auto temp = AttackBaseState::Update())
	{
		return temp;
	}

	float pattern = meshAnim.mesh.GetPattern();
	// �A�j���[�V�����̏��
	int animMax = meshAnim.mesh.GetMaxAnimation();
	// �I���O��
	if (pattern > (Quarter(animMax) * 3.0f))
	{
		// ����R�}���h�ŉ���X�e�[�g
		if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
		{
			if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
			{
				return new AvoidanceState;
			}
		}

		if (next_)
		{
			// ���̍U�����s��
			next_ = false;
			// �A�j���[�V�������Ō�܂ōs������ŏ��ɖ߂�
			meshAnim.animation = static_cast<int>(Player::Animation::Slash_3);
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

			return new Slash3AttackState;
		}
	}

	return nullptr;
}

void Slash2AttackState::GuiUpdate(void)
{
	ImGui::Text("Slash2Attack");

	AttackBaseState::GuiUpdate();
}
