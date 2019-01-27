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
	/* @brief	コンストラクタ		*/
	ItemList(void);
	/* @brief	デストラクタ		*/
	~ItemList(void);
	


	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし				*/
	void Init(void)   override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void) override;
	
	/* @brief	更新処理
	 * @param	なし
	 * @return	なし				*/
	void Update(void) override;
	
	/* @brief	Guiの更新処理
	 * @param	なし
	 * @return	なし				*/
	void GuiUpdate(void) override;

	/* @brief	プレイヤー(親)の設定処理
	 * @param	(player)	設定するプレイヤー			*/
	inline void SetPlayer(Player* player) { player_ = player; }

private:
	/* @brief	アイテムの背景の設定
	 * @sa		Update()
	 * @param	なし
	 * @return	なし				*/
	void SetItemBack(void);
	
	/* @brief	アイテム移動の開始
	 * @sa		Update()
	 * @param	(ctrl)	コントローラ情報
	 * @param	(lpad)	左移動のゲームパッド
	 * @param	(lkey)	左移動のキー
	 * @param	(rpad)	右移動のゲームパッド
	 * @param	(rkey)	右移動のキー
	 * @return	移動開始したらtrue		*/
	bool SetMove(Controller& ctrl, WORD lpad, int lkey, WORD rpad, int rkey);
	
	/* @brief	ボタンUIのEnable変更
	 * @sa		Update()
	 * @param	(l)			LUI
	 * @param	(maru)		〇UI
	 * @param	(shikaku)	□UI
	 * @param	なし					*/
	void SetButtonUIEnable(bool l, bool maru, bool shikaku);

	/* @brief	コントローラの判別
	 * @sa		Update()
	 * @param	(ctrl)	コントローラ情報
	 * @return	なし					*/
	void JudgeCtrl(Controller& ctrl);

	//! プレイヤーのポインタ
	Player* player_;

	//! 背景
	CanvasRenderer::Image back_;
	//! アイテムの名前背景
	CanvasRenderer::Image itemName_;
	//! アイテムアイコンの背景
	CanvasRenderer::Image backItemBack_[static_cast<int>(BackItem::MAX)];

	//! アイテムアイコン
	CanvasRenderer::Image item_[static_cast<int>(BackItem::MAX)];
	//! アイテム名前
	CanvasRenderer::Text  text_;

	//! UI
	CanvasRenderer::Image ui_[static_cast<int>(ButtonUI::MAX)];

	int8	flag_;		//! 左右フラグ
	uint8	cnt_;		//! カウンタ
};

#endif // _ITEM_LIST_H_
