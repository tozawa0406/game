//-----------------------------------------------------------------------------
//
//	Auther : ���V�đ�
//-----------------------------------------------------------------------------
#include "PlayerHunter.h"
#include <FrameWork/Systems/Input/Controller.h>
#include "../../Scene/GameScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Object/ObjectManager.h>
#include <random>

#include <FrameWork/Graphics/DirectX11/DirectX11Wrapper.h>

#include "PlayerState/PaidState.h"
#include "PlayerState/DrawnState.h"

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

	state_ = new PaidState;
	if (state_) { state_->Init(this, GetCtrl(0)); }
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

	COLOR c = COLOR(1, 1, 1, 1);
	const auto& hits = collider_->HitCollider();
	for (auto& hit : hits)
	{
		if (const auto& o = hit->GetParent())
		{
			if (o->GetTag() == Object::Tag::ENEMY && hit->GetRendererColor() == COLOR(1, 0, 0, 1))
			{
				c = COLOR(1, 0, 0, 1);
			}
		}
	}
	meshAnim_.mesh.material.diffuse = c;

#ifdef _SELF_DEBUG
	// �f�o�b�O�p�A�G�̑��쒆�̓v���C���[�̑���͂��Ȃ�
	if (cameraManager_ && cameraManager_->GetMainNum() != 0) { return; }
#endif

	Setup();

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
		meshAnim_.animation = static_cast<int>(Animation::Setup);
		meshAnim_.mesh.ChangeAnimation(meshAnim_.animation,30);

		// �[�����͋t�Đ�
		if (isDraw) { meshAnim_.animSpeed = -0.75f; }

		// �[���������t���O�𗧂Ă�
		BitAdd(flag_, IS_SETUP);
	}

	// �A�j���[�V������
	if (meshAnim_.animation == static_cast<int>(Animation::Setup))
	{
		// �[���������ł���A�A�j���[�V���������ȉ�
		if (BitCheck(flag_, IS_SETUP) && fabs(meshAnim_.mesh.GetPattern()) >= 30)
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
			meshAnim_.animSpeed = 0.6f;
			BitAdd(flag_, IS_ATTACK);
			meshAnim_.animation = static_cast<int>(Animation::Slash_1);
			meshAnim_.mesh.ChangeAnimation(meshAnim_.animation, 15);
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
		int animMax = meshAnim_.mesh.GetMaxAnimation();
		int pattern = static_cast<int>(meshAnim_.mesh.GetPattern());

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
				meshAnim_.animation = (meshAnim_.animation < static_cast<int>(Animation::Slash_3)) ? static_cast<int>(meshAnim_.animation) + 1 : static_cast<int>(Animation::Slash_1);
				meshAnim_.mesh.ChangeAnimation(meshAnim_.animation, 15);
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
			meshAnim_.animation = static_cast<int>(Animation::SetupWait);
			meshAnim_.mesh.ChangeAnimation(meshAnim_.animation, 15, true);
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
