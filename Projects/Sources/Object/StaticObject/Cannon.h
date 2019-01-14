/*
 * @file		Cannon.h
 * @brief		大砲
 * @author		戸澤翔太
 * @data		2019/01/15
 */
#ifndef _CANNON_H_
#define _CANNON_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Cannon : public Object
{
public:
	/* @brief	コンストラクタ
	 * @param	(transform)		初期姿勢
	 * @param	(model)			モデル		*/
	Cannon(void);
	/* @brief	デストラクタ		*/
	~Cannon(void);
	


	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし				*/
	void Init(void) override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void) override;

	/* @brief	更新処理(動かないためなし)		*/
	void Update(void) override {}
};

#endif // _CANNON_H_
