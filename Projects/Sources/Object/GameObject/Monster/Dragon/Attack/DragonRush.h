/*
 * @file		DragonRush.h
 * @brief		龍の突進
 * @author		戸澤翔太
 * @data		2018/11/23
 */
#ifndef _DRAGON_RUSH_H_
#define _DRAGON_RUSH_H_

#include "../../MonsterAttack.h"

class DragonRush : public MonsterAttack
{
public:
	/* @brief	コンストラクタ		*/
	DragonRush(void);
	/* @brief	デストラクタ		*/
	~DragonRush(void);
	
	/* @brief	初期化処理
	 * @param	(monster)	当たり判定の親クラスに登録
	 * @return	なし				*/
	void Init(GameObject* monster) override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void)  override;

	/* @brief	実行する準備
	 * @param	なし
	 * @return	なし
	 * @detail	この攻撃特有のアニメーション、速度を設定する		*/
	void SetMove(void) override;
	
	/* @brief	更新処理
	 * @param	なし
	 * @return	攻撃が終了したらtrue
	 * @detail	姿勢変更はなし、移動しないよう速度は0に固定
				一定時間経過後、アニメーションの速度を上げる、アニメーションが終了したら元に戻って終了		*/
	bool Update(void)  override;
	
	/* @brief	モーションの終了処理
	 * @param	なし
	 * @return	なし				*/
	void EndMove(void) override;
	
	/* @brief	Guiの更新処理
	 * @param	なし
	 * @return	なし
	 * @detail	攻撃元オブジェクトから呼ばれる		*/
	void GuiUpdate(void) override;

private:
	int cnt_;		//! カウンタ
};

#endif // _DRAGON_RUSH_H_
