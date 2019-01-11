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
	WallParts(Model::Game model);
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

private:
	//! 当たり判定
	Collider3D::OBB*	collider_;
	//! メッシュ
	MeshRenderer		mesh_;
	//! モデル
	Model::Game model_;
};

#endif // _WALL_A_H_
