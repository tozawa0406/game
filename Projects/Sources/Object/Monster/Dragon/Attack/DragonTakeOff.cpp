#include "DragonTakeOff.h"
#include "../Dragon.h"

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����		*/
DragonTakeOff::DragonTakeOff(void) : fly_(false), debug_velocity_(0)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...					*/
DragonTakeOff::~DragonTakeOff(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	(object)	���̃N���X�ł͎g��Ȃ�
 * @return	�Ȃ�				*/
void DragonTakeOff::Init(Object* object)
{
	UNREFERENCED_PARAMETER(object);
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void DragonTakeOff::Uninit(void)
{
}

/* @fn		SetMove
 * @brief	���s���鏀��
 * @param	(mesh)			�A�j���[�V������ύX���������b�V��
 * @param	(animSpeed)		�A�j���[�V�������x
 * @param	(animNum)		�A�j���[�V�����̔ԍ�
 * @return	�Ȃ�
 * @detail	���̍U�����L�̃A�j���[�V�����A���x��ݒ肷��		*/
void DragonTakeOff::SetMove(MeshRenderer& mesh, float& animSpeed, int& animNum)
{
	// ���Ɏg�p���Ȃ�d���h�~
	if (enable_) { return; }

	MonsterAttack::SetMove(mesh, animSpeed, animNum);

	// ���x�̐ݒ�
	animSpeed = fly_ ? -0.75f : 0.75f;

	// �A�j���[�V�����̐ݒ�
	animNum = static_cast<int>(Dragon::Animation::TAKE_OFF);

	// ���ۂ̃A�j���[�V�����̐؂�ւ�
	mesh.ChangeAnimation(animNum, 15);
}

/* @fn		Update
 * @brief	�X�V����
 * @param	(tarns)		�p���̕ύX������ꍇ
 * @param	(velocity)	���x�̕ύX������ꍇ()
 * @param	(mesh)		�A�j���[�V������ύX���������b�V��
 * @param	(animSpeed)	�A�j���[�V�������x
 * @param	(animNum)	�A�j���[�V�����̔ԍ�
 * @param	(animEnd)	�A�j���[�V�������I��������ǂ���
 * @return	�U�����I��������true
 * @detail	�p���ύX�͂Ȃ��A�ړ����Ȃ��悤���x��0�ɌŒ�
			��莞�Ԍo�ߌ�A�A�j���[�V�����̑��x���グ��A�A�j���[�V�������I�������猳�ɖ߂��ďI��		*/
bool DragonTakeOff::Update(Transform& trans, VECTOR3& velocity, MeshRenderer& mesh, float& animSpeed, int& animNum, bool animEnd)
{
	// �g��Ȃ�
	UNREFERENCED_PARAMETER(trans);

	int sign = (fly_) ? -1 : 1;
	velocity.x = 0;
	velocity.z = 0;
	velocity.y += 0.1f * sign;

	debug_velocity_ = velocity.y;

	// ���o�p
	frame_++;

	// �A�j���[�V�����I��
	if (animEnd)
	{
		// ���ɖ߂�
		animSpeed = 0.75f;
		animNum = static_cast<int>(Dragon::Animation::WAIT);
		enable_ = false;
		mesh.ChangeAnimation(animNum, 5, true);
		return true;
	}
	return false;
}

/* @fn		GuiUpdate
 * @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�U�����I�u�W�F�N�g����Ă΂��		*/
void DragonTakeOff::GuiUpdate(void)
{
	ImGui::Text("frame : %d", frame_);
	ImGui::Text("velocity : %.2f", debug_velocity_);
}
