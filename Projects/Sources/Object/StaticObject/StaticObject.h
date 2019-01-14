/*
 * @file		StaticObject.h
 * @brief		動かないオブジェクト
 * @author		戸澤翔太
 * @data		2019/01/15
 */
#ifndef _STATIC_OBJECT_H_
#define _STATIC_OBJECT_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class StaticObject : public Object
{
public:
	/* @brief	コンストラクタ
	 * @param	(model)	モデル		*/
	StaticObject(uint8 model);
	/* @brief	デストラクタ		*/
	~StaticObject(void);
	


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

	/* @brief	姿勢の設定
	 * @param	(trans)		姿勢	*/
	inline void SetTransform(Transform trans) { transform_ = trans; }

	/* @brief	当たり判定の設定
	 * @param	(position)	オフセット位置
	 * @param	(rotation)	オフセット回転
	 * @param	(size)		当たり判定の大きさ		*/
	void SetColliderOffset(VECTOR3 position, VECTOR3 rotation, VECTOR3 size);

private:
	//! 当たり判定
	Collider3D::OBB*	collider_;
	//! メッシュ
	MeshRenderer		mesh_;
	//! モデル
	uint8				model_;
};

#endif // _STATIC_OBJECT_H_
