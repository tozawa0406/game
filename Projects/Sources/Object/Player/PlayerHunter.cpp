//-----------------------------------------------------------------------------
//
//	Auther : ���V�đ�
//-----------------------------------------------------------------------------
#include "PlayerHunter.h"
#include <FrameWork/Systems/Input/Controller.h>
#include "../../Scene/EachScene/GameScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Object/ObjectManager.h>
#include <random>

#include <FrameWork/Graphics/DirectX11/DirectX11Wrapper.h>

//! @def	����A�j���[�V�����̏I���t���[��
static constexpr int END_AVOIDANCE_ANIMATION = 30;

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
PlayerHunter::PlayerHunter(void)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...						*/
PlayerHunter::~PlayerHunter(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	���N���X�̏����Ăяo��		*/
void PlayerHunter::Init(void)
{
	PlayerMove::Init();
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	���N���X�̏����Ăяo��		*/
void PlayerHunter::Uninit(void)
{
	PlayerMove::Uninit();
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	���N���X�̏����Ăяo���A
			�[�����������A��������A�U������		*/
void PlayerHunter::Update(void)
{
	PlayerMove::Update();

	Setup();

	Avoidance();

	Attack();

}

/* @fn		Setup
 * @brief	�[����������
 * @sa		Update()
 * @param	�Ȃ�
 * @return	�Ȃ�							*/
void PlayerHunter::Setup(void)
{
	// ��𒆂͂ł��Ȃ�
	if (BitCheck(flag_, IS_AVOIDANCE)) { return; }

	// �ǂ��g���̂ňꎞ�ϐ��ɓ����
	bool isDraw = BitCheck(flag_, IS_DRAWN);

	// �[�����Ɣ������ŃL�[�ƃR���g���[���{�^����ς���
	uint key  = (isDraw) ? DIK_H: DIK_U;
	WORD ctrl = (isDraw) ? Input::GAMEPAD_SQUARE : Input::GAMEPAD_TRIANGLE;

	// ����
	if (GetCtrl(0)->Trigger(ctrl, key))
	{
		// �A�j���[�V����
		animation_ = Animation::Setup;
		mesh_.ChangeAnimation((int)animation_,30);

		// �[�����͋t�Đ�
		if (isDraw) { animCnt_ = -0.75f; }

		// �[���������t���O�𗧂Ă�
		BitAdd(flag_, IS_SETUP);
	}

	// �A�j���[�V������
	if (animation_ == Animation::Setup)
	{
		// �[���������ł���A�A�j���[�V���������ȉ�
		if (BitCheck(flag_, IS_SETUP) && fabs(mesh_.GetPattern()) >= 30)
		{
			// ����̏�Ԑ؂�ւ�
			if (wapon_)
			{
				wapon_->Setup(isDraw);
			}
			// �[�������I��
			BitSub(flag_, IS_SETUP);
			isDraw ? BitSub(flag_, IS_DRAWN) : BitAdd(flag_, IS_DRAWN);
		}
	}
}

// �������
/* @fn		Avoidance
 * @brief	�������
 * @sa		Update()
 * @param	�Ȃ�
 * @return	�Ȃ�							*/
void PlayerHunter::Avoidance(void)
{
	// ����
	if (GetCtrl(0)->Trigger(Input::GAMEPAD_CROSS, DIK_M))
	{
		if (wapon_)
		{
			wapon_->AttackEnd();
		}
		animCnt_     =	ANIMATION_DEFAULT;
		BitAdd(flag_, IS_AVOIDANCE);
		animation_   = Animation::Roll;
		BitSub(flag_, IS_SETUP);
		BitSub(flag_, IS_ATTACK);
		mesh_.ChangeAnimation((int)animation_, 15);

		// ���͕����ɉ���A���͂��Ȃ��Ƃ��͑O�ɉ��
		if (inputDir_ == 0)
		{
			avoidanceDir_ = -front_;
		}
		else
		{
			if (camera_)
			{
				avoidanceDir_ = VECTOR3(0);
				avoidanceDir_ += camera_->GetFrontXPlane() * inputDir_.y;
				avoidanceDir_ -= camera_->GetRightXPlane() * inputDir_.x;
				avoidanceDir_ = VecNorm(avoidanceDir_);
			}
		}
	}

	if (animation_ == Animation::Roll)
	{
		if (BitCheck(flag_, IS_AVOIDANCE) && mesh_.GetPattern() >= END_AVOIDANCE_ANIMATION)
		{				
			if (BitCheck(flag_, IS_DRAWN))
			{
				animation_ = (inputDir_ != 0) ? Animation::SetupWalk : Animation::SetupWait;
			}
			else
			{
				animation_ = (inputDir_ != 0) ? (inputDash_ == 1) ? Animation::Walk : Animation::Run : Animation::Wait;
			}

			mesh_.ChangeAnimation((int)animation_, 15, false);
			velocity_ *= 0.5f;
			BitSub(flag_, IS_AVOIDANCE);
		}
	}
}

/* @fn		Attack
 * @brief	�U������
 * @sa		Update()
 * @param	�Ȃ�
 * @return	�Ȃ�			*/
void PlayerHunter::Attack(void)
{
	// ��𒆂ł��鎞�A�������łȂ��Ƃ��͕Ԃ�
	if (BitCheck(flag_, IS_AVOIDANCE) || !BitCheck(flag_, IS_DRAWN)) { return; }

	// ����
	if (GetCtrl(0)->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
	{
		// �U�����Ă��Ȃ���
		if (!BitCheck(flag_, IS_ATTACK))
		{
			if (wapon_)
			{
				wapon_->AttackStart();
			}
			// �ŏ��̍U�����[�V����
			animCnt_ = 0.75f;
			BitAdd(flag_, IS_ATTACK);
			animation_ = Animation::Slash_1;
			mesh_.ChangeAnimation((int)animation_, 15);
		}
		else
		{
			// �U�����Ȃ玟�̍U�����s��
			BitAdd(flag_, IS_NEXT_SLASH);
		}
	}

	// �U����
	if (BitCheck(flag_, IS_ATTACK))
	{
		// �A�j���[�V�����̏��
		int animMax = mesh_.GetMaxAnimation();
		int pattern = static_cast<int>(mesh_.GetPattern());

		// �I���O��
		if (pattern > (Quarter(animMax) * 3))
		{
			// ���̓��͂�����
			if (BitCheck(flag_, IS_NEXT_SLASH))
			{
				if (wapon_)
				{
					wapon_->AttackStart();
				}
				// ���̍U�����s��
				BitSub(flag_, IS_NEXT_SLASH);
				// �A�j���[�V�������Ō�܂ōs������ŏ��ɖ߂�
				animation_ = (animation_ < Animation::Slash_3) ? static_cast<Animation>(static_cast<int>(animation_) + 1) : Animation::Slash_1;
				mesh_.ChangeAnimation((int)animation_, 15);
			}
		}

		// �A�j���[�V�����̏I��
		if (BitCheck(flag_, IS_END_ANIMATION))
		{
			if (wapon_)
			{
				wapon_->AttackEnd();
			}
			// �����ҋ@��Ԃɖ߂�
			BitSub(flag_, IS_ATTACK);
			animation_ = Animation::SetupWait;
			mesh_.ChangeAnimation((int)animation_, 15, true);
		}
	}
}

/* @fn		GuiUpdate
 * @brief	�f�o�b�O�p�`��X�V
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void PlayerHunter::GuiUpdate(void)
{
	PlayerMove::GuiUpdate();
}
