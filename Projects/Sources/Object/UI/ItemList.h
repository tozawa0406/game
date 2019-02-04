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
#include <FrameWork/Systems/Renderer/CanvasRenderer/CanvasRendererText.h>

#include "PlayerItemList.h"

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

	struct UI_ITEM_LIST
	{
		ITEM_LIST	info;
		uint8		arrayNum;
		CanvasRenderer::Image back;
		CanvasRenderer::Image img;
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

	/* @brief	���݂̃A�C�e����Ԃ�
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	inline const ITEM_LIST& GetCurrentItem(void) const { return item_[static_cast<int>(BackItem::Center)].info; }

	/* @brief	�A�C�e���g�p
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void UseItem(void) const { if (list_) { list_->UseItem(item_[static_cast<int>(BackItem::Center)].arrayNum); } }

private:
	/* @brief	�����A�C�e�����̐ݒ�
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void SetItemInfo(void);

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

	/* @brief	�A�C�e���̏����擾����
	 * @param	(list)		�i�[���郊�X�g
	 * @param	(arrayNum)	�A�C�e�����̔z��ԍ�
	 * @return	�Ȃ�					*/
	void GetItemInfo(UI_ITEM_LIST& list, int arrayNum);

	/* @brief	���̃A�C�e���̌���
	 * @sa		Init(), Update()
	 * @param	�Ȃ�
	 * @return	�Ȃ�					*/
	void SearchNextItem(void);

	/* @brief	���̃A�C�e������������
	 * @sa		SearchNextItem()
	 * @param	(i)				���݂̑����ʒu
	 * @param	(arrangement)	�z�u
	 * @return	��������true			*/
	bool FindNext(int i, BackItem arrangement);

	//! �w�i
	CanvasRenderer::Image back_;
	//! �A�C�e���̖��O�w�i
	CanvasRenderer::Image itemName_;
	//! �A�C�e����UI�\��
	UI_ITEM_LIST item_[static_cast<int>(BackItem::MAX)];
	//! �A�C�e�����O
	CanvasRenderer::Text  text_;
	//! �A�C�e���g�pUI
	CanvasRenderer::Image useUI_;
	//! ������
	CanvasRenderer::Text  possession_;
	//! �������w�i
	CanvasRenderer::Image possessionBack_;


	//! UI
	CanvasRenderer::Image ui_[static_cast<int>(ButtonUI::MAX)];

	PlayerItemList* list_;

	int8	flag_;		//! ���E�t���O
	uint8	cnt_;		//! �J�E���^
};

#endif // _ITEM_LIST_H_
