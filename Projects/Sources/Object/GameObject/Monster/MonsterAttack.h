/*
 * @file		MonsterAttack.h
 * @brief		�����X�^�[�̍U�������N���X�̊��N���X
 * @author		���V�đ�
 * @data		2018/11/06
 */
#ifndef _MONSTER_ATTACK_H_
#define _MONSTER_ATTACK_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>

#include "../GameObject.h"
#include <random>

class MonsterAttack : public GUI
{
public:
	/* @brief	�R���X�g���N�^		*/
	MonsterAttack(void)	: GUI(Systems::Instance(), nullptr, "attack"), enable_(false), debug_nextFrame_(false), attackManager_(nullptr)	{}
	/* @brief	�f�X�g���N�^		*/
	virtual ~MonsterAttack(void)	{}

	/* @brief	����������			*/
	virtual void Init(GameObject* monster) { monster_ = monster; }
	/* @brief	�㏈��				*/
	virtual void Uninit(void) = 0;
	/* @brief	��������			*/
	virtual void SetMove(void) 
	{
		enable_ = true;
		if (!attackManager_) { attackManager_ = monster_->GetAttackManager(); }
		attackID_ = attackManager_->CreateAttackID();
	}

	/* @brief	�X�V����			*/
	virtual bool Update(void) = 0;
	/* @brief	�I������			*/
	virtual void EndMove(void)   = 0;

	/* @brief	Gui�X�V����			*/
	virtual void GuiUpdate(void) override
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

protected:
	//! �f�t�H���g�̃A�j���[�V�������x
	static constexpr float DEFAULT_ANIMATION_SPEED = 0.75f;
	//! @def	�A�j���[�V�����ύX���x
	static constexpr int   ANIMATION_CHANGE_FRAME5 = 5;
	//! @def	�A�j���[�V�����ύX���x
	static constexpr int   ANIMATION_CHANGE_FRAME15 = 15;
	//! @def	�A�j���[�V�����ύX���x
	static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;

	const AttackManager*	attackManager_;
	//! �g�p�t���O
	bool	enable_;
	//! �f�o�b�O�̃t���[������
	bool	debug_nextFrame_;
	//! �U�����s�����̃����X�^�[�̃|�C���^
	GameObject* monster_;

	//! �O��̍U��ID
	uint8 attackID_;
};

#endif // _MONSTAER_ATTACK_H_
