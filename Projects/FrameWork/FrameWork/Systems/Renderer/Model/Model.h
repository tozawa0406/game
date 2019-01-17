//-----------------------------------------------------------------------------
//
//	ÉÇÉfÉãä«óù[Model.h]
//	Auther : åÀ‡V„ƒëæ
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_

#include "../../../Define/Define.h"
#include "../../BaseManager.h"

enum class MaterialType
{
	Diffuse = 0,
	Normal,

	MAX
};

struct MATERIAL
{
	COLOR		ambient;
	COLOR		diffuse;
	COLOR		specular;
	COLOR		emission;
	float		power;
	uint		texture[static_cast<int>(MaterialType::MAX)];
	string		textureName[static_cast<int>(MaterialType::MAX)];
};

struct BONE
{
	string	name;
	MATRIX	offsetMtx;
	MATRIX  inverseMtx;
	std::vector<std::vector<MATRIX>> animMtx;
	MATRIX  nowBone;
};

struct MESH
{
	string						name;
	MATRIX						transMtx;
	std::vector<VERTEX>			vertex;
	std::vector<WORD>			index;
	uint						vertexBuffer;
	uint						indexBuffer;
	uint						computeShader;
	MATERIAL					material;
};

struct MODEL
{
	MATRIX				transMtx;
	std::vector<MESH>	mesh;

	std::vector<BONE>	bone;
};

class Loading;
class Model : public Interface
{
	friend class Systems;
public:
	~Model(void) {}
	int		SetUpLoading(Loading* loading, int sceneNum);
	HRESULT Load(int sceneNum);
	void	Release(bool uninit = false);

private:
	Model(Systems* systems);
	HRESULT Init(void)   override;
	void	Uninit(void) override;

	int sceneNum_;
	Loading* loading_;
};

#endif // _X_MODEL_H_
