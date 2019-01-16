/*
 * @file		Tent.h
 * @brief		テント
 * @author		戸澤翔太
 * @data		2018/01/16
 */
#ifndef _TENT_H_
#define _TENT_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Tent : public Object
{
public:
	/* @brief	コンストラクタ		*/
	Tent(void);
	/* @brief	デストラクタ		*/
	~Tent(void);


	
	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし				*/
	void Init(void) override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし						*/
	void Uninit(void) override;

	/* @brief	更新処理(動かないためなし)		*/
	void Update(void) override {}

private:
	//! メッシュ
	MeshRenderer mesh_;
	//! 当たり判定
	Collider3D::OBB* collider_;
};

#endif // _TENT_H_
