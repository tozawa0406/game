/*
 * @file		DragonTakeOff.h
 * @brief		龍の翼攻撃
 * @author		戸澤翔太
 * @data		2018/11/07
 */
#ifndef _DRAGON_TAKE_OFF_H_
#define _DRAGON_TAKE_OFF_H_

#include "../../MonsterAttack.h"

class DragonTakeOff : public MonsterAttack
{
public:
	/* @brief	コンストラクタ		*/
	DragonTakeOff(void);
	/* @brief	コンストラクタ		*/
	~DragonTakeOff(void);
	
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

	/* @brief	飛行状態遷移		*/
	inline void SetFly(bool fly) { fly_ = fly; }

private:
	bool fly_;		//! 飛んでいるフラグ

	float debug_velocity_;
};

#endif // _DRAGON_TAKE_OFF_H_
