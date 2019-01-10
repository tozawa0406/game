/*
 * @file		WallParts.h
 * @brief		壁の小さなオブジェクト
 * @author		戸澤翔太
 * @data		2019/01/10
 */
#ifndef _WALL_PARTS_H_
#define _WALL_PARTS_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class WallParts : public Object
{
public:
	/* @brief	コンストラクタ
	 * @param	(transform)		初期姿勢
	 * @param	(model)			モデル		*/
	WallParts(void);
	/* @brief	デストラクタ		*/
	~WallParts(void);
	


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
	 * @param	(trans)		3D姿勢				*/
	inline void SetTransform(const Transform& trans) { transform_ = trans; }

	/* @brief	モデルの設定
	 * @param	(model)		モデルの番号		*/
	inline void SetModelNum(Model::Game model) { mesh_.Init(Systems::Instance(), static_cast<int>(model), &transform_); }

	/* @brief	当たり判定の大きさ指定
	 * @param	(size)		大きさ				*/
	inline void SetColliderSize(const VECTOR3 size) { if (collider_) { collider_->SetSize(size); } }
	
	/* @brief	当たり判定のオフセット設定
	 * @param	(trans)		3D姿勢				*/
	void SetColliderOffset(const VECTOR3& position, const VECTOR3& rotation);

private:
	//! 当たり判定
	Collider3D::OBB*	collider_;
	//! メッシュ
	MeshRenderer		mesh_;
};

#endif // _WALL_A_H_
