#include "DieState.h"
#include <random>

/* @brief	�R���X�g���N�^			*/
DieState::DieState(void)
{
}

/* @brief	�f�X�g���N�^			*/
DieState::~DieState(void)
{
}

/* @brief	����������
 * @param	(player)	�v���C���[�ւ̃|�C���^
 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
 * @return	�Ȃ�					*/
void DieState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// �U�����f
	if (const auto& wapon = player->GetWapon())
	{
		wapon->AttackEnd();
	}
	meshAnim.animSpeed = ANIMATION_DEFAULT;
	meshAnim.animation = static_cast<int>(Player::Animation::Die);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

	if (const auto& collider = player->GetCollider())
	{
		collider->SetEnable(false);
	}
}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void DieState::Uninit(void)
{
}

/* @brief	�X�V����
 * @param	�Ȃ�
 * @return	���̃X�e�[�g			*/
PlayerState* DieState::Update(void)
{
	if (!player_) { return nullptr; }

	return nullptr;
}

/* @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�v���C���[����Ă΂��	*/
void DieState::GuiUpdate(void)
{
	ImGui::Text("Die");
}
