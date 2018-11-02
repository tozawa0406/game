//-----------------------------------------------------------------------------
//
//	fĮ[Model.h]
//	Auther : ËāVãÄū
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_

#include "../../../Define/Define.h"
#include "../../BaseManager.h"

struct MATERIAL
{
	COLOR		ambient;
	COLOR		diffuse;
	COLOR		specular;
	COLOR		emission;
	float		power;
	UINT		texture;
	string		textureName;
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
	UINT						vertexBuffer;
	UINT						indexBuffer;
	UINT						computeShader;
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
	const string directoryName = Define::ResourceDirectoryName + "Model/";
public:
	enum class Base : int
	{
		UNOWN = -1,
		MAX,
	};

	enum class Title : int
	{
		MAX = (int)Base::MAX,
	};

	enum class Game : int
	{
		CUBE = (int)Base::MAX,
		SKY,
		UNITY_CHAN,
		SWORD,
		BOX,
		DRAGON,
		MAX
	};
	enum class Result : int
	{
		MAX = (int)Base::MAX,
	};

	class Animation
	{
	public:
		enum class Base : int
		{
			UNOWN = -1,
			MAX,
		};

		enum class Title : int
		{
			MAX = (int)Base::MAX,
		};

		enum class Game : int
		{
			WAIT = (int)Base::MAX,
			WAIT_TIME1,
			WAIT_TIME2,
			WALK,
			RUN,
			ROLL,
			SETUP,
			SETUP_WAIT,
			SETUP_RUN,

			SRASH_1,
			SRASH_2,
			SRASH_3,

			MAX
		};
		enum class Result : int
		{
			MAX = (int)Base::MAX,
		};

	};

	struct ANIMATION_INFO
	{
		string	fileName;
		int		parent;
	};

private:
	const string gameFileName[(int)Game::MAX]
	{
		{ directoryName + "cube/cube.bmx"						},
		{ directoryName + "sky/sky.bmx"							},
		{ directoryName + "unitychan/unitychan.bmx"				},
		{ directoryName + "wapon/kohaku_sword/kohaku_sword.bmx" },
		{ directoryName + "box/TreasureChest.bmx"				},
		{ directoryName + "dragon/dragon.bmx"					},
	};

	const ANIMATION_INFO animationGameFileName[(int)Animation::Game::MAX]
	{
		{ directoryName + "unitychan_animation/unitychan_WAIT00.bamx"		, (int)Game::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WAIT01.bamx"		, (int)Game::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WAIT02.bamx"		, (int)Game::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WALK00_F.bamx"		, (int)Game::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_RUN00_F.bamx"		, (int)Game::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Roll.bamx"			, (int)Game::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Setup.bamx"		, (int)Game::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SetupWait.bamx"	, (int)Game::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SetupWalk.bamx"	, (int)Game::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SLASH1.bamx"		, (int)Game::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SLASH2.bamx"		, (int)Game::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SLASH3.bamx"		, (int)Game::UNITY_CHAN },
	};

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
