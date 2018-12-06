#include "AttackBaseState.h"
#include "../DrawnState/DrawnWaitState.h"

//! @def	�A�j���[�V�������x
static constexpr float ANIM_SPEED = 0.6f;
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

/* @brief	�R���X�g���N�^			*/
AttackBaseState::AttackBaseState(void) : dir_(VECTOR3(0))
	, next_(false)
	, debug_nextFrame_(false)
	, animSpeed_(0)
	, animation_(Player::Animation::Wait)
	, changeFrame_(0)
	, collisionStart_(0)
	, collisionEnd_(0)
	, moveStart_(0)
	, moveEnd_(0)
	, move_(0)
{
}

/* @brief	�f�X�g���N�^			*/
AttackBaseState::~AttackBaseState(void)
{
}

/* @brief	����������
 * @param	(player)	�v���C���[�ւ̃|�C���^
 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
 * @return	�Ȃ�					*/
void AttackBaseState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	next_ = false;

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// �ŏ��̍U�����[�V����
	meshAnim.animSpeed = animSpeed_;
	meshAnim.animation = static_cast<int>(animation_);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, changeFrame_);

	// ���͕����ɉ���A���͂��Ȃ��Ƃ��͑O�ɉ��
	if (ctrl_)
	{
		VECTOR2 inputDir;
		// Input
		inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// ���K��
		inputDir = VecNorm(inputDir);

		// ��������
		if (inputDir == 0)
		{
			dir_ -= player->GetFront();
		}
		else
		{
			if (const auto& camera_ = player->GetCamera())
			{
				dir_ = VECTOR3(0);
				dir_ += camera_->GetFrontXPlane() * inputDir.y;
				dir_ -= camera_->GetRightXPlane() * inputDir.x;
				dir_ = VecNorm(dir_);
			}
		}
	}
}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void AttackBaseState::Uninit(void)
{
}

/* @breif	�X�V����
 * @param	�Ȃ�
 * @return	���̃X�e�[�g			*/
PlayerState* AttackBaseState::Update(void)
{
	if (!player_) { return nullptr; }

	Wapon* wapon = player_->GetWapon();
	if (!wapon) { return nullptr; }

	auto& meshAnim	= player_->GetMeshAnimation();
	int max			= meshAnim.mesh.GetMaxAnimation();

	// �A�j���[�V�����̏��
	float pattern = meshAnim.mesh.GetPattern();

	if (pattern > collisionStart_)
	{
		if (!wapon->IsAttack())
		{
			// ����̍U�����J�n
			wapon->AttackStart();
		}
	}
	if (pattern > collisionEnd_)
	{
		// ����̍U�����I��
		wapon->AttackEnd();
	}

	if(moveStart_ < pattern && pattern < moveEnd_)
	{
		auto v = player_->GetVelocity();
		if (const auto& camera = player_->GetCamera())
		{
			v += dir_ * move_;
		}
		player_->SetVelocity(v);
	}

	// �A�j���[�V�����̏I��
	if (pattern >= max - 1)
	{
		// �����ҋ@��Ԃɖ߂�
		meshAnim.animation = static_cast<int>(Player::Animation::SetupWait);
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15, true);

		return new DrawnWaitState;
	}

	// �U���R�}���h�Ŏ��̍U��
	if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U)) { next_ = true; }

	return nullptr;
}

/* @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�v���C���[����Ă΂��	*/
void AttackBaseState::GuiUpdate(void)
{
	if (const auto& systems = Systems::Instance())
	{
		if (const auto& debug = systems->GetDebug())
		{
			if (debug->GetDebugPause())
			{
				if (ImGui::Button("Next Frame"))
				{
					debug_nextFrame_ = true;
					debug->SetDebugPause(false);
				}
				ImGui::SameLine();
				ImGui::Text("pattern : %.2f", player_->GetMeshAnimation().mesh.GetPattern());
			}
			else
			{
				if (debug_nextFrame_)
				{
					debug->SetDebugPause(true);
				}
				debug_nextFrame_ = false;
			}
		}
	}
}
