#include "DragonMoveController.h"
#include "Dragon.h"

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
DragonMoveController::DragonMoveController(void) : GUI(Systems::Instance(), nullptr, "DragonController")
	, parent_(nullptr)
	, target_(nullptr)
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
void DragonMoveController::Move(VECTOR3& velocity)
{
	if (!parent_ || !target_) { return; }

	const int range = 20;

	// ������
	dir_ = target_->GetTransform().position - parent_->GetTransform().position;
	dir_.y = 0;
	dir_ = (VecLengthSq(dir_) < range * range) ? 0 : VecNorm(dir_);

	// ���蔻��
	float inputDash = 1;
	//BitSub(flag_, IS_DASH);
	//if (ctrl->Press(Input::GAMEPAD_R1, DIK_LSHIFT))
	//{
	//	inputDash = 10;
	//	BitAdd(flag_, IS_DASH);
	//}

	velocity += VECTOR3(0, 0, 1) * dir_.z * inputDash * Dragon::MOVE_SPEED;
	velocity += VECTOR3(1, 0, 0) * dir_.x * inputDash * Dragon::MOVE_SPEED;


	auto& meshAnim = parent_->GetMeshAnimation();

	Dragon::Animation tempAnim = static_cast<Dragon::Animation>(meshAnim.animation);
	if (dir_ != 0)
	{
		if (tempAnim == Dragon::Animation::WAIT1 || (tempAnim == Dragon::Animation::WALK || tempAnim == Dragon::Animation::RUN))
		{
			meshAnim.animSpeed = 0.5f;
			meshAnim.animation = static_cast<int>(Dragon::Animation::WALK);
			//if (BitCheck(flag_, IS_DASH))
			//{
			//	meshAnim.animSpeed = 0.75f;
			//	meshAnim.animation = static_cast<int>(Dragon::Animation::RUN);
			//}
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
	}


}

void DragonMoveController::GuiUpdate(void)
{
	ImGui::Text("dir : %.2f,%.2f, %.2f", dir_.x, dir_.y, dir_.z);
}
