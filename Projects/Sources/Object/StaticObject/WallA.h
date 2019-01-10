/*
 * @file		WallA.h
 * @brief		壁
 * @author		戸澤翔太
 * @data		2018/11/27
 */
#ifndef _WALL_A_H_
#define _WALL_A_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class WallA : public Object
{
	//! @def	1つの壁に2つのメッシュを使用
	static constexpr int MESH_NUM = 2;
public:
	/* @brief	コンストラクタ
	 * @param	(position)		初期位置
	 * @param	(rotation)		初期回転角		*/
	WallA(const VECTOR3& position, const VECTOR3& rotation);
	/* @brief	デストラクタ		*/
	~WallA(void);
	


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
	Collider3D::Plane*	collider_;				//! 当たり判定
	Collider3D::OBB*	entranceCollider_[2];	//! 入口用の当たり判定

	//! メッシュ
	MeshRenderer	mesh_[MESH_NUM];
	//! メッシュの姿勢
	Transform		transformMesh_[MESH_NUM];
};

#endif // _WALL_A_H_
