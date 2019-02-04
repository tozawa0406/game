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
#include <FrameWork/Systems/Renderer/CanvasRenderer/CanvasRendererText.h>

#include "PlayerItemList.h"

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

	struct UI_ITEM_LIST
	{
		ITEM_LIST	info;
		uint8		arrayNum;
		CanvasRenderer::Image back;
		CanvasRenderer::Image img;
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

	/* @brief	現在のアイテムを返す
	 * @param	なし
	 * @return	なし				*/
	inline const ITEM_LIST& GetCurrentItem(void) const { return item_[static_cast<int>(BackItem::Center)].info; }

	/* @brief	アイテム使用
	 * @param	なし
	 * @return	なし				*/
	void UseItem(void) const { if (list_) { list_->UseItem(item_[static_cast<int>(BackItem::Center)].arrayNum); } }

private:
	/* @brief	中央アイテム情報の設定
	 * @param	なし
	 * @return	なし				*/
	void SetItemInfo(void);

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

	/* @brief	アイテムの情報を取得する
	 * @param	(list)		格納するリスト
	 * @param	(arrayNum)	アイテム側の配列番号
	 * @return	なし					*/
	void GetItemInfo(UI_ITEM_LIST& list, int arrayNum);

	/* @brief	次のアイテムの検索
	 * @sa		Init(), Update()
	 * @param	なし
	 * @return	なし					*/
	void SearchNextItem(void);

	/* @brief	次のアイテムを見つけた時
	 * @sa		SearchNextItem()
	 * @param	(i)				現在の走査位置
	 * @param	(arrangement)	配置
	 * @return	見つけたらtrue			*/
	bool FindNext(int i, BackItem arrangement);

	//! 背景
	CanvasRenderer::Image back_;
	//! アイテムの名前背景
	CanvasRenderer::Image itemName_;
	//! アイテムのUI表示
	UI_ITEM_LIST item_[static_cast<int>(BackItem::MAX)];
	//! アイテム名前
	CanvasRenderer::Text  text_;
	//! アイテム使用UI
	CanvasRenderer::Image useUI_;
	//! 所持数
	CanvasRenderer::Text  possession_;
	//! 所持数背景
	CanvasRenderer::Image possessionBack_;


	//! UI
	CanvasRenderer::Image ui_[static_cast<int>(ButtonUI::MAX)];

	PlayerItemList* list_;

	int8	flag_;		//! 左右フラグ
	uint8	cnt_;		//! カウンタ
};

#endif // _ITEM_LIST_H_
