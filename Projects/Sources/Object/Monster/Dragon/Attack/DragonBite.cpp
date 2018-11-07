#include "DragonBite.h"
#include "../Dragon.h"

//! @def	�A�j���[�V�����̑��x��ς���(��������)�^�C�~���O
static constexpr int CHANGE_FRAME = 20;

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����		*/
DragonBite::DragonBite(void) : debug_speed_(0), debug_changeFrame_(CHANGE_FRAME)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...					*/
DragonBite::~DragonBite(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	(object)	���̃N���X�ł͎g��Ȃ�
 * @return	�Ȃ�				*/
void DragonBite::Init(Object* object)
{
	UNREFERENCED_PARAMETER(object);
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void DragonBite::Uninit(void)
{
}

/* @fn		SetMove
 * @brief	���s���鏀��
 * @param	(mesh)			�A�j���[�V������ύX���������b�V��
 * @param	(animSpeed)		�A�j���[�V�������x
 * @param	(animNum)		�A�j���[�V�����̔ԍ�
 * @return	�Ȃ�
 * @detail	���̍U�����L�̃A�j���[�V�����A���x��ݒ肷��		*/
void DragonBite::SetMove(MeshRenderer& mesh, float& animSpeed, int& animNum)
{
	// ���Ɏg�p���Ȃ�d���h�~
	if (enable_) { return; }

	MonsterAttack::SetMove(mesh, animSpeed, animNum);

	// ���x�̐ݒ�
	animSpeed	 = 0.4f;
	debug_speed_ = animSpeed;

	// �A�j���[�V�����̐ݒ�
	animNum   = static_cast<int>(Dragon::Animation::BITE);

	// ���ۂ̐؂�ւ�
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
bool DragonBite::Update(Transform& trans, VECTOR3& velocity, MeshRenderer& mesh, float& animSpeed, int& animNum, bool animEnd)
{
	// �g��Ȃ�
	UNREFERENCED_PARAMETER(trans);

	// �ړ��͂����Ȃ�
	velocity = VECTOR3(0);

	// ���o�p
	frame_++;

	// ���̎��Ԃ𒴂�����A�j���[�V�������x��ς���
#ifdef _SELF_DEBUG
	// �f�o�b�O�p
	if (frame_ > debug_changeFrame_)
#else
	if (frame_ > CHANGE_FRAME)
#endif
	{
		animSpeed	 = 0.75f; 
		debug_speed_ = animSpeed;
	}

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
void DragonBite::GuiUpdate(void)
{
	ImGui::Text("frame : %d", frame_);
	ImGui::Text("speed : %.2f", debug_speed_);
	ImGui::DragInt("changeFrame", &debug_changeFrame_);
}
