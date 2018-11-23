#include "DragonMoveController.h"
#include "Dragon.h"
#include <random>

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
DragonMoveController::DragonMoveController(void) : GUI(Systems::Instance(), nullptr, "DragonController")
	, parent_(nullptr)
	, target_(nullptr)
	, isDash_(false)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...						*/
DragonMoveController::~DragonMoveController(void)
{
}

/* @fn		Init
 * @brief	������
 * @param	(parent)	����������
 * @return	�Ȃ�					*/
void DragonMoveController::Init(Dragon* parent)
{
	parent_ = parent;
}

/* @fn		Uninit
 * @brief	������
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void DragonMoveController::Uninit(void)
{
}

/* @fn		Uninit
 * @brief	������
 * @param	(velocity)	�^���鑬�x
 * @return	�Ȃ�					*/
void DragonMoveController::Action(int& act, uint& flag)
{
	if (!parent_ || !target_) { return; }

	const int range = 20;

	// ������
	dir_ = target_->GetTransform().position - parent_->GetTransform().position;
	dir_.y = 0;

	// ���蔻��
	float inputDash = 1;
	BitSub(flag, Dragon::IS_DASH);

	float lenghtSq = VecLengthSq(dir_);

	if (lenghtSq > 75 * 75)
	{
		isDash_ = true;
		inputDash = 10;
		BitAdd(flag, Dragon::IS_DASH);
	}
	else
	{
		// 1�񑖂����Ȃ璆�r���[�ɏI����
		if (isDash_)
		{
			inputDash = 10;
			BitAdd(flag, Dragon::IS_DASH);

			// 1���[�v������I�����Ă悢
			if (parent_->IsEndAnim()) { isDash_ = false; }
		}
	}

	dir_ = (lenghtSq < range * range) ? 0 : VecNorm(dir_);

	VECTOR3 velocity = parent_->GetVelocity();
	velocity += VECTOR3(0, 0, 1) * dir_.z * inputDash * Dragon::MOVE_SPEED;
	velocity += VECTOR3(1, 0, 0) * dir_.x * inputDash * Dragon::MOVE_SPEED;
	parent_->SetVelocity(velocity);

	auto& meshAnim = parent_->GetMeshAnimation();

	Dragon::Animation tempAnim = static_cast<Dragon::Animation>(meshAnim.animation);
	if (dir_ != 0)
	{
		act = -1;
		if (tempAnim == Dragon::Animation::WAIT1 || (tempAnim == Dragon::Animation::WALK || tempAnim == Dragon::Animation::RUN))
		{
			meshAnim.animSpeed = 0.5f;
			meshAnim.animation = static_cast<int>(Dragon::Animation::WALK);
			if (BitCheck(flag, Dragon::IS_DASH))
			{
				meshAnim.animSpeed = 0.75f;
				meshAnim.animation = static_cast<int>(Dragon::Animation::RUN);
			}
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
		}
	}
	else
	{
		if (tempAnim == Dragon::Animation::WALK || tempAnim == Dragon::Animation::RUN)
		{
			meshAnim.mesh.ChangeAnimation(static_cast<int>(Dragon::Animation::WAIT1), 30);
			meshAnim.animSpeed = 0.75f;
		}
		if (act == 0)
		{
			std::random_device randDev;
			act = (randDev() % 2) + 1;
		}
	}


}

void DragonMoveController::GuiUpdate(void)
{
	ImGui::Text("dir : %.2f,%.2f, %.2f", dir_.x, dir_.y, dir_.z);
}
