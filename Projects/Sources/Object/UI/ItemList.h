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
	//! @def	アイテム
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

	/* @brief	プレイヤー(親)の設定処理
	 * @param	(player)	設定するプレイヤー			*/
	inline void SetPlayer(Player* player) { player_ = player; }

private:
	void SetItemBack(void);
	bool SetMove(Controller& ctrl, WORD lpad, int lkey, WORD rpad, int rkey);
	void SetButtonUIEnable(bool l, bool maru, bool shikaku);

	//! プレイヤーのポインタ
	Player* player_;

	//! 背景
	CanvasRenderer::Image back_;
	//! アイテムの名前背景
	CanvasRenderer::Image itemName_;
	//! アイテムアイコンの背景
	CanvasRenderer::Image backItemBack_[static_cast<int>(BackItem::MAX)];

	//! UI
	CanvasRenderer::Image ui_[static_cast<int>(ButtonUI::MAX)];

	int		flag_;		//! 左右フラグ
	uint8	cnt_;		//! カウンタ
};

#endif // _ITEM_LIST_H_
