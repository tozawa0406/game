/*
 * @file		DragonScream.h
 * @brief		龍の咆哮
 * @author		戸澤翔太
 * @data		2018/11/06
 */
#ifndef _DRAGON_SCREAM_H_
#define _DRAGON_SCREAM_H_

#include "../../MonsterAttack.h"

class DragonScream : public MonsterAttack
{
public:
	/* @brief	コンストラクタ		*/
	DragonScream(void);
	/* @brief	デストラクタ		*/
	~DragonScream(void);
	
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
};

#endif // _DRAGON_SCREAM_H_
