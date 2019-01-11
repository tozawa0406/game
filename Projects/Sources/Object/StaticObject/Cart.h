/*
 * @file		Cart.h
 * @brief		手押し車
 * @author		戸澤翔太
 * @data		2018/01/1
 */
#ifndef _CART_H_
#define _CART_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Cart : public Object
{
public:
	/* @brief	コンストラクタ		*/
	Cart(void);
	/* @brief	デストラクタ		*/
	~Cart(void);


	
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

#endif // _CART_H_
