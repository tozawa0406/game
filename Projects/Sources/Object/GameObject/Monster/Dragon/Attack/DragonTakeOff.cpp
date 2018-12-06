#include "DragonTakeOff.h"
#include "../Dragon.h"

/* @brief	�R���X�g���N�^			*/
DragonTakeOff::DragonTakeOff(void) : fly_(false)
	, debug_velocity_(0)
{
}

/* @brief	�f�X�g���N�^			*/
DragonTakeOff::~DragonTakeOff(void)
{
}

/* @brief	����������
 * @param	(object)	���̃N���X�ł͎g��Ȃ�
 * @return	�Ȃ�					*/
void DragonTakeOff::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);
}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void DragonTakeOff::Uninit(void)
{
}

/* @brief	���s���鏀��
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	���̍U�����L�̃A�j���[�V�����A���x��ݒ肷��		*/
void DragonTakeOff::SetMove(void)
{
	// ���Ɏg�p���Ȃ�d���h�~
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	// ���x�̐ݒ�
	meshAnim.animSpeed = fly_ ? -0.75f : 0.75f;

	// �A�j���[�V�����̐ݒ�
	meshAnim.animation = static_cast<int>(Dragon::Animation::TAKE_OFF);

	// ���ۂ̃A�j���[�V�����̐؂�ւ�
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);
}

/* @brief	�X�V����
 * @param	�Ȃ�
 * @return	�U�����I��������true
 * @detail	�p���ύX�͂Ȃ��A�ړ����Ȃ��悤���x��0�ɌŒ�
			��莞�Ԍo�ߌ�A�A�j���[�V�����̑��x���グ��A�A�j���[�V�������I�������猳�ɖ߂��ďI��		*/
bool DragonTakeOff::Update(void)
{
	if (!monster_) { return true; }

	auto velocity = monster_->GetVelocity();
	int sign = (fly_) ? -1 : 1;
	velocity.x = 0;
	velocity.z = 0;
	velocity.y += 0.1f * sign;
	monster_->SetVelocity(velocity);

	debug_velocity_ = velocity.y;

	// ���o�p
	frame_++;

	// �A�j���[�V�����I��
	if (monster_->IsEndAnim())
	{
		auto& meshAnim = monster_->GetMeshAnimation();
		// ���ɖ߂�
		meshAnim.animSpeed = 0.75f;
		meshAnim.animation = static_cast<int>((fly_) ? Dragon::Animation::WAIT1 : Dragon::Animation::FLY_IDLE);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, 5, true);
		return true;
	}
	return false;
}

/* @brief	���[�V�����̏I������
 * @param	�Ȃ�
 * @return	�Ȃ�			*/
void DragonTakeOff::EndMove(void)
{
}

/* @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�U�����I�u�W�F�N�g����Ă΂��		*/
void DragonTakeOff::GuiUpdate(void)
{
	ImGui::Text("frame : %d", frame_);
	ImGui::Text("velocity : %.2f", debug_velocity_);
	ImGui::Text("y : %.2f", monster_->GetTransform().position.y);
}
