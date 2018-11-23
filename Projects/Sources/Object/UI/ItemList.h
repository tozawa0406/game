/*
 * @file		ItemList.h
 * @brief		アイテムのリスト
 * @author		戸澤翔太
 * @data		2018/11/22
 */
#ifndef _ITEM_LIST_H_
#define _ITEM_LIST_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

#include "../GameObject/Player/Player.h"

class ItemList : public Object, public GUI
{
	//! @def	裏に出るアイテム
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

	//! @def	ボタンUI
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

	/* @fn		SetPlayer
	 * @brief	プレイヤー(親)
	 * @param	(player)	設定するプレイヤー			*/
	inline void SetPlayer(Player* player) { player_ = player; }

private:
	void SetItemBack(void);
	bool SetMove(Controller& ctrl, WORD lpad, int lkey, WORD rpad, int rkey);
	void SetButtonUIEnable(bool l, bool maru, bool shikaku);

	Player* player_;

	CanvasRenderer back_;
	CanvasRenderer itemName_;
	CanvasRenderer backItemBack_[static_cast<int>(BackItem::MAX)];

	CanvasRenderer ui_[static_cast<int>(ButtonUI::MAX)];

	int flag_;
	int cnt_;
};

#endif // _ITEM_LIST_H_
