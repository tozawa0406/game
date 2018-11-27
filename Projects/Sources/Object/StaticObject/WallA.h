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
public:
	WallA(const VECTOR3& position, const VECTOR3& rotation);
	~WallA(void);

	void Init(void) override;
	void Uninit(void) override;

	/* @fn		Update
	 * @brief	更新処理(動かないためなし)		*/
	void Update(void) override {}

private:
	Collider3D::Plane* collider_;

	//! メッシュ
	MeshRenderer mesh_[2];

	Transform transformMesh_[2];
};

#endif // _WALL_A_H_
