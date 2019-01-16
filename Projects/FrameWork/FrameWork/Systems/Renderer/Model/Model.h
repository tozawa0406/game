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
	const string directoryName = Define::ResourceDirectoryName + "Model/";
public:
	enum class Base : int
	{
		UNOWN = -1,
		SKY,
		MAX,
	};

	enum class Title : int
	{
		MAX = (int)Base::MAX,
	};

	enum class Camp : int
	{
		CUBE = (int)Base::MAX,
		UNITY_CHAN,
		SWORD,
		BOX,
		DRAGON,
		ROCK_1,
		ROCK_4,
		ROCK_6,
		ROCK_10,
		ROCK_12,
		ROCK_14,
		CART,
		CANNON,
		CANNON_BARREL,
		CANNON_BULLET,
		TENT,
		MAX
	};
	enum class Buttle : int
	{
		CUBE = (int)Base::MAX,
		UNITY_CHAN,
		SWORD,
		BOX,
		DRAGON,
		ROCK_1,
		ROCK_4,
		ROCK_6,
		ROCK_10,
		ROCK_12,
		ROCK_14,
		CART,
		CANNON,
		CANNON_BARREL,
		CANNON_BULLET,
		TENT,
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

		enum class Camp : int
		{
			WAIT = (int)Base::MAX,
			WAIT_TIME1,
			WAIT_TIME2,
			WALK,
			RUN,
			ROLL,
			DIVE,
			KNOCK_BACK,
			KNOCK_OUT,
			DIE,
			STANDUP,
			WALK_SETUP,
			SETUP,
			SETUP_DRAWN,
			SETUP_WAIT,
			SETUP_RUN,

			SRASH_1,
			SRASH_2,
			SRASH_3,

			D_WAIT1,
			D_WAIT2,
			D_WALK,
			D_RUN,
			D_SCREAM,
			D_HIT,
			D_DIE,
			D_BITE,
			D_WING_ATTACK,
			D_TAKEOFF,
			D_FLY_IDLE,

			MAX
		};

		enum class Buttle : int
		{
			WAIT = (int)Base::MAX,
			WAIT_TIME1,
			WAIT_TIME2,
			WALK,
			RUN,
			ROLL,
			DIVE,
			KNOCK_BACK,
			KNOCK_OUT,
			DIE,
			STANDUP,
			WALK_SETUP,
			SETUP,
			SETUP_DRAWN,
			SETUP_WAIT,
			SETUP_RUN,

			SRASH_1,
			SRASH_2,
			SRASH_3,

			D_WAIT1,
			D_WAIT2,
			D_WALK,
			D_RUN,
			D_SCREAM,
			D_HIT,
			D_DIE,
			D_BITE,
			D_WING_ATTACK,
			D_TAKEOFF,
			D_FLY_IDLE,

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
	const string baseFileName[(int)Base::MAX]
	{
		{ directoryName + "sky/sky2.bmx" },
	};

	const string campFileName[(int)Camp::MAX - (int)Base::MAX]
	{
		{ directoryName + "cube/cube2.bmx"									},
		{ directoryName + "unitychan/unitychan.bmx"							},
		{ directoryName + "wapon/kohaku_sword/kohaku_sword_notangent.bmx"	},
		{ directoryName + "box/TreasureChest_notangent.bmx"					},
		{ directoryName + "terror_dragon/terror_dragon2.bmx"				},
		{ directoryName + "wallRocks/Rock1/Rock1.bmx"						},
		{ directoryName + "wallRocks/Rock4/Rock4.bmx"						},
		{ directoryName + "wallRocks/Rock6/Rock6.bmx"						},
		{ directoryName + "wallRocks/Rock10/Rock10.bmx"						},
		{ directoryName + "wallRocks/Rock12/Rock12.bmx"						},
		{ directoryName + "wallRocks/Rock14/Rock14.bmx"						},
		{ directoryName + "cart/cart.bmx"									},
		{ directoryName + "cannon/cannon.bmx"								},
		{ directoryName + "cannon/cannon_barrel.bmx"						},
		{ directoryName + "cannon/cannon_bullet.bmx"						},
		{ directoryName + "tent/tent.bmx"									},
	};

	const string buttleFileName[(int)Buttle::MAX - (int)Base::MAX]
	{
		{ directoryName + "cube/cube2.bmx"									},
		{ directoryName + "unitychan/unitychan.bmx"							},
		{ directoryName + "wapon/kohaku_sword/kohaku_sword_notangent.bmx"	},
		{ directoryName + "box/TreasureChest_notangent.bmx"					},
		{ directoryName + "terror_dragon/terror_dragon2.bmx"				},
		{ directoryName + "wallRocks/Rock1/Rock1.bmx"						},
		{ directoryName + "wallRocks/Rock4/Rock4.bmx"						},
		{ directoryName + "wallRocks/Rock6/Rock6.bmx"						},
		{ directoryName + "wallRocks/Rock10/Rock10.bmx"						},
		{ directoryName + "wallRocks/Rock12/Rock12.bmx"						},
		{ directoryName + "wallRocks/Rock14/Rock14.bmx"						},
		{ directoryName + "cart/cart.bmx"									},
		{ directoryName + "cannon/cannon.bmx"								},
		{ directoryName + "cannon/cannon_barrel.bmx"						},
		{ directoryName + "cannon/cannon_bullet.bmx"						},
		{ directoryName + "tent/tent.bmx"									},

	};

	const ANIMATION_INFO animationCampFileName[(int)Animation::Camp::MAX]
	{
		{ directoryName + "unitychan_animation/unitychan_WAIT00.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WAIT01.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WAIT02.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WALK00_F.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_RUN00_F.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Roll.bamx"					, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_dive.bamx"					, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_KnockBack.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_KnockOut.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Die.bamx"					, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Standup.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_walksetup.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Setup.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SetupDrawn.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SetupWait.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SetupWalk.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Slash1.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Slash2.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Slash3.bamx"				, (int)Camp::UNITY_CHAN },

		{ directoryName + "terror_dragon_animation/terror_dragon_idle1.bamx"		, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_idle2.bamx"		, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_walk.bamx"			, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_run.bamx"			, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_scream.bamx"		, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_hit.bamx"			, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_die.bamx"			, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_bite.bamx"			, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_wing_attack.bamx"	, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_takeoff.bamx"		, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_fly_idle.bamx"		, (int)Camp::DRAGON		},
	};
	
	const ANIMATION_INFO animationButtleFileName[(int)Animation::Buttle::MAX]
	{
		{ directoryName + "unitychan_animation/unitychan_WAIT00.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WAIT01.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WAIT02.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WALK00_F.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_RUN00_F.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Roll.bamx"					, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_dive.bamx"					, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_KnockBack.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_KnockOut.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Die.bamx"					, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Standup.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_walksetup.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Setup.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SetupDrawn.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SetupWait.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SetupWalk.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Slash1.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Slash2.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Slash3.bamx"				, (int)Camp::UNITY_CHAN },

		{ directoryName + "terror_dragon_animation/terror_dragon_idle1.bamx"		, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_idle2.bamx"		, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_walk.bamx"			, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_run.bamx"			, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_scream.bamx"		, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_hit.bamx"			, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_die.bamx"			, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_bite.bamx"			, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_wing_attack.bamx"	, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_takeoff.bamx"		, (int)Camp::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_fly_idle.bamx"		, (int)Camp::DRAGON		},
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
