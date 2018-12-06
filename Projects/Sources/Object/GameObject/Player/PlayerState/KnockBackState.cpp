#include "KnockBackState.h"
#include <random>

#include "PaidState/PaidWaitState.h"
#include "DrawnState/DrawnWaitState.h"

//! @def	��𑬓x
static constexpr float AVOIDANCE_SPEED = 2.75f;
//! @def	����A�j���[�V�����̏I���t���[��
static constexpr int   END_AVOIDANCE_ANIMATION = 30;

/* @brief	�R���X�g���N�^			*/
KnockBackState::KnockBackState(void)
{
}

/* @brief	�f�X�g���N�^			*/
KnockBackState::~KnockBackState(void)
{
}

/* @brief	����������
 * @param	(player)	�v���C���[�ւ̃|�C���^
 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
 * @return	�Ȃ�					*/
void KnockBackState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// �U�����f
	if (const auto& wapon = player->GetWapon())
	{
		wapon->AttackEnd();
	}
	meshAnim.animSpeed = 1;
	meshAnim.animation = static_cast<int>(Player::Animation::KnockBack);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

	if (auto collider = player_->GetCollider())
	{
		collider->SetEnable(false);
	}
}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void KnockBackState::Uninit(void)
{
}

/* @brief	�X�V����
 * @param	�Ȃ�
 * @return	���̃X�e�[�g			*/
PlayerState* KnockBackState::Update(void)
{
	if (!player_) { return nullptr; }

	if (player_->IsEndAnim())
	{
		if (auto collider = player_->GetCollider())
		{
			collider->SetEnable(true);
		}

		if (player_->IsDraw())
		{
			return new DrawnWaitState;
		}
		else
		{
			return new PaidWaitState;
		}
	}

	return nullptr;
}

/* @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�v���C���[����Ăяo�����		*/
void KnockBackState::GuiUpdate(void)
{
	ImGui::Text("KnockBack");
	ImGui::Text(string("IsDraw : " + Systems::Instance()->GetDebug()->BoolToString(player_->IsDraw())).c_str());
}
