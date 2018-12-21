/*
 * @file		KohakuSword.h
 * @brief		剣
 * @author		戸澤翔太
 * @data		2018/10/30
 */
#ifndef _KOHAKU_SWORD_H_
#define _KOHAKU_SWORD_H_

#include "Wapon.h"

class KohakuSword : public Wapon, public GUI
{
public:
	/* @brief	コンストラクタ		*/
	KohakuSword(void);
	/* @brief	デストラクタ		*/
	~KohakuSword(void);



	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし	　			*/
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

private:
	/* @brief	納刀処理
	 * @param	なし
	 * @return	なし				*/
	void PaidSword(void)  override;

	/* @brief	抜刀処理
	　* @param	なし
	　* @return	なし				*/
	void DrawnSword(void) override;

	bool debug_hit_;		//! デバッグ用
};

#endif // _KOHAKU_SWORD_H_
