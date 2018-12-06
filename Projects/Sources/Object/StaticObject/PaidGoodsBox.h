/*
 * @file		PaidGoodsBox.h
 * @brief		支給品箱
 * @author		戸澤翔太
 * @data		2018/10/30
 */
#ifndef _PAID_GOODS_BOX_H_
#define _PAID_GOODS_BOX_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class PaidGoodsBox : public Object
{
public:
	PaidGoodsBox(void);
	~PaidGoodsBox(void);

	void Init(void) override;
	void Uninit(void) override;

	/* @brief	更新処理(動かないためなし)		*/
	void Update(void) override {}

private:
	//! メッシュ
	MeshRenderer mesh_;
	//! 当たり判定
	Collider3D::OBB* collider_;
};

#endif // _PAID_GOODS_BOX_H_
