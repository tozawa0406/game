/*
 * @file		DragonTurn.h
 * @brief		回転攻撃
 * @author		戸澤翔太
 * @data		2019/02/02
 */
#ifndef _DRAGON_TURN_H_
#define _DRAGON_TURN_H_

#include "../../MonsterAttack.h"

class DragonTurn : public MonsterAttack
{
	//! 攻撃当たり判定
	enum class Collision : uint8
	{
		WING_LR = 0,
		WING_LC,
		WING_RR,
		WING_RC,
		TAIL_5,
		TAIL_4,
		TAIL_3,
		TAIL_2,
		TAIL_1,
		MAX
	};
public:
	/* @brief	コンストラクタ		*/
	DragonTurn(void);
	/* @brief	デストラクタ		*/
	~DragonTurn(void);


	
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
	//! 当たり判定
	Collider3D::OBB* collider_[static_cast<int>(Collision::MAX)];
};

#endif // _DRAGON_TURN_H_
