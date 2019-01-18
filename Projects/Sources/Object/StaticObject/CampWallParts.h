/*
 * @file		CampWallParts.h
 * @brief		キャンプの壁の一面
 * @author		戸澤翔太
 * @data		2018/11/27
 */
#ifndef _CAMP_WALL_PARTS_H_
#define _CAMP_WALL_PARTS_H_

#include "WallParts.h"

class CampWallParts : public WallParts
{
public:
	/* @brief	コンストラクタ
	 * @param	(position)		初期位置
	 * @param	(rotation)		初期回転角		*/
	CampWallParts(const VECTOR3& position, const VECTOR3& rotation);
	/* @brief	デストラクタ		*/
	~CampWallParts(void);
	


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
	Collider3D::OBB*	entranceCollider_[2];	//! 入口用の当たり判定
};

#endif // _CAMP_WALL_PARTS_H_
