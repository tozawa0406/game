#include "DrawnWaitAction.h"
#include <random>

//! @def	�A�j���[�V�����ύX���x
static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;
//! @def	�A�j���[�V�����̑��x
static constexpr float ANIMATION_DEFAULT = 0.75f;

DrawnWaitAction::DrawnWaitAction(void)
{
}

DrawnWaitAction::~DrawnWaitAction(void)
{
}

void DrawnWaitAction::Init(PlayerHunter* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerAction::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// �[����ԂƔ�����ԂŃA�j���[�V�����̐؂�ւ�
	meshAnim.animation = static_cast<int>(PlayerMove::Animation::SetupWait);
	// �Đ����x�̐ݒ�
	meshAnim.animSpeed = ANIMATION_DEFAULT;

	// �A�j���[�V�����̕ύX
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30, true);
}

void DrawnWaitAction::Uninit(void)
{
}

PlayerAction* DrawnWaitAction::Update(void)
{
	return nullptr;
}
