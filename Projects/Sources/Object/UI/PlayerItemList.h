/*
 * @file		PlayerItemList.h
 * @brief		死亡処理
 * @author		戸澤翔太
 * @data		2018/11/14
 */
#ifndef _PLAYER_ITEM_LIST_H_
#define _PLAYER_ITEM_LIST_H_

#include <FrameWork/Define/Define.h>

//! @enum	アイテムのリスト
enum class ItemID : int8
{
	UNKNOWN  = -1,
	Recovery = 0,
	Rations,
	MAX
};

struct ITEM_LIST
{
	ItemID itemID;
	uint8  possession;
};

class PlayerItemList
{
public:
	static constexpr uint8 MAX_ITEM = 5;

	/* @brief	コンストラクタ		*/
	PlayerItemList(void);
	/* @brief	デストラクタ		*/
	~PlayerItemList(void);


	
	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし				*/
	void Init(void);
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void);
		
	/* @brief	Guiの更新処理
	 * @param	なし
	 * @return	なし
	 * @detail	プレイヤーから呼ばれる	*/
	void GuiUpdate(void);

	/* @brief	アイテムの取得処理
	 * @param	(addItem)	追加するアイテムの情報
	 * @return	所持品が一杯の時false	*/
	bool AddItem(ITEM_LIST addItem);

	/* @brief	中央のアイテムの取得処理
	 * @param	(arrayNum)		配列番号
	 * @return	アイテムの情報			*/
	inline ITEM_LIST GetItemInfo(int arrayNum) { return itemList_[arrayNum]; }

private:
	// アイテムのリスト
	ITEM_LIST	itemList_[MAX_ITEM];
};

#endif // _DIE_STATE_H_
