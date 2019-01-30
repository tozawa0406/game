/*
 * @file		Player.h
 * @brief		�v���C���[�̓���
 * @author		���V�đ�
 * @data		2018/11/02
 */
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <FrameWork/Systems/Camera/CameraManager.h>
#include "../GameObject.h"
#include "../../Wapon/Wapon.h"

#include "../../UI/PlayerLife.h"
#include "../../UI/ItemList.h"

class ItemList;
class PlayerState;
class Player : public GameObject, public GUI
{
protected:
	//! @def	�A�j���[�V�����̑��x
	static constexpr float ANIMATION_DEFAULT = 0.75f;

public:
	//! @enum	�A�j���[�V����
	enum class Animation
	{
		Wait = 0,
		WaitTime1,
		WaitTime2,
		Walk,
		Run,
		Roll,
		Dive,
		KnockBack,
		KnockOut,
		Die,
		Earplug,
		Standup,
		Heal,
		WalkSetup,
		Setup,
		SetupDrawn,
		SetupWait,
		SetupWalk,
		Slash_1,
		Slash_2,
		Slash_3,
		MAX
	};
	//! @def	�X�^�~�i�񕜒l
	static constexpr float ADD_STAMINA = 0.2f;

	/* @brief	�R���X�g���N�^		*/
	Player(const VECTOR3& position);
	/* @brief	�f�X�g���N�^		*/
	virtual ~Player(void);


	
	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	virtual void Init(void)   override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	virtual void Uninit(void) override;
	
	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	virtual void Update(void) override;
	
	/* @brief	�f�o�b�O�p�`��X�V
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void GuiUpdate(void) override;
	
	/* @brief	�_���[�W����
	 * @param	(damage)	�_���[�W
	 * @return	�Ȃ�				*/
	void Hit(int damage) override;

	/* @brief	�����ڍs����
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Earplug(uint8 attack);

	/* @brief	����̐ݒ�
	 * @param	(wapon)		����			*/
	inline void SetWapon(Wapon* wapon) { wapon_ = wapon; wapon->SetParent(transform_, body_, hand_); }

	/* @brief	�J�����擾����				*/
	inline Camera*	GetCamera(void)	{ return camera_;		}
	/* @brief	����̎擾����				*/
	inline Wapon*	GetWapon(void)	{ return wapon_;		}

	/* @brief	�X�e�[�g�Őݒ�				*/
	inline void SetPosition(const VECTOR3& position) { transform_.position = position; }

	/* @brief	�����蔻��̎擾����		*/
	inline Collider3D::OBB* GetCollider(void) { return collider_; }

	/* @brief	�h�䓖���蔻��̎擾����		*/
	inline Collider3D::OBB* GetDefenseCollider(void) { return colliderDefense_; }

	/* @brief	�f�o�b�O�p�F�G�̑��쒆�Ƀv���C���[�𑀍삵�Ȃ�	*/
	inline bool IsDebugCtrl(void) { return (cameraManager_ && cameraManager_->GetMainNum() != 0) ? true : false; }

	/* @brief	�[�������̕ύX				*/
	inline void SetDraw(bool isDraw) { isDraw_ = isDraw;	}
	/* @brief	�[�������̎擾				*/
	inline bool IsDraw(void)		 { return isDraw_;		}

	/* @brief	�X�^�~�i�̎擾				*/
	inline float GetStamina(void)			{ return stamina_;		}
	/* @brief	�X�^�~�i�̐ݒ�				*/
	inline void  SetStamina(float stamina)	{ stamina_ = stamina;	}

	/* @brief	���C�t�ƃX�^�~�i�̏���l�̎擾
	 * @return	x:���C�t y:�X�^�~�i			*/
	inline VECTOR2 GetMaxLifeStamina(void) { return VECTOR2(maxLife_, maxStamina_); }

	inline void AddMaxLife(void)	{ maxLife_		= min(150, maxLife_	   + 25); }
	inline void AddMaxStamina(void) { maxStamina_	= min(150, maxStamina_ + 25); }

	/* @brief	�񕜏���
	 * @param	(recovery)	�񕜒l
	 * @return	�Ȃ�						*/
	inline void Recovery(int recovery) { life_ = min(life_ + recovery, maxLife_); }

	inline const ItemList* GetItemLIst(void) { return itemList_; }

private:
	/* @brief	���S����
	 * @sa		Update()
	 * @param	�Ȃ�
	 * @return	����ł�����true			*/
	bool IsDedJudge(void);

	ItemList*		itemList_;			//! �A�C�e�����X�g
	PlayerState*	state_;				//! �v���C���[�̃X�e�[�g
	float			stamina_;			//! �X�^�~�i
	int				staminaCnt_;		//! �X�^�~�i�ő�l�����̃J�E���^
	int				maxLife_;
	float			maxStamina_;
	bool			isDraw_;			//! �����t���O

	Wapon*			wapon_;				//! ����
	CameraManager*	cameraManager_;		//! �J�����}�l�[�W���[
	Camera*			camera_;			//! �J����

	Collider3D::OBB* collider_;			//! �����蔻��
	Collider3D::OBB* colliderDefense_;			//! �����蔻��

	const MATRIX*	body_;				//! �̂̃{�[���s��
	const MATRIX*	hand_;				//! �E��̃{�[���s��

	uint8			hitAttack_;			//! �󂯂��U����2�x�󂯂Ȃ��悤��
};

#endif // _PLAYER_H_
