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

class MonsterAttack : public GUI
{
public:
	/* @brief	�R���X�g���N�^		*/
	MonsterAttack(void)	: GUI(Systems::Instance(), nullptr, "attack"), enable_(false), frame_(0), debug_nextFrame_(false)	{}
	/* @brief	�f�X�g���N�^		*/
	virtual ~MonsterAttack(void)	{}

	/* @brief	����������			*/
	virtual void Init(GameObject* monster) { monster_ = monster; }
	/* @brief	�㏈��				*/
	virtual void Uninit(void) = 0;
	/* @brief	��������			*/
	virtual void SetMove(void) { enable_ = true; frame_ = 0; }
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
	//! �g�p�t���O
	bool	enable_;
	//! �U�����̓����蔻�蓙���f�p
	int		frame_;
	//! �f�o�b�O�̃t���[������
	bool	debug_nextFrame_;
	//! �U�����s�����̃����X�^�[�̃|�C���^
	GameObject* monster_;
};

#endif // _MONSTAER_ATTACK_H_
