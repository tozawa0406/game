#include "Slash1AttackState.h"
#include <random>

#include "Slash2AttackState.h"
#include "../DrawnState/DrawnWaitState.h"
#include "../AvoidanceState.h"

//! @def	�R���W�����J�n
static constexpr int COLLISION_START = 18;
//! @def	�R���W�����I��
static constexpr int COLLISION_END = 26;
//! @def	�ړ�����
static constexpr float MOVE = 0.1f;
//! @def	�ړ��J�n
static constexpr int MOVE_START = 10;
//! @def	�ړ��I��
static constexpr int MOVE_END = 15;

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
Slash1AttackState::Slash1AttackState(void)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...						*/
Slash1AttackState::~Slash1AttackState(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	(player)	�v���C���[�ւ̃|�C���^
 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
 * @return	�Ȃ�					*/
void Slash1AttackState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }
	
	animSpeed_		= ANIM_SPEED;
	animation_		= Player::Animation::Slash_1;
	changeFrame_	= ANIMATION_CHANGE_FRAME30;
	collisionStart_ = COLLISION_START;
	collisionEnd_	= COLLISION_END;
	moveStart_		= MOVE_START;
	moveEnd_		= MOVE_END;
	move_			= MOVE;

	AttackBaseState::Init(player, ctrl);
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Slash1AttackState::Uninit(void)
{
}

/* @fn		Update
 * @breif	�X�V����
 * @param	�Ȃ�
 * @return	���̃X�e�[�g			*/
PlayerState* Slash1AttackState::Update(void)
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
			meshAnim.animation = static_cast<int>(Player::Animation::Slash_2);
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

			return new Slash2AttackState;
		}
	}

	return nullptr;
}

/* @fn		GuiUpdate
 * @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�v���C���[����Ăяo�����			*/
void Slash1AttackState::GuiUpdate(void)
{
	ImGui::Text("Slash1Attack");

	AttackBaseState::GuiUpdate();
}
