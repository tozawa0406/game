/*
 * @file		ItemList.h
 * @brief		�A�C�e���̃��X�g
 * @author		���V�đ�
 * @data		2018/11/22
 */
#ifndef _ITEM_LIST_H_
#define _ITEM_LIST_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

#include "../GameObject/Player/Player.h"

class ItemList : public Object, public GUI
{
	//! @def	�A�C�e��
	enum class BackItem : uint8
	{
		BackRight = 0,
		FrontRight,
		Center,
		FrontLeft,
		BackLeft,
		EMPTY,
		MAX
	};

	//! @def	�{�^��UI
	enum class ButtonUI : uint8
	{
		L = 0,
		MARU,
		SHIKAKU,
		MAX
	};

public:
	/* @brief	�R���X�g���N�^		*/
	ItemList(void);
	/* @brief	�f�X�g���N�^		*/
	~ItemList(void);
	


	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Init(void)   override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void) override;
	
	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Update(void) override;
	
	/* @brief	Gui�̍X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void GuiUpdate(void) override;

	/* @brief	�v���C���[(�e)�̐ݒ菈��
	 * @param	(player)	�ݒ肷��v���C���[			*/
	inline void SetPlayer(Player* player) { player_ = player; }

private:
	/* @brief	�A�C�e���̔w�i�̐ݒ�
	 * @sa		Update()
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void SetItemBack(void);
	
	/* @brief	�A�C�e���ړ��̊J�n
	 * @sa		Update()
	 * @param	(ctrl)	�R���g���[�����
	 * @param	(lpad)	���ړ��̃Q�[���p�b�h
	 * @param	(lkey)	���ړ��̃L�[
	 * @param	(rpad)	�E�ړ��̃Q�[���p�b�h
	 * @param	(rkey)	�E�ړ��̃L�[
	 * @return	�ړ��J�n������true		*/
	bool SetMove(Controller& ctrl, WORD lpad, int lkey, WORD rpad, int rkey);
	
	/* @brief	�{�^��UI��Enable�ύX
	 * @sa		Update()
	 * @param	(l)			LUI
	 * @param	(maru)		�ZUI
	 * @param	(shikaku)	��UI
	 * @param	�Ȃ�					*/
	void SetButtonUIEnable(bool l, bool maru, bool shikaku);

	/* @brief	�R���g���[���̔���
	 * @sa		Update()
	 * @param	(ctrl)	�R���g���[�����
	 * @return	�Ȃ�					*/
	void JudgeCtrl(Controller& ctrl);

	//! �v���C���[�̃|�C���^
	Player* player_;

	//! �w�i
	CanvasRenderer::Image back_;
	//! �A�C�e���̖��O�w�i
	CanvasRenderer::Image itemName_;
	//! �A�C�e���A�C�R���̔w�i
	CanvasRenderer::Image backItemBack_[static_cast<int>(BackItem::MAX)];

	//! �A�C�e���A�C�R��
	CanvasRenderer::Image item_[static_cast<int>(BackItem::MAX)];
	//! �A�C�e�����O
	CanvasRenderer::Text  text_;

	//! UI
	CanvasRenderer::Image ui_[static_cast<int>(ButtonUI::MAX)];

	int8	flag_;		//! ���E�t���O
	uint8	cnt_;		//! �J�E���^
};

#endif // _ITEM_LIST_H_
