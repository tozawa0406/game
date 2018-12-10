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
	ItemList(void);
	~ItemList(void);

	void Init(void)   override;
	void Uninit(void) override;
	void Update(void) override;

	void GuiUpdate(void) override;

	/* @brief	�v���C���[(�e)�̐ݒ菈��
	 * @param	(player)	�ݒ肷��v���C���[			*/
	inline void SetPlayer(Player* player) { player_ = player; }

private:
	void SetItemBack(void);
	bool SetMove(Controller& ctrl, WORD lpad, int lkey, WORD rpad, int rkey);
	void SetButtonUIEnable(bool l, bool maru, bool shikaku);

	//! �v���C���[�̃|�C���^
	Player* player_;

	//! �w�i
	CanvasRenderer::Image back_;
	//! �A�C�e���̖��O�w�i
	CanvasRenderer::Image itemName_;
	//! �A�C�e���A�C�R���̔w�i
	CanvasRenderer::Image backItemBack_[static_cast<int>(BackItem::MAX)];

	//! UI
	CanvasRenderer::Image ui_[static_cast<int>(ButtonUI::MAX)];

	int		flag_;		//! ���E�t���O
	uint8	cnt_;		//! �J�E���^
};

#endif // _ITEM_LIST_H_
